/*
 * enemy.c
 */

#include "common.h"

#include "util.h"
#include "enemy.h"
#include "player.h"


extern Stage stage;



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
void spawnsEnemies(void) {//PASSAR A TEXTURA POR PARAMETRO
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



/*==============================================================================
 * Cria um projétil disparado por um inimigo.
 *
 * Responsabilidades:
 * - Validar possibilidade de disparo
 * - Alocar memória para o projétil
 * - Inserir projétil na lista de balas
 * - Configurar propriedades iniciais
 * - Calcular direção do disparo em relação ao jogador
 * - Aplicar velocidade do projétil
 * - Reiniciar temporizador de recarga do inimigo
 *============================================================================*/
void fireAlienBullet(Entity *e) {
    Entity *bullet;

    /* Impede disparo caso jogador esteja fora do ângulo permitido */
    if (!canAlienShootPlayer(e)) {
        return;
    }

    /* Aloca memória para o projétil inimigo */
    bullet = malloc(sizeof(Entity));

    /* Inicializa estrutura com zero */
    memset(bullet, 0, sizeof(Entity));

    /* Adiciona projétil na lista encadeada de balas */
    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    /* Define posição inicial do projétil */
    bullet->x = e->x;
    bullet->y = e->y;

    /* Configura propriedades básicas do projétil */
    bullet->health = 1;
    bullet->texture = alienBulletTexture;
    bullet->side = SIDE_ALIEN;

    /* Obtém dimensões da textura */
    SDL_QueryTexture(bullet->texture, NULL, NULL,
                     &bullet->w, &bullet->h);

    /*
     * Centraliza projétil em relação ao inimigo
     * para manter alinhamento visual do disparo.
     */
    bullet->x += (e->w / 2) - (bullet->w / 2);
    bullet->y += (e->h / 2) - (bullet->h / 2);

    /*
     * Calcula direção normalizada do disparo
     * em relação à posição atual do jogador.
     */
    calcSlop(player->x + (player->w / 2),
             player->y + (player->h / 2),
             e->x + (e->w / 2),
             e->y + (e->h / 2),
             &bullet->dx,
             &bullet->dy);

    /* Aplica velocidade final do projétil */
    bullet->dx *= ALIEN_BULLET_SPEED;
    bullet->dy *= ALIEN_BULLET_SPEED;

    /*
     * Reinicia temporizador de disparo do inimigo
     * com valor aleatório para variar frequência de ataque.
     */
    e->reload = rand() % (FPS * 2);
}



/*==============================================================================
 * Verifica se o inimigo possui ângulo válido para disparar no jogador.
 *
 * Responsabilidades:
 * - Calcular direção entre inimigo e jogador
 * - Normalizar vetor de direção
 * - Calcular alinhamento angular utilizando produto escalar
 * - Restringir disparo ao campo de visão frontal do inimigo
 *
 * Retorno:
 * - 1 : jogador está dentro do ângulo permitido
 * - 0 : jogador está fora do ângulo permitido
 *============================================================================*/
int canAlienShootPlayer(Entity *e) {
    float enemyCenterX;
    float enemyCenterY;

    float playerCenterX;
    float playerCenterY;

    float dx;
    float dy;

    float length;

    float dot;
    float cosAngle;

    /* Garante que o jogador exista antes do cálculo */
    if (player == NULL) {
        return 0;
    }

    /* Calcula centro do inimigo */
    enemyCenterX = e->x + (e->w / 2);
    enemyCenterY = e->y + (e->h / 2);

    /* Calcula centro do jogador */
    playerCenterX = player->x + (player->w / 2);
    playerCenterY = player->y + (player->h / 2);

    /*
     * Vetor direção:
     * inimigo -> jogador
     */
    dx = playerCenterX - enemyCenterX;
    dy = playerCenterY - enemyCenterY;

    /* Calcula comprimento do vetor */
    length = sqrtf(dx * dx + dy * dy);

    /*
     * Evita divisão por zero caso
     * inimigo e jogador estejam sobrepostos.
     */
    if (length == 0) {
        return 1;
    }

    /*
     * Vetor frontal do inimigo.
     *
     * Neste caso:
     * (-1, 0) = olhando para esquerda
     */
    dot = (dx * -1.0f) + (dy * 0.0f);

    /*
     * Produto escalar normalizado.
     *
     * Resultado representa o cosseno do ângulo
     * entre direção do inimigo e direção do jogador.
     */
    cosAngle = dot / length;

    /*
     * Permite disparo apenas dentro
     * do campo de visão frontal.
     *
     * Quanto menor o valor permitido,
     * maior o ângulo de disparo aceito.
     */
    return cosAngle >= -0.0f;
}