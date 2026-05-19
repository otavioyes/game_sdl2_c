/*
 * highscore.c
 */

#include "common.h"

#include "background.h"
#include "highscore.h"
#include "stage.h"
#include "text.h"
#include "title.h"

extern App        app;
extern Highscores highscores;

static void logic(void);
static void draw(void);
static int  highscoreComparator(const void *a, const void *b);
static void drawHighscores(void);
static void doNameInput(void);
static void drawNameInput(void);

static Highscore *newHighscore;
static int        cursorBlink;
static int        lastScore;


/*==============================================================================
 * Inicializa a tabela de highscores.
 *
 * Responsabilidades:
 * - Zerar a estrutura global de pontuações
 * - Inicializar todos os scores como vazios
 * - Limpar marcação de score recente
 * - Reiniciar estado de input de nome
 *============================================================================*/
void initHighscoreTable(void)
{
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


/*==============================================================================
 * Inicializa a tela de highscores.
 *
 * Responsabilidades:
 * - Definir delegates de lógica e renderização
 * - Limpar estado atual do teclado
 * - Reiniciar cursor visual de input
 *============================================================================*/
void initHighscores(void)
{
    app.delegate.logic = logic;
    app.delegate.draw  = draw;

    /*
     * Limpa teclado para evitar que um ENTER vindo da fase
     * seja reaproveitado automaticamente nesta tela.
     */
    memset(app.keyboard, 0, sizeof(int) * MAX_KEYBOARD_KEYS);

    cursorBlink = 0;
}


/*==============================================================================
 * Atualiza a lógica da tela de highscores.
 *
 * Responsabilidades:
 * - Atualizar background e starfield
 * - Processar input de nome quando houver novo highscore
 * - Permitir retorno para nova partida após exibir ranking
 * - Atualizar animação do cursor piscante
 *============================================================================*/
static void logic(void)
{
    doBackground();
    doStarfield();

    if (newHighscore != NULL) {
        doNameInput();

    } else {
        /*
         * Após o nome ser confirmado, ENTER inicia nova partida.
         */
        if (app.keyboard[SDL_SCANCODE_RETURN]) {
            app.keyboard[SDL_SCANCODE_RETURN] = 0;
            initStage();
        }
    }

    /*
     * Atualiza contador usado para piscar o cursor
     * durante a digitação do nome.
     */
    if (++cursorBlink >= FPS) {
        cursorBlink = 0;
    }
}


/*==============================================================================
 * Processa entrada de texto para o nome do jogador.
 *
 * Responsabilidades:
 * - Ler caracteres digitados via app.inputText
 * - Converter caracteres para maiúsculo
 * - Respeitar limite máximo do nome
 * - Processar BACKSPACE
 * - Confirmar nome com ENTER
 * - Usar nome padrão caso o jogador confirme vazio
 *============================================================================*/
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

    /*
     * Processa todos os caracteres recebidos neste frame.
     */
    for (i = 0; i < inputLen; i++) {
        c = toupper(app.inputText[i]);

        /*
         * Aceita caracteres imprimíveis básicos e impede
         * escrita além do limite do buffer.
         */
        if (n < MAX_SCORE_NAME_LENGTH - 1 && c >= ' ' && c <= 'Z') {
            newHighscore->name[n++] = c;
            newHighscore->name[n] = '\0';
        }
    }

    /*
     * Remove último caractere digitado.
     *
     * A tecla é consumida para evitar múltiplas remoções
     * em apenas um frame de input.
     */
    if (n > 0 && app.keyboard[SDL_SCANCODE_BACKSPACE]) {
        newHighscore->name[--n] = '\0';
        app.keyboard[SDL_SCANCODE_BACKSPACE] = 0;
    }

    /*
     * ENTER confirma o nome.
     *
     * O controle enterPressed evita múltiplas confirmações
     * enquanto a tecla estiver pressionada.
     */
    if (app.keyboard[SDL_SCANCODE_RETURN]) {
        if (!enterPressed) {

            /*
             * Nome padrão usado quando o jogador confirma
             * sem digitar nenhum caractere.
             */
            if (n == 0) {
                STRNCPY(newHighscore->name, "PLAYER", MAX_SCORE_NAME_LENGTH);
            }

            /*
             * Finaliza modo de input.
             *
             * A partir daqui, a tela passa a desenhar
             * diretamente a tabela de highscores.
             */
            newHighscore = NULL;

            /*
             * Consome ENTER para impedir que a mesma tecla
             * confirme o nome e já inicie uma nova partida.
             */
            app.keyboard[SDL_SCANCODE_RETURN] = 0;
        }

        enterPressed = 1;

    } else {
        enterPressed = 0;
    }
}


