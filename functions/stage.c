//stage.c
#include "common.h"

//PROXIMO ARQUIVO A SER CRIADO -> util.c

// protótipos internos
static void initPlayer(void);
static void logic(void);
static void doPlayer(void);
static void doBullet(void);
static void fireBullet(void);

//novos prototipos sendo declarados para a criação de inimigos
static void doFighters(void);
static void spawnsEnemies(void);

static int enemySpawnTimer;

//prototipo da colisao
static int bulletHitFighter(Entity *b);

//prototipo para inimigo disparando
static void resetStage(void);
static void doEnemies(void);
static void fireAlienBullet(Entity *e);
static void clipPlayer(void);
static void resetStage(void);

//Inicializa o estágio (fase) do jogo
void initStage(void)
{
    app.delegate.logic = logic;// Define qual função será chamada para atualizar a lógica do jogo
    app.delegate.draw  = draw;// Define qual função será chamada para desenhar na tela

    memset(&stage, 0, sizeof(stage));//Zera toda a estrutura do estagio (Limpa memoria)

    // Inicializa os ponteiros das listas ligadas:
    // Início e final da lista de lutadores (jogador e inimigos)
    stage.fighterTail = &stage.fighterHead;

    stage.bulletTail  = &stage.bulletHead;// Início e final da lista de balas

    initPlayer();//Cria e posiciona o jogador no cenario

    bulletTexture = loadTexture("gfx/playerBullet.png");//Carrega textura da bala
    enemyTexture = loadTexture("gfx/enemy.png");//carrega texrtura do inimigo
    enemySpawnTimer = 0; //Zera o temporizador de geraçao de inimigos

    // Inimigos revidando
    alienBulletTexture = loadTexture("gfx/alienBullet.png");
    playerTexture = loadTexture("gfx/player.png");

    resetStage();

}

/*Iinicia o player com localizacao e tamanho*/
static void initPlayer(void)
{
    player = malloc(sizeof(Entity));
    memset(player, 0, sizeof(Entity));//inicializando os campos da strut com 0 S/lixo de mem
    stage.fighterTail->next = player;
    stage.fighterTail = player;

    player->x       = 100; //posicao X onde o player nasce
    player->y       = 100; //posicao Y onde o player nasce
    player->scale   = 0.1f;//tamanho da imagem /scale
    player->texture = loadTexture("gfx/player1.png");

    SDL_QueryTexture(player->texture, NULL, NULL, &player->width, &player->height);

    player->side = SIDE_PLAYER;
}

static void resetStage(void)
{
    Entity *e;

    while (stage.fighterHead.next)
    {
        e = stage.fighterHead.next;
        stage.fighterHead.next = e->next;
        free(e);
    }

    while (stage.bulletHead.next)
    {
        e = stage.bulletHead.next;
        stage.bulletHead.next = e->next;
        free(e);
    }

    memset(&stage, 0, sizeof(Stage));
    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail = &stage.bulletHead;

    initPlayer();

    enemySpawnTimer =  0;

    stageResetTimer = FPS * 2;
}


// Gera inimigos periodicamente e adiciona à lista de lutadores
static void spawnEnemies(void)
{
    Entity *enemy;

    // Decrementa o temporizador e verifica se é hora de criar um inimigo
    if(--enemySpawnTimer <=0)
    {
        enemy = malloc(sizeof(Entity)); //aloca memoria para um novo inimigo
        memset(enemy, 0, sizeof(Entity)); //zera os dados da estrutura

        // Adiciona o inimigo ao final da lista ligada de lutadores
        stage.fighterTail->next = enemy;
        stage.fighterTail = enemy;

        enemy->scale = 0.1f; // define o tamnho visual do inimigo(scale)

        enemy->x = SCREEN_WIDTH; //Comeca fora da tela a direita
        enemy->y = rand() % SCREEN_HEIGHT;//coordenada Y e escolhida aleatoriamente com base em SCREEN_HEIGHT
        enemy->texture = enemyTexture; // usa textura do inimigo
        SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->width, &enemy->height); //define largura e altura original

        enemy->dx = -(2 + (rand() % 4));//velocidade para a esquerda negativa
        enemy->dy = rand() %5 - 2; //Movimentacao do eixo Y eleatoria
        enemySpawnTimer = 20  + (rand() % 60); //taxa de geracao do inimigo
        enemy->reload = FPS * (1 + (rand() % 3));

        enemy->side = SIDE_ALIEN;

        enemy->health = 3;
    }
}

