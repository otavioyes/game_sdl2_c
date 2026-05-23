/*
 * bullet.c
 */

#include "common.h"

#include "bullet.h"
#include "player.h"
#include "stage.h"
#include "sound.h"
#include "draw.h"
#include "util.h"
#include "points.h"

extern Stage stage;


/*==============================================================================
 * Cria um novo projétil disparado pelo jogador.
 *
 * Responsabilidades:
 * - Alocar memória para o projétil
 * - Inserir projétil na lista de balas da fase
 * - Configurar propriedades iniciais do disparo
 * - Calcular direção baseada no ângulo da nave
 * - Posicionar projétil no centro do jogador
 * - Aplicar tempo de recarga do disparo
 *============================================================================*/
void fireBullet(SDL_Texture *texture)
{
    Entity *bullet;
    float angleRad;

    /* Aloca memória para a nova bala */
    bullet = malloc(sizeof(Entity));

    /* Verifica falha de alocação */
    if (bullet == NULL) {
        SDL_Log("malloc falhou em fireBullet");
        exit(1);
    }

    /* Inicializa estrutura com zero */
    memset(bullet, 0, sizeof(Entity));

    /* Adiciona bala na lista encadeada de projéteis */
    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    /* Configuração inicial do projétil */
    bullet->texture = texture;
    bullet->side = SIDE_PLAYER;
    bullet->type = ET_PLAYER_BULLET;
    bullet->health = 1;

    /* Obtém dimensões da textura */
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

    /*
     * SDL_RenderCopyEx utiliza graus para rotação,
     * porém funções trigonométricas utilizam radianos.
     */
    angleRad = player->angle * (M_PI / 180.0f);

    /*
     * Calcula velocidade horizontal e vertical
     * baseada no ângulo atual da nave.
     *
     * cosf() controla o eixo X.
     * sinf() controla o eixo Y.
     */
    bullet->dx = cosf(angleRad) * PLAYER_BULLET_SPEED;
    bullet->dy = sinf(angleRad) * PLAYER_BULLET_SPEED;

    /*
     * Posiciona o projétil no centro do jogador
     * para manter alinhamento visual do disparo.
     */
    bullet->x = player->x + (player->w / 2) - (bullet->w / 2);
    bullet->y = player->y + (player->h / 2) - (bullet->h / 2);

    /* Mantém rotação visual sincronizada com a nave */
    bullet->angle = player->angle;

    /* Aplica tempo de recarga entre disparos */
    player->reload = 8;
}


/*==============================================================================
 * Atualiza lógica dos projéteis ativos.
 *
 * Responsabilidades:
 * - Atualizar posição das balas
 * - Detectar colisões com entidades
 * - Remover projéteis fora da tela
 * - Liberar memória de projéteis destruídos
 * - Manter integridade da lista encadeada
 *============================================================================*/
void doBullet(SDL_Texture *pointsTexture)
{
    Entity *b;
    Entity *prev;

    /* Ponteiro auxiliar para manipulação segura da lista encadeada */
    prev = &stage.bulletHead;

    /* Percorre todos os projéteis ativos */
    for (b = stage.bulletHead.next; b != NULL; b = b->next) {

        /* Atualiza posição baseada na velocidade atual */
        b->x += b->dx;
        b->y += b->dy;

        /*
         * Remove projétil caso:
         * - atinja uma entidade
         * - saia dos limites visíveis da tela
         */
        if (bulletHitFighter(b, pointsTexture) ||
            b->x < -b->w ||
            b->y < -b->h ||
            b->x > SCREEN_WIDTH ||
            b->y > SCREEN_HEIGHT) {

            /*
             * Atualiza ponteiro final da lista
             * caso o projétil removido seja o último elemento.
             */
            if (b == stage.bulletTail) {
                stage.bulletTail = prev;
            }

            /* Remove projétil da lista encadeada */
            prev->next = b->next;

            /* Libera memória da entidade removida */
            free(b);

            /*
             * Retorna uma posição no loop para manter
             * iteração segura após remoção do elemento.
             */
            b = prev;
        }

        /* Avança ponteiro auxiliar */
        prev = b;
    }
}


/*==============================================================================
 * Renderiza todos os projéteis ativos da fase.
 *
 * Responsabilidades:
 * - Percorrer lista de projéteis
 * - Renderizar disparos do jogador com rotação
 * - Renderizar disparos inimigos sem rotação
 *============================================================================*/
void drawBullets(void)
{
    Entity *b;

    /* Percorre lista de projéteis ativos */
    for (b = stage.bulletHead.next; b != NULL; b = b->next) {

        /*
         * Projéteis do jogador utilizam rotação
         * baseada no ângulo do disparo.
         */
        if (b->side == SIDE_PLAYER) {
            blitRotated(b->texture, b->x, b->y, b->angle);
        } else {
            /* Projéteis inimigos utilizam renderização padrão */
            blit(b->texture, b->x, b->y, 0);
        }
    }
}


/*==============================================================================
 * Verifica e processa colisão entre um projétil e entidades da fase.
 *
 * Responsabilidades:
 * - Detectar colisão entre projétil e fighters
 * - Aplicar dano na entidade atingida
 * - Remover projétil após impacto
 * - Gerar efeito visual de impacto
 * - Gerar debris apenas quando a entidade for destruída
 * - Criar cápsula de pontuação apenas ao destruir inimigos
 * - Reproduzir efeitos sonoros de morte quando necessário
 *
 * Retorno:
 * - 1 : colisão detectada
 * - 0 : nenhuma colisão
 *============================================================================*/
int bulletHitFighter(Entity *b, SDL_Texture *pointsTexture)
{
    Entity *e;

    /* Percorre lista de entidades da fase */
    for (e = stage.fighterHead.next; e != NULL; e = e->next) {

        /*
         * Detecta colisão apenas entre entidades
         * de lados opostos.
         */
        if (e->side != b->side &&
            collision(b->x, b->y, b->w, b->h,
                      e->x, e->y, e->w, e->h)) {

            /* Marca projétil para remoção */
            b->health = 0;

            /*
             * Jogador recebe dano maior
             * em relação aos inimigos.
             */
            if (e == player) {
                e->health -= 10;

                /* Impede valores negativos de vida */
                if (e->health < 0) {
                    e->health = 0;
                }

            } else {
                /* Inimigos recebem dano padrão */
                e->health--;
            }

            /*
             * Cria efeito visual básico no ponto de impacto.
             * Este efeito ocorre em qualquer colisão.
             */
            addExplosions(e->x, e->y, 5);

            /*
             * Efeitos de destruição só acontecem quando
             * a entidade atingida fica sem vida.
             */
            if (e->health <= 0) {

                /* Gera fragmentos da entidade destruída */
                addDebris(e);

                if (e == player) {
                    /*
                     * Reproduz som associado à morte/dano final
                     * do jogador.
                     */
                    playerSound(SND_ALIEN_DIE, CH_PLAYER);

                } else {
                    /*
                     * Gera cápsula de pontos no centro
                     * da entidade inimiga destruída.
                     */
                    addPointsPod(e->x + e->w / 2,
                                 e->y + e->h / 2,
                                 pointsTexture);

                    /* Reproduz som de destruição do inimigo */
                    playerSound(SND_ALIEN_DIE, CH_ANY);
                }
            }

            /* Colisão detectada */
            return 1;
        }
    }

    /* Nenhuma colisão detectada */
    return 0;
}