/*==============================================================================
 * Renderiza a tela de highscores.
 *
 * Responsabilidades:
 * - Desenhar background e starfield
 * - Exibir input de nome quando houver novo score
 * - Exibir tabela de highscores após confirmação
 *============================================================================*/
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


/*==============================================================================
 * Renderiza interface de digitação do nome.
 *
 * Responsabilidades:
 * - Exibir título de novo recorde
 * - Exibir instruções para o jogador
 * - Exibir nome digitado
 * - Renderizar cursor piscante
 * - Informar confirmação por ENTER
 *============================================================================*/
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

    /*
     * Desenha cursor piscante ao final do nome.
     */
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
             "PRESSIONE ENTER PARA CONFIRMAR");
}


/*==============================================================================
 * Renderiza a tabela de highscores.
 *
 * Responsabilidades:
 * - Exibir título da tela
 * - Mostrar score da última partida
 * - Listar pontuações registradas
 * - Destacar pontuação mais recente
 * - Exibir mensagem quando não houver pontuações
 * - Exibir instrução para iniciar nova partida
 *============================================================================*/
static void drawHighscores(void)
{
    int i;
    int y;
    int r;
    int g;
    int b;
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

    /*
     * Percorre a tabela exibindo apenas pontuações válidas.
     */
    for (i = 0; i < NUM_HIGHSCORES; i++) {
        if (highscores.highscore[i].score <= 0) {
            continue;
        }

        hasScore = 1;

        r = 255;
        g = 255;
        b = 255;

        /*
         * Destaca visualmente o score mais recente.
         */
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


/*==============================================================================
 * Adiciona uma nova pontuação à tabela de highscores.
 *
 * Responsabilidades:
 * - Guardar score da última partida
 * - Copiar scores atuais para uma tabela temporária
 * - Inserir novo score
 * - Ordenar pontuações em ordem decrescente
 * - Manter apenas os melhores resultados
 * - Marcar o score recém-adicionado como recente
 *============================================================================*/
void addHighscore(int score)
{
    int i;
    Highscore tempHighscores[NUM_HIGHSCORES + 1];

    lastScore = score;

    memset(tempHighscores, 0, sizeof(Highscore) * (NUM_HIGHSCORES + 1));

    /*
     * Copia highscores existentes para tabela temporária.
     * A marcação recent é limpa para que apenas o novo score
     * seja destacado após a ordenação.
     */
    for (i = 0; i < NUM_HIGHSCORES; i++) {
        tempHighscores[i] = highscores.highscore[i];
        tempHighscores[i].recent = 0;
    }

    /*
     * Insere novo score no slot extra.
     *
     * O nome será preenchido posteriormente pela tela
     * de input, caso o score entre no top ranking.
     */
    tempHighscores[NUM_HIGHSCORES].score = score;
    tempHighscores[NUM_HIGHSCORES].name[0] = '\0';
    tempHighscores[NUM_HIGHSCORES].recent = 1;

    /*
     * Ordena do maior score para o menor score.
     */
    qsort(tempHighscores,
          NUM_HIGHSCORES + 1,
          sizeof(Highscore),
          highscoreComparator);

    newHighscore = NULL;

    /*
     * Copia apenas os melhores scores de volta
     * para a tabela principal.
     */
    for (i = 0; i < NUM_HIGHSCORES; i++) {
        highscores.highscore[i] = tempHighscores[i];

        /*
         * Se o novo score ficou dentro do ranking,
         * entra no modo de digitação de nome.
         */
        if (highscores.highscore[i].recent) {
            newHighscore = &highscores.highscore[i];
        }
    }
}


/*==============================================================================
 * Compara duas pontuações para ordenação decrescente.
 *
 * Retorno:
 * - positivo : segundo score vem antes do primeiro
 * - negativo : primeiro score vem antes do segundo
 * - zero     : scores equivalentes
 *============================================================================*/
static int highscoreComparator(const void *a, const void *b)
{
    const Highscore *h1;
    const Highscore *h2;

    h1 = (const Highscore *)a;
    h2 = (const Highscore *)b;

    if (h2->score > h1->score) {
        return 1;
    }

    if (h2->score < h1->score) {
        return -1;
    }

    return 0;
}