/*
 * player.c
 */

#include "common.h"

#include "player.h"
#include "sound.h"
#include "bullet.h"

extern Stage stage;
extern App app;

/*
 * Ponteiro global da entidade do jogador.
 *
 * O player é tratado como uma entidade persistente da fase,
 * permitindo acesso simplificado pelos demais módulos.
 */
Entity *player;


/*==============================================================================
 * Inicializa a entidade do jogador.
 *
 * Responsabilidades:
 * - Alocar memória para o player
 * - Inserir o player na lista de entidades da fase
 * - Configurar atributos iniciais
 * - Associar textura do jogador
 * - Obter dimensões do sprite
 * - Definir tipo e lado da entidade
 *============================================================================*/
void initPlayer(SDL_Texture *texture)
{
    /* Aloca memória para a entidade do jogador */
    player = malloc(sizeof(Entity));

    /* Valida falha de alocação */
    if (player == NULL) {
        SDL_Log("malloc falhou em initPlayer");
        exit(1);
    }

    /* Inicializa toda a estrutura com zero */
    memset(player, 0, sizeof(Entity));

    /*
     * Insere o jogador no final da lista encadeada
     * de fighters da fase.
     */
    stage.fighterTail->next = player;
    stage.fighterTail = player;

    /* Configuração inicial do jogador */
    player->health = PLAYER_MAX_HEALTH;
    player->x = 100;
    player->y = 100;

    /* Associa textura do jogador */
    player->texture = texture;

    /*
     * Obtém largura e altura do sprite
     * diretamente da textura carregada.
     */
    SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);

    /* Define lado da entidade */
    player->side = SIDE_PLAYER;

    /* Define tipo da entidade */
    player->type = ET_PLAYER;
}


/*==============================================================================
 * Atualiza a lógica do jogador.
 *
 * Responsabilidades:
 * - Processar entrada do teclado
 * - Controlar movimentação
 * - Controlar rotação da nave
 * - Atualizar recarga do disparo
 * - Disparar projéteis
 * - Normalizar ângulo da nave
 *============================================================================*/
void doPlayer(SDL_Texture *bulletTexture)
{
    /*
     * Garante que a entidade do jogador exista
     * antes de processar qualquer lógica.
     */
    if (player == NULL) {
        return;
    }

    /*
     * Reinicia velocidade do jogador a cada frame.
     *
     * A movimentação é recalculada continuamente
     * com base no input atual.
     */
    player->dx = 0;
    player->dy = 0;

    /* Atualiza temporizador de recarga do disparo */
    if (player->reload > 0) {
        player->reload--;
    }

    /* Movimento horizontal para esquerda */
    if (app.keyboard[SDL_SCANCODE_A] || app.keyboard[SDL_SCANCODE_LEFT]) {
        player->dx = -PLAYER_SPEED;
    }

    /* Movimento horizontal para direita */
    if (app.keyboard[SDL_SCANCODE_D] || app.keyboard[SDL_SCANCODE_RIGHT]) {
        player->dx = PLAYER_SPEED;
    }

    /* Movimento vertical para cima */
    if (app.keyboard[SDL_SCANCODE_W] || app.keyboard[SDL_SCANCODE_UP]) {
        player->dy = -PLAYER_SPEED;
    }

    /* Movimento vertical para baixo */
    if (app.keyboard[SDL_SCANCODE_S] || app.keyboard[SDL_SCANCODE_DOWN]) {
        player->dy = PLAYER_SPEED;
    }

    /*
     * Dispara projéteis apenas quando o tempo
     * de recarga estiver disponível.
     */
    if (app.keyboard[SDL_SCANCODE_SPACE] && player->reload <= 0) {

        /* Reproduz efeito sonoro do disparo */
        playerSound(SND_PLAYER_FIRE, CH_PLAYER);

        /* Cria projétil do jogador */
        fireBullet(bulletTexture);
    }

    /* Rotaciona nave para esquerda */
    if (app.keyboard[SDL_SCANCODE_Q]) {
        player->angle -= 4;
    }

    /* Rotaciona nave para direita */
    if (app.keyboard[SDL_SCANCODE_E]) {
        player->angle += 4;
    }

    /*
     * Mantém o ângulo da nave dentro do intervalo
     * de 0 a 359 graus.
     *
     * Isso evita crescimento infinito do valor
     * e possíveis problemas futuros de precisão.
     */
    if (player->angle >= 360) {
        player->angle -= 360;
    }

    if (player->angle < 0) {
        player->angle += 360;
    }
}


