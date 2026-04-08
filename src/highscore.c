/*
 * highsocre.c
 */

#include "common.h"

#include "background.h"
#include "highscore.h"
#include "stage.h"
#include "text.h"
#include "title.h"

extern App app;
extern Highscore highscore;

static void logic(void);
static void draw(void);
static int highscoreComparator(const void *a, const void *b);
static void drawHighScore(void);
static void doNameInput(void);
static void drawNameInput(void);

static Highscore *newHighscore;
static int cursorBlink;
static int timeout;

void initHighscore(void) {
    int i;

    memset(&highscore, 0, sizeof(highscore));
    for (i = 0; i < MAX_HIGHSCORE; i++) {
        highscore.highscore[i].score = NUM_HIGHSCORES - i;
        STRNCPY(highscore.highscore[i].name, "OTAVIO CORREA", MAX_SCORE_NAME_LENGTH);
    }
    newHighscore = NULL;
    cursorBlink = 0;
}

void initHighscore(void) {
    app.delegate.logic = logic;
    app.delegate.draw = draw;

    memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_SIZE);
    timeout = FPS * 5;
}

static void logic(void) {
    doBackground();
    doStarfield();

    if (newHighscore != NULL) {
        doNameInput();
    }else {
        if (--timeout <= 0) {
            initTitle();
        }
        if (app.keyboard[SDL_SCANCODE_KP_ENTER]) {
            initStage();
        }
    }
    if (++cursorBlink >= FPS) {
        cursorBlink = 0;
    }
}

static void doNameInput(void) {
    int i, n;
    char c;

    n = strlen(newHighscore->name);
    for (i = 0; i < strlen(app.inputText); i++) {
        c = toupper(app.inputText[i]);
        if (n < MAX_SCORE_NAME_LENGTH - 1 && C >= ' ' && c <= 'Z') {
            newHighscore->name[n++] = c;
        }
    }
    if (n > 0 && app.keyboard[SDL_SCANCODE_KP_BACKSPACE]) {
        newHighscore->name[--n] = '\0';
        app.keyboard[SDL_SCANCODE_KP_BACKSPACE] = 0;
    }
    if (app.keyboard[SDL_SCANCODE_RETURN]) {
        if (strlen(newHighscore->name) == 0) {
            STRNCPY(newHighscore->name, "OTAVIO CORREA", MAX_SCORE_NAME_LENGTH);
        }
        newHighscore = NULL;
    }
}

static void draw(void) {
    drawBackground();
    drawStarfield();

    if (newHighscore != NULL) {
        drawNameInput();
    }else {
        drawHighScore();
        if (timeout % 40 < 20) {
            drawText(SCREEN_WIDTH / 2, 600, 255, 255, 255,
            TEXT_CENTER, "PRESS FIRE TO PLAY");
        }
    }
}

static void drawNameInput(void) {
    SDL_Rect r;

    drawText(SCREEN_WIDTH / 2, 70, 255, 255, 255,TEXT_CENTER,
        "CONGRATULATIONS, YOU'VE GAINED A HIGHSCORE");

    drawText(SCREEN_WIDTH / 2, 120, 255, 255, 255,TEXT_CENTER,
        "ENTER YOUR NAME BELOW");

    drawText(SCREEN_WIDTH / 2, 250, 128, 255, 128, TEXT_CENTER,
        newHighscore->name);

    if (cursorBlink < FPS / 2) {
        r.x = ((SCREEN_WIDTH / 2) + (strlen(newHighscore->name) * GLYPH_WIDTH) / 2) + 5;
        r.y = 250;
        r.w = GLYPH_WIDTH;
        r.h = GLYPH_HEIGHT;

        SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(app.renderer, &r);
    }
    drawText(SCREEN_WIDTH / 2, 625, 255, 255, 255,TEXT_CENTER,
        "PRESS RETURN WHEN FINISHED");
}

static void drawHighscore(void) {
    int i, y, r, g, b;

    y = 150;

    drawText(SCREEN_WIDTH /2, 70, 255, 255, 255, TEXT_CENTER,
        "HIGHSCORE");
    for (i = 0; i < MAX_HIGHSCORES; i++) {
        r = 255;
        g = 255;
        b = 255;
        if (highscore.highscore[i].recent) {
            b = 0;
        }
        drawText(SCREEN_WIDTH / 2, y, r, g, b, TEXT_CENTER,
            "#%d. %-15s......%03d", (i + 1), highscore.highscore[i].name, highscore.highscore[i].score);
        y += 50;
    }
}

void addHighscore(int score) {
    int i;
    Highscore newHighscore[NUM_HIGHSCORES + 1];

    memset(newHighscore, 0, sizeof(Highscore) * (NUM_HIGHSCORES + 1));
    for (i = 0; i < NUM_HIGHSCORES; i++) {
        newHighscore[i] = highscore.highscore[i];
        newHighscore[i].recent = 0;
    }
    newHighscore[NUM_HIGHSCORES].score = score;
    newHighscore[NUM_HIGHSCORES].recent = 1;

    qsort(newHighscores, NUM_HIGHSCORES + 1, sizeof(Highscore), highscoreComparator);

    newHighscore = NULL;

    for (i = 0; i < NUM_HIGHSCORES; i++) {
        highscore.highscore[i] = newHighscore[i];
        if (highscore.highscore[i].recent) {
            newHighscore = &highscore.highscore[i];
        }
    }
}

static int highscoreComparator(const void *a, const void *b) {
    Highscore *h1 = ((Highscore *)a);
    Highscore *h2 = ((Highscore *)b);

    return h2->score - h1->score;
}