//Atualiza a logica do jogo a cada frame
static void logic(void)
{
    doPlayer();
    doFighters();
    doBullet();
    spawnEnemies();

    clipPlayer();

    if (player == NULL && --stageResetTimer <= 0)
    {
        resetStage();
        printf("Player morto");
    }
}

//funcao de  movimentacao do jogador
static void doPlayer(void)
{

    if (player != NULL)
    {
        player->dx = player->dy = 0;
    }

    if (player->reload > 0)
    {
        player->reload--;
    }

    if (app.keyboard[SDL_SCANCODE_W]) { player->dy = -PLAYER_SPEED; }

    if (app.keyboard[SDL_SCANCODE_S]) { player->dy = PLAYER_SPEED; }

    if (app.keyboard[SDL_SCANCODE_A]) { player->dx = -PLAYER_SPEED; }

    if (app.keyboard[SDL_SCANCODE_D]) { player->dx = PLAYER_SPEED; }

    if (app.mouse[SDL_BUTTON(SDL_BUTTON_LEFT)] && player->reload == 0) { fireBullet(); }


    // restringindo o jogador aos limites da tela
    if(player->x + player->width * player->scale > SCREEN_WIDTH)
    {
        player->x = SCREEN_WIDTH - player->width * player->scale;
    }

    if(player->x < 0) { player->x = 0; }

    if(player->y + player->height * player->scale > SCREEN_HEIGHT)
    {
        player->y = SCREEN_HEIGHT - player->height * player->scale;
    }

    if(player->y < 0) { player->y = 0; }
}

static void clipPlayer(void)
{
    if (player != NULL)
    {
        if (player->x < 0)
        {
            player->x = 0;
        }

        if (player->y < 0)
        {
            player->y = 0;
        }

        if (player->x > SCREEN_WIDTH / 2)
        {
            player->x = SCREEN_WIDTH /2;
        }

        if (player->y > SCREEN_HEIGHT / 2)
        {
            player->y = SCREEN_HEIGHT /2;
        }

    }
}


static void doEnemies(void)
{
    Entity *e;

    for (e = stage.fighterHead.next ; e != NULL ; e = e->next);
    {
        if (e != player && player != NULL && --e->reload <= 0)
        {
            fireAlienBullet(e);
        }
    }
}


