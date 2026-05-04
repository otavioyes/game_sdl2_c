/*
 * highscore.c
 *
 * Gerencia a tabela de recordes do jogo: exibição, entrada de nome,
 * ordenação e geração de nomes aleatórios estilo arcade.
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

/* Declarações antecipadas das funções internas */
static void logic(void);
static void draw(void);
static int  highscoreComparator(const void *a, const void *b);
static void drawHighscores(void);
static void doNameInput(void);
static void drawNameInput(void);
static const char *randomArcadeName(void);

/* Ponteiro para o novo recorde que está sendo inserido (NULL se nenhum) */
static Highscore *newHighscore;

/* Contador para piscar o cursor de digitação */
static int cursorBlink;

/* Contador regressivo antes de voltar ao título */
static int timeout;

/* -----------------------------------------------------------------------
 * Nomes aleatórios estilo arcade clássico
 * Compostos por prefixo + sufixo para gerar variedade
 * ----------------------------------------------------------------------- */
static const char *arcadePrefixes[] = {
        "ACE", "RED", "BIG", "ICE", "MAD", "NEO", "SKY", "ZAP",
        "DARK", "IRON", "STAR", "NOVA", "CYBER", "BLAZE", "GHOST",
        "TURBO", "HYPER", "ULTRA", "MEGA", "ALPHA"
};

static const char *arcadeSuffixes[] = {
        "X", "Z", "ACE", "ONE", "MAX", "RAY", "GUN", "BOT",
        "HAWK", "WOLF", "BYTE", "ZERO", "FORCE", "BLADE", "STORM",
        "RIDER", "SNAKE", "CLAW", "FURY", "PRIME"
};

/*
 * Retorna um nome arcade aleatório combinando prefixo + sufixo.
 * Ex: "TURBO" + "HAWK" = "TURBOHAWK"
 * Usa buffer estático — não chamar duas vezes na mesma expressão.
 */
static const char *randomArcadeName(void) {
    static char name[MAX_SCORE_NAME_LENGTH];

    int numPrefixes = sizeof(arcadePrefixes) / sizeof(arcadePrefixes[0]);
    int numSuffixes = sizeof(arcadeSuffixes) / sizeof(arcadeSuffixes[0]);

    const char *prefix = arcadePrefixes[rand() % numPrefixes];
    const char *suffix = arcadeSuffixes[rand() % numSuffixes];

    snprintf(name, sizeof(name), "%s%s", prefix, suffix);
    return name;
}

/*
 * Inicializa a tabela de recordes com valores padrão.
 * Cada posição recebe uma pontuação decrescente e um nome arcade aleatório.
 * Chamada uma vez ao iniciar o jogo.
 */
void initHighscoreTable(void) {
    int i;

    memset(&highscores, 0, sizeof(highscores));

    /* Semente do gerador aleatório baseada no tempo atual */
    srand((unsigned int)time(NULL));

    for (i = 0; i < NUM_HIGHSCORES; i++) {
        /* Pontuação decrescente: 1º lugar tem a maior pontuação */
        highscores.highscore[i].score = (NUM_HIGHSCORES - i) * 100;

        /* Nome aleatório estilo arcade para cada posição */
        STRNCPY(highscores.highscore[i].name, randomArcadeName(), MAX_SCORE_NAME_LENGTH);
    }

    newHighscore = NULL;
    cursorBlink  = 0;
}

/*
 * Configura os delegates de lógica e desenho para a tela de highscores.
 * Chamada toda vez que a tela de recordes é exibida.
 */
void initHighscores(void) {
    app.delegate.logic = logic;
    app.delegate.draw  = draw;

    /* Limpa o estado do teclado para evitar inputs residuais */
    memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS);

    /* Tempo antes de retornar automaticamente ao título (em frames) */
    timeout = FPS * 5;
}

/*
 * Lógica principal da tela de highscores, chamada a cada frame.
 * Atualiza fundo, estrelas, entrada de nome e timeout.
 */
static void logic(void) {
    doBackground();
    doStarfield();

    if (newHighscore != NULL) {
        /* Há um novo recorde: processa a entrada do nome do jogador */
        doNameInput();
    } else {
        /* Sem novo recorde: conta regressiva para voltar ao título */
        if (--timeout <= 0) {
            initTitle();
        }
        /* Atalho: ENTER inicia uma nova partida direto */
        if (app.keyboard[SDL_SCANCODE_KP_ENTER]) {
            initStage();
        }
    }

    /* Cicla o cursor piscante entre 0 e FPS-1 */
    if (++cursorBlink >= FPS) {
        cursorBlink = 0;
    }
}

/*
 * Processa a entrada de texto para o nome do novo recordista.
 * Suporta: digitação de letras/números, backspace e confirmação com Enter.
 */
static void doNameInput(void) {
    if (newHighscore == NULL) {
        return;
    }

    int  i, n;
    char c;

    n = strlen(newHighscore->name);
    int inputLen = strlen(app.inputText);

    /* Adiciona cada caractere digitado (convertido para maiúsculo) */
    for (i = 0; i < inputLen; i++) {
        c = toupper(app.inputText[i]);

        /* Aceita apenas caracteres imprimíveis até o limite de tamanho */
        if (n < MAX_SCORE_NAME_LENGTH - 1 && c >= ' ' && c <= 'Z') {
            newHighscore->name[n++] = c;
            newHighscore->name[n]   = '\0'; /* mantém a string sempre terminada */
        }
    }

    /* Backspace: remove o último caractere digitado */
    if (n > 0 && app.keyboard[SDL_SCANCODE_BACKSPACE]) {
        newHighscore->name[--n] = '\0';
        app.keyboard[SDL_SCANCODE_BACKSPACE] = 0; /* consome o input */
    }

    /*
     * Enter: confirma o nome e encerra a entrada.
     * A flag enterPressed evita que o Enter seja detectado em múltiplos frames.
     */
    static int enterPressed = 0;

    if (app.keyboard[SDL_SCANCODE_RETURN]) {
        if (!enterPressed) {
            /* Nome vazio recebe um nome arcade aleatório */
            if (n == 0) {
                STRNCPY(newHighscore->name, randomArcadeName(), MAX_SCORE_NAME_LENGTH);
            }
            newHighscore = NULL; /* libera o ponteiro, encerrando a entrada */
        }
        enterPressed = 1;
    } else {
        enterPressed = 0;
    }
}