/*==============================================================================
 * Mantém o jogador dentro dos limites visíveis da tela.
 *
 * Responsabilidades:
 * - Validar existência da entidade do jogador
 * - Restringir movimentação horizontal
 * - Restringir movimentação vertical
 * - Garantir que o sprite permaneça totalmente visível
 *============================================================================*/
void clipPlayer(void)
{
    /* Garante que o jogador exista */
    if (player == NULL) {
        return;
    }

    /* Limite esquerdo da tela */
    if (player->x < 0) {
        player->x = 0;
    }

    /* Limite superior da tela */
    if (player->y < 0) {
        player->y = 0;
    }

    /*
     * Limite direito da tela.
     *
     * Subtrai a largura do sprite para impedir
     * que o jogador ultrapasse parcialmente
     * a área visível.
     */
    if (player->x > SCREEN_WIDTH - player->w) {
        player->x = SCREEN_WIDTH - player->w;
    }

    /*
     * Limite inferior da tela.
     *
     * Subtrai a altura do sprite para manter
     * o jogador completamente dentro da tela.
     */
    if (player->y > SCREEN_HEIGHT - player->h) {
        player->y = SCREEN_HEIGHT - player->h;
    }
}


/*==============================================================================
 * Desenha a barra de vida segmentada do jogador.
 *
 * Responsabilidades:
 * - Validar existência do jogador
 * - Converter vida total em segmentos visuais
 * - Renderizar fundo da barra
 * - Renderizar segmentos preenchidos e vazios
 * - Renderizar bordas individuais
 *============================================================================*/
void drawPlayerHealthBar(void)
{
    SDL_Rect bg;
    SDL_Rect segment;

    int i;

    /* Quantidade máxima de segmentos visuais */
    int maxLives = 10;

    /* Quantidade atual de segmentos preenchidos */
    int currentLives;

    /* Dimensões de cada segmento */
    int segmentWidth = 25;
    int segmentHeight = 20;

    /* Espaçamento entre segmentos */
    int gap = 4;

    /* Garante que o jogador exista */
    if (player == NULL) {
        return;
    }

    /*
     * Impede que valores negativos de vida
     * afetem a renderização da HUD.
     */
    if (player->health < 0) {
        player->health = 0;
    }

    /*
     * Converte vida total em quantidade
     * de segmentos ativos.
     */
    currentLives = player->health / 10;

    /* Configuração do retângulo de fundo da barra */
    bg.x = 10;
    bg.y = 50;
    bg.w = (segmentWidth * maxLives) + (gap * (maxLives - 1));
    bg.h = segmentHeight;

    /* Renderiza fundo geral da barra */
    SDL_SetRenderDrawColor(app.renderer, 40, 40, 40, 255);
    SDL_RenderFillRect(app.renderer, &bg);

    /* Renderiza segmentos individuais da barra */
    for (i = 0; i < maxLives; i++) {

        /* Calcula posição do segmento atual */
        segment.x = bg.x + (i * (segmentWidth + gap));
        segment.y = bg.y;
        segment.w = segmentWidth;
        segment.h = segmentHeight;

        /*
         * Segmentos preenchidos representam
         * vida disponível.
         */
        if (i < currentLives) {

            SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);
            SDL_RenderFillRect(app.renderer, &segment);

        } else {

            /*
             * Segmentos vazios representam
             * vida perdida.
             */
            SDL_SetRenderDrawColor(app.renderer, 80, 80, 80, 255);
            SDL_RenderFillRect(app.renderer, &segment);
        }

        /* Renderiza borda visual do segmento */
        SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(app.renderer, &segment);
    }
}