// Atualiza os lutadores e remove os que saíram da tela
static void doFighters(void)
{
    Entity *e, *prev;

    prev = &stage.fighterHead;//comeca no cabecalho da lista de lutadores


    for (e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        e->x += e->dx;//atualiza posicao x com base na velocidade
        e->y += e->dy;//atualiza posica y com base na velocidade

        if(e->y + e->height * e->scale > SCREEN_HEIGHT)
        {
            e->y = SCREEN_HEIGHT - e->width * e->scale;
        }

        if (e != player && e->x < -e->width)
        {
            e->health = 0;
        }

        if (e->health == 0)
        {
            if (e == player)
            {
                player = NULL;
            }

            if(e != player && collision(
                                (int)(e->x), (int)(e->y),
                                (int)(e->width  *   e->scale),
                                (int)(e->height *   e->scale),
                                /*jogador*/
                                (int)(player->x), (int)(player->y),
                                (int)(player->width     *   player->scale),
                                (int)(player->height    *   player->scale)))
                                {
                                e->health = 0;//inimigo morre 100% ao encontar no jogador
                               // player->health -= 1;// reduz a vida do jogador
                                SDL_Log("Inimigo colidiu com o jogador"); // Log de colisao
                                }

        if(e->y < 0)
        {
            e->y = 0;
        }

        // Se não for o jogador e o lutador saiu completamente da tela à esquerda
        if(e != player && ( (e->x + e->width * e->scale) < 0 || e->health <= 0) )// Exclui um lutador se sua 'health'  for 0
        {
            SDL_Log("Inimigo removido! Vida = %d", e->health);

            // Se for o último da lista, atualiza o ponteiro do final
            if(e == stage.fighterTail)
            {
                stage.fighterTail = prev;
            }

            prev->next = e->next; //Remove da lista
            free(e); // Libera da memória
            e = prev; //Volta um passo para manter o loop consistente
        }

        prev = e; // Avança o ponteiro anterior
    }
}

// Função que testa se a bala `b` atingiu algum inimigo
static int bulletHitFighter(Entity *b)
{
    Entity *e; // Ponteiro para percorrer a lista de fighters

    // Para cada fighter na lista encadeada de inimigos…
    for(e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        if(e->side != b->side && collision(
                                          (int) (b->x),
                                          (int) (b->y),
                                          (int) (b->width   *   b->scale),
                                          (int) (b->height  *   b->scale), // retângulo da bala
                                          /*inimigo*/
                                          (int) (e->x),
                                          (int) (e->y),
                                          (int) (e->width   *   e->scale),
                                          (int) (e->height  *   e->scale))) // retângulo do fighter
        {
            b->health = 0; // Marca a bala para a remocao
            e->health -= 1; // Marca o inimigo para a remocao
            return 1; // Marca um "hit" (colisao bem sucedida)
        }
    }
    return 0; //Nenhua colisao
}


//Atualiza as balas e remove as que sairam da tela (lado direito)
static void doBullet(void)
{
    Entity *b, *prev;

    prev = &stage.bulletHead; // Ponteiro anterior, começa no cabeçalho da lista de balas

    //Percorre a lista de balas
    for (b = stage.bulletHead.next ; b != NULL ; b = b->next)
    {
        b->x    += b->dx; //atualiza a posicao X da bala
        b->y    += b->dy; //atualiza a posicao y da bala

        if (bulletHitFighter(b) || b->x < -b->width || b->y < -b->height || b->x > SCREEN_WIDTH || b->y > SCREEN_HEIGHT )//Se a bala passou do limite direito da tela
        {
            //Se a bala era ultima da lista, atualiza o ponteiro
            if (b == stage.bulletTail)
            {
                stage.bulletTail = prev;
            }

            prev->next = b->next; //Remove a bala da lista
            free(b); //Libera a memoria da bala
            b = prev; //Volta a uma posição da lista para manter o loop seguro
        }

        prev = b;//Avanca o ponteiro anterior
    }
}


// Função que cria uma nova bala (disparo do jogador)
static void fireBullet(void)
{
    Entity *bullet;

    // Aloca memória para a bala e zera seus dados
    bullet = malloc(sizeof(Entity));
    memset(bullet, 0, sizeof(Entity));

    // Adiciona a bala ao final da lista de balas
    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    bullet->scale   = 0.1f; // Define o tamanho (escala) da bala

    bullet->texture = bulletTexture; // Usa a textura da bala

    // Obtém as dimensões originais da textura da bala
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->width, &bullet->height);

    //Define a posição inicial da bala no centro do jogador
    bullet->x   =   player->x + (player->width * player->scale / 2) - (bullet->width * bullet->scale / 2);
    bullet->y   =   player->y + (player->height * player->scale / 2) - (bullet->height * bullet->scale / 2);

    bullet->dx = PLAYER_BULLET_SPEED; //Velocidade da bala no eixo Y
    bullet->health = 1; //A bala esta ativa (vida util)
    player->reload = 8; //Tempo de recarga antes de atirar novamente.

    bullet->side = SIDE_PLAYER;

}

static void fireAlienBullet(Entity *e)
{
    Entity *bullet;

    bullet = malloc(sizeof(Entity));
    memset(bullet, 0, sizeof(Entity));
    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    bullet->x =  e->x;
    bullet->y = e->y;
    bullet->scale = 0.1f;
    bullet->health = 1;
    bullet->Texture = alienBulletTexture;
    bullet->side = SIDE_ALIEN;
    SQL_QueryTexture(bullet->texture, NULL, NULL, &bullet->width, &bullet->height);

    bullet->x += (e->width / 2) - (bullet->width / 2) - (bullet->width * bullet->scale / 2);
    bullet->y += (e->hight / 2) - (bullet->height / 2) - (bullet->height * bullet->scale / 2);

    calcSlop(player->x + (player->width / 2), player->y + (player->height / 2),
             e->x, e->y, &bullet->dx, &bullet->dy);

    bullet->dx *= ALIEN_BULLET_SPEED;
    bullet->dy *= ALIEN_BULLET_SPEED;

    e->reload = (rand() % FPS *2);
}





