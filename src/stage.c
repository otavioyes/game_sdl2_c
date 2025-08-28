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

// Inimigo revidando
static int stageResetTimer = 0;

//prototipo da colisao
static int bulletHitFighter(Entity *b);

//prototipo para inimigo disparando
static void resetStage(void);
static void doEnemies(void);
static void fireAlienBullet(Entity *e);
static void clipPlayer(void);

void initStage(void)
{
    app.delegate.logic = logic;
    app.delegate.draw  = draw;

    // Carrega texturas primeiro (uma só vez)
    bulletTexture     = loadTexture("assets/gfx/playerBullet.png");
    enemyTexture      = loadTexture("assets/gfx/enemy.png");
    alienBulletTexture= loadTexture("assets/gfx/alienBullet.png");
    playerTexture     = loadTexture("assets/gfx/player1.png");

    if (playerTexture == NULL)
    {
        SDL_Log("Erro ao carregar texture player1.png (initStage)");
        exit(1);
    }

    // Inicializa a estrutura do stage
    memset(&stage, 0, sizeof(stage));
    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail  = &stage.bulletHead;

    // Usa resetStage para criar o player (evita duplicidade)
    enemySpawnTimer = 0;
    resetStage();
}


/*Iinicia o player com localizacao e tamanho*/
static void initPlayer(void)
{
    player = malloc(sizeof(Entity));
    if (player == NULL)
    {
        SDL_Log("malloc falhou em initPlayer");
        exit(1);
    }
    memset(player, 0, sizeof(Entity));

    stage.fighterTail->next = player;
    stage.fighterTail = player;

    player->x       = 100;
    player->y       = 100;
    player->scale   = 0.1f;

    // Usa a textura já carregada globalmente
    if (playerTexture == NULL)
    {
        player->texture = loadTexture("assets/gfx/player1.png");
        playerTexture = player->texture;
    }
    else
    {
        player->texture = playerTexture;
    }

    if (player->texture == NULL)
    {
        SDL_Log("Erro ao carregar textura player1.png (initPlayer)");
        exit(1);
    }

    SDL_QueryTexture(player->texture, NULL, NULL, &player->width, &player->height);

    player->side = SIDE_PLAYER;
    player->health = 3;

    SDL_Log("Player inicializado em (%.1f, %.1f) size %dx%d", player->x, player->y, player->width, player->height);
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

    enemySpawnTimer =  60;

    stageResetTimer = FPS * 2;
}


// Gera inimigos periodicamente e adiciona à lista de lutadores
static void spawnsEnemies(void)
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
    doEnemies(); //inimigo dispara aqui
    doBullet();
    spawnsEnemies();

    clipPlayer();

    if (player == NULL && --stageResetTimer <= 0)
    {
        resetStage();
        printf("Player morto\n");
    }
}


static void doPlayer(void)
{
    if (player == NULL) return; // se não existe jogador, sai da função

    player->dx = player->dy = 0;

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
    if (player->x + player->width * player->scale > SCREEN_WIDTH)
    {
        player->x = SCREEN_WIDTH - player->width * player->scale;
    }
    if (player->x < 0) { player->x = 0; }
    if (player->y + player->height * player->scale > SCREEN_HEIGHT)
    {
        player->y = SCREEN_HEIGHT - player->height * player->scale;
    }
    if (player->y < 0) { player->y = 0; }
}

/*
    Essa função é muito importante!
    Ela faz aparecer o personagem!
*/
static void clipPlayer(void)
{
    if (player == NULL) return;

    // Usa largura/altura * scale para clipping consistente
    float w = player->width * player->scale;
    float h = player->height * player->scale;

    if (player->x < 0) player->x = 0;
    if (player->y < 0) player->y = 0;
    if (player->x > SCREEN_WIDTH  - w) player->x = SCREEN_WIDTH  - w;
    if (player->y > SCREEN_HEIGHT - h) player->y = SCREEN_HEIGHT - h;
}


static void doEnemies(void)
{
    Entity *e;

    for (e = stage.fighterHead.next ; e != NULL ; e = e->next)
    {
        if (e != player && player != NULL && --e->reload <= 0)
        {
            fireAlienBullet(e);
        }
    }
}


//Atualiza os lutadores e remove os que saíram da tela
static void doFighters(void)
{
    Entity *e = stage.fighterHead.next;
    Entity *prev = &stage.fighterHead; // começo no cabeçalho da lista de lutadores

    while (e != NULL)
    {
        // Atualiza posição com base na velocidade
        e->x += e->dx;
        e->y += e->dy;

        // Restringe o lutador ao limite inferior da tela (corrigido e usando altura)
        if (e->y + e->height * e->scale > SCREEN_HEIGHT)
        {
            e->y = SCREEN_HEIGHT - e->height * e->scale;
        }

        // Se o lutador não for o player e saiu da tela pela esquerda, marca vida como 0
        if (e != player && e->x < -e->width)
        {
            e->health = 0;
        }

        // Se a vida for 0, trata remoção e colisão
        if (e->health == 0)
        {
            if (e == player)
            {
                player = NULL;
            }

            if (player && e != player && checkCollisionEntities(e, player))
            {
                e->health = 0; // inimigo morre ao colidir com jogador
                // player->health -= 1; // opcional: reduzir vida do jogador
                SDL_Log("Inimigo colidiu com o jogador");
            }
        }

        // Restringe limite superior da tela
        if (e->y < 0)
        {
            e->y = 0;
        }

        // Condição para remover lutadores que saíram da tela à esquerda ou estão mortos
        if (e != player && ((e->x + e->width * e->scale) < 0 || e->health <= 0))
        {
            SDL_Log("Inimigo removido! Vida = %d", e->health);

            // Se for o último da lista, atualiza ponteiro do final
            if (e == stage.fighterTail)
            {
                stage.fighterTail = prev;
            }

            prev->next = e->next; // remove da lista

            Entity *toFree = e; // salva ponteiro para liberar depois
            e = e->next;        // avança antes de liberar

            free(toFree);       // libera memória

            continue;           // pula o avanço de prev para manter a lista consistente
        }

        // Avança ponteiros normalmente
        prev = e;
        e = e->next;
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

        if (bulletHitFighter(b) || b->x < -b->width ||
           b->y < -b->height || b->x > SCREEN_WIDTH ||
           b->y > SCREEN_HEIGHT )//Se a bala passou do limite direito da tela
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

    bullet->x = e->x;
    bullet->y = e->y;
    bullet->scale = 0.05f;
    bullet->health = 1;
    bullet->texture = alienBulletTexture;
    bullet->side = SIDE_ALIEN;
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->width, &bullet->height);

    bullet->x += (e->width / 2) - (bullet->width / 2) - (bullet->width * bullet->scale / 2);
    bullet->y += (e->height / 2) - (bullet->height / 2) - (bullet->height * bullet->scale / 2);

    calcSlop(player->x + (player->width / 2),
             player->y + (player->height / 2),
             e->x + (e->width * e->scale / 2),
             e->y + (e->height * e->scale / 2),
             &bullet->dx,
             &bullet->dy);

    bullet->dx *= ALIEN_BULLET_SPEED;
    bullet->dy *= ALIEN_BULLET_SPEED;

    e->reload = (rand() % FPS *2);
}
