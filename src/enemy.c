/*
 * enemy.c
 */

#include "common.h"




void spawnsEnemies(void);
void doEnemies(void){



/*==============================================================================
 * Gera inimigos periodicamente durante a fase.
 *
 * Responsabilidades:
 * - Controlar temporizador de spawn
 * - Alocar memória para novos inimigos
 * - Inserir inimigos na lista de fighters
 * - Configurar atributos iniciais do inimigo
 * - Definir movimentação e tempo de disparo
 *============================================================================*/
void spawnsEnemies(void)
{
    Entity *enemy;

    /* Atualiza temporizador de geração de inimigos */
    if (--enemySpawnTimer <= 0) {

        /* Aloca memória para nova entidade inimiga */
        enemy = malloc(sizeof(Entity));

        /* Inicializa estrutura com zero */
        memset(enemy, 0, sizeof(Entity));

        /* Adiciona inimigo na lista encadeada de fighters */
        stage.fighterTail->next = enemy;
        stage.fighterTail = enemy;

        /* Define posição inicial fora da tela, no lado direito */
        enemy->x = SCREEN_WIDTH;
        enemy->y = rand() % SCREEN_HEIGHT;

        /* Configura textura do inimigo */
        enemy->texture = enemyTexture;

        /* Obtém dimensões da textura */
        SDL_QueryTexture(enemy->texture, NULL, NULL,
                         &enemy->w, &enemy->h);

        /*
         * Velocidade horizontal negativa
         * faz o inimigo se mover para esquerda.
         */
        enemy->dx = -(2 + (rand() % 4));

        /*
         * Pequena variação vertical aleatória
         * para criar movimentação menos previsível.
         */
        enemy->dy = -100 + rand() % 200;
        enemy->dy /= 100;

        /* Define facção da entidade */
        enemy->side = SIDE_ALIEN;

        /* Vida inicial do inimigo */
        enemy->health = 1;

        /*
         * Temporizador inicial de disparo.
         * Cria intervalo aleatório entre tiros.
         */
        enemy->reload = FPS * (1 + (rand() % 3));

        /*
         * Define novo tempo para próximo spawn.
         * Mantém frequência variável de inimigos.
         */
        enemySpawnTimer = 30 + (rand() % FPS);
    }
}



/*==============================================================================
 * Atualiza lógica dos inimigos ativos da fase.
 *
 * Responsabilidades:
 * - Atualizar comportamento dos inimigos
 * - Restringir movimentação vertical na tela
 * - Controlar temporizador de disparo
 * - Executar disparos inimigos
 * - Reproduzir efeitos sonoros de ataque
 *============================================================================*/
    void doEnemies(void) {
        Entity *e;

        /* Percorre lista de fighters ativos */
        for (e = stage.fighterHead.next; e != NULL; e = e->next) {

            /* Ignora entidade do jogador */
            if (e != player) {

                /*
                 * Mantém inimigo dentro dos limites verticais da tela.
                 */
                e->y = MIN(MAX(e->y, 0),
                           SCREEN_HEIGHT - e->h);

                /*
                 * Executa disparo apenas se:
                 * - jogador existir
                 * - temporizador de recarga chegar a zero
                 */
                if (player != NULL && --e->reload <= 0) {

                    /* Cria projétil inimigo */
                    fireAlienBullet(e);

                    /* Reproduz efeito sonoro do disparo */
                    playerSound(SND_PLAYER_FIRE, CH_ALIEN_FIRE);
                }
            }
        }
    }
