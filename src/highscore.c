/*
 * highscore.c
 */

#include "common.h"

#include "background.h"
#include "highscore.h"
#include "stage.h"
#include "text.h"
#include "title.h"
#include "time.h"

extern App        app;
extern Highscores highscores;

static void logic(void);
static void draw(void);
static int  highscoreComparator(const void *a, const void *b);
static void drawHighscores(void);
static void doNameInput(void);
static void drawNameInput(void);

static Highscore *newHighscore;
static int cursorBlink;
static int lastScore;


void initHighscoreTable(void) {
    int i;

    memset(&highscores, 0, sizeof(highscores));

    for (i = 0; i < NUM_HIGHSCORES; i++) {
        highscores.highscore[i].score = 0;
        highscores.highscore[i].name[0] = '\0';
        highscores.highscore[i].recent = 0;
    }

    newHighscore = NULL;
    cursorBlink  = 0;
    lastScore    = 0;
}

void initHighscores(void)
{
    app.delegate.logic = logic;
    app.delegate.draw  = draw;

    memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS);

    cursorBlink = 0;
}

static void logic(void){
    doBackground();
    doStarfield();

    if (newHighscore != NULL) {
        doNameInput();
    } else {
        if (app.keyboard[SDL_SCANCODE_RETURN]) {
            app.keyboard[SDL_SCANCODE_RETURN] = 0;
            initStage();
        }
    }

    if (++cursorBlink >= FPS) {
        cursorBlink = 0;
    }
}

static void doNameInput(void)
{
    int  i;
    int  n;
    int  inputLen;
    char c;

    static int enterPressed = 0;

    if (newHighscore == NULL) {
        return;
    }

    n = strlen(newHighscore->name);
    inputLen = strlen(app.inputText);

    for (i = 0; i < inputLen; i++) {
        c = toupper(app.inputText[i]);

        if (n < MAX_SCORE_NAME_LENGTH - 1 && c >= ' ' && c <= 'Z') {
            newHighscore->name[n++] = c;
            newHighscore->name[n] = '\0';
        }
    }

    if (n > 0 && app.keyboard[SDL_SCANCODE_BACKSPACE]) {
        newHighscore->name[--n] = '\0';
        app.keyboard[SDL_SCANCODE_BACKSPACE] = 0;
    }

    /*
     * ENTER confirma o nome.
     * Depois disso, newHighscore vira NULL e a tela passa
     * automaticamente para drawHighscores().
     */
    if (app.keyboard[SDL_SCANCODE_RETURN]) {
        if (!enterPressed) {
            if (n == 0) {
                STRNCPY(newHighscore->name, "PLAYER", MAX_SCORE_NAME_LENGTH);
            }

            newHighscore = NULL;

            /*
             * Consome o ENTER para evitar que o mesmo ENTER
             * confirme o nome e já inicie uma nova partida.
             */
            app.keyboard[SDL_SCANCODE_RETURN] = 0;
        }

        enterPressed = 1;
    } else {
        enterPressed = 0;
    }
}
static void draw(void)
{
    drawBackground();
    drawStarfield();

    if (newHighscore != NULL) {
        drawNameInput();
    } else {
        drawHighscores();
    }
}

static void drawNameInput(void)
{
    SDL_Rect r;

    drawText(SCREEN_WIDTH / 2, 70,
             255, 255, 255,
             TEXT_CENTER,
             "NOVO RECORDE");

    drawText(SCREEN_WIDTH / 2, 120,
             255, 255, 255,
             TEXT_CENTER,
             "DIGITE SEU NOME ABAIXO");

    drawText(SCREEN_WIDTH / 2, 250,
             128, 255, 128,
             TEXT_CENTER,
             newHighscore->name);

    if (cursorBlink < FPS / 2) {
        r.x = ((SCREEN_WIDTH / 2) +
               (strlen(newHighscore->name) * GLYPH_WIDTH) / 2) + 5;
        r.y = 250;
        r.w = GLYPH_WIDTH;
        r.h = GLYPH_HEIGHT;

        SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(app.renderer, &r);
    }

    drawText(SCREEN_WIDTH / 2, 625,
             255, 255, 255,
             TEXT_CENTER,
             "PRESSIONE ENTER QUANDO TERMINAR");
}

static void drawHighscores(void){
    int i;
    int y;
    int r, g, b;
    int hasScore;

    y = 180;
    hasScore = 0;

    drawText(SCREEN_WIDTH / 2, 50,
             255, 255, 255,
             TEXT_CENTER,
             "HIGHSCORE");

    drawText(SCREEN_WIDTH / 2, 100,
             255, 255, 0,
             TEXT_CENTER,
             "SCORE DA PARTIDA: %03d",
             lastScore);

    for (i = 0; i < NUM_HIGHSCORES; i++) {
        if (highscores.highscore[i].score <= 0) {
            continue;
        }

        hasScore = 1;

        r = 255;
        g = 255;
        b = 255;

        if (highscores.highscore[i].recent) {
            b = 0;
        }

        drawText(SCREEN_WIDTH / 2, y,
                 r, g, b,
                 TEXT_CENTER,
                 "#%d. %-15s......%03d",
                 i + 1,
                 highscores.highscore[i].name,
                 highscores.highscore[i].score);

        y += 45;
    }

    if (!hasScore) {
        drawText(SCREEN_WIDTH / 2, 300,
                 255, 255, 255,
                 TEXT_CENTER,
                 "NENHUMA PONTUACAO AINDA");
    }

    drawText(SCREEN_WIDTH / 2, 650,
             255, 255, 255,
             TEXT_CENTER,
             "PRESSIONE ENTER PARA JOGAR NOVAMENTE");
}

void addHighscore(int score)
{
    int i;
    Highscore newHighscores[NUM_HIGHSCORES + 1];

    lastScore = score;

    memset(newHighscores, 0, sizeof(Highscore) * (NUM_HIGHSCORES + 1));

    for (i = 0; i < NUM_HIGHSCORES; i++) {
        newHighscores[i] = highscores.highscore[i];
        newHighscores[i].recent = 0;
    }

    newHighscores[NUM_HIGHSCORES].score = score;
    newHighscores[NUM_HIGHSCORES].recent = 1;

    qsort(newHighscores,
          NUM_HIGHSCORES + 1,
          sizeof(Highscore),
          highscoreComparator);

    newHighscore = NULL;

    for (i = 0; i < NUM_HIGHSCORES; i++) {
        highscores.highscore[i] = newHighscores[i];

        if (highscores.highscore[i].recent) {
            newHighscore = &highscores.highscore[i];
        }
    }
}

static int highscoreComparator(const void *a, const void *b)
{
    const Highscore *h1 = (const Highscore *)a;
    const Highscore *h2 = (const Highscore *)b;

    return h2->score - h1->score;
}