/*
 * Renderiza a tela de highscores a cada frame.
 * Alterna entre a entrada de nome e a exibição da tabela.
 */
static void draw(void) {
    drawBackground();
    drawStarfield();

    if (newHighscore != NULL) {
        /* Modo de entrada: mostra o campo de digitação */
        drawNameInput();
    } else {
        /* Modo de exibição: mostra a tabela de recordes */
        drawHighscores();

        /* Texto piscante de instrução (alterna a cada 20 frames) */
        if (timeout % 40 < 20) {
            drawText(SCREEN_WIDTH / 2, 600, 255, 255, 255,
                     TEXT_CENTER, "PRESSIONE ENTER PARA INICIAR");
        }
    }
}

/*
 * Desenha a tela de entrada de nome para o novo recordista.
 * Exibe o nome atual com um cursor piscante ao final.
 */
static void drawNameInput(void) {
    SDL_Rect r;

    drawText(SCREEN_WIDTH / 2, 70,  255, 255, 255, TEXT_CENTER, "NOVO RECORDE");
    drawText(SCREEN_WIDTH / 2, 120, 255, 255, 255, TEXT_CENTER, "DIGITE SEU NOME ABAIXO");

    /* Nome digitado em verde */
    drawText(SCREEN_WIDTH / 2, 250, 128, 255, 128, TEXT_CENTER, newHighscore->name);

    /* Cursor piscante: visível na primeira metade do ciclo */
    if (cursorBlink < FPS / 2) {
        /* Posiciona o cursor logo após o último caractere do nome */
        r.x = ((SCREEN_WIDTH / 2) + (strlen(newHighscore->name) * GLYPH_WIDTH) / 2) + 5;
        r.y = 250;
        r.w = GLYPH_WIDTH;
        r.h = GLYPH_HEIGHT;

        SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(app.renderer, &r);
    }

    drawText(SCREEN_WIDTH / 2, 625, 255, 255, 255, TEXT_CENTER,
             "PRESSIONE ENTER QUANDO TERMINAR");
}

/*
 * Desenha a tabela de recordes com posição, nome e pontuação.
 * Entradas recentes são destacadas em amarelo (sem azul).
 */
static void drawHighscores(void) {
    int i, y, r, g, b;

    y = 150;

    drawText(SCREEN_WIDTH / 2, 70, 255, 255, 255, TEXT_CENTER, "HIGHSCORE");

    for (i = 0; i < NUM_HIGHSCORES; i++) {
        r = 255;
        g = 255;
        b = 255;

        /* Destaque amarelo para o recorde recém-adicionado */
        if (highscores.highscore[i].recent) {
            b = 0;
        }

        drawText(SCREEN_WIDTH / 2, y, r, g, b, TEXT_CENTER,
                 "#%d. %-15s......%03d",
                 (i + 1),
                 highscores.highscore[i].name,
                 highscores.highscore[i].score);

        y += 50;
    }
}

/*
 * Adiciona uma nova pontuação à tabela de recordes.
 * Cria um array temporário com NUM_HIGHSCORES + 1 entradas,
 * ordena em ordem decrescente e mantém apenas os melhores.
 * Se a nova pontuação entrou na tabela, ativa a entrada de nome.
 */
void addHighscore(int score) {
    int i;
    Highscore newHighscores[NUM_HIGHSCORES + 1];

    /* Copia os recordes atuais para o array temporário, limpando o flag recent */
    memset(newHighscores, 0, sizeof(Highscore) * (NUM_HIGHSCORES + 1));
    for (i = 0; i < NUM_HIGHSCORES; i++) {
        newHighscores[i]        = highscores.highscore[i];
        newHighscores[i].recent = 0;
    }

    /* Insere a nova pontuação na última posição do array temporário */
    newHighscores[NUM_HIGHSCORES].score  = score;
    newHighscores[NUM_HIGHSCORES].recent = 1;

    /* Ordena do maior para o menor score */
    qsort(newHighscores, NUM_HIGHSCORES + 1, sizeof(Highscore), highscoreComparator);

    /* Copia apenas os primeiros NUM_HIGHSCORES de volta para a tabela */
    newHighscore = NULL;
    for (i = 0; i < NUM_HIGHSCORES; i++) {
        highscores.highscore[i] = newHighscores[i];

        /* Se este slot é o recorde recente, ativa a entrada de nome */
        if (highscores.highscore[i].recent) {
            newHighscore = &highscores.highscore[i];
        }
    }
}

/*
 * Comparador para qsort: ordena Highscores em ordem decrescente de pontuação.
 * Retorna positivo se h2 > h1 (h2 vem antes na ordem final).
 */
static int highscoreComparator(const void *a, const void *b) {
    Highscore *h1 = (Highscore *)a;
    Highscore *h2 = (Highscore *)b;

    return h2->score - h1->score;
}