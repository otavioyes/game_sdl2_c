/*
 * structs.h
 *
 * Estruturas principais utilizadas pelo jogo.
 *
 * Responsabilidades:
 * - Definir entidades do gameplay
 * - Definir estruturas de renderização
 * - Definir gerenciamento de memória/listas
 * - Centralizar dados compartilhados do jogo
 */


/*==============================================================================
 * Forward declarations
 *============================================================================*/

/* Estrutura base de entidades do jogo */
typedef struct Entity Entity;

/* Estrutura de partículas de explosão */
typedef struct Explosion Explosion;

/* Estrutura de fragmentos visuais */
typedef struct Debris Debris;

/* Estrutura de cache de texturas */
typedef struct Texture Texture;



/*==============================================================================
 * Delegate
 *
 * Responsável por armazenar ponteiros de funções
 * do loop principal da aplicação.
 *============================================================================*/
typedef struct
{
    void (*logic)(void);
    void (*draw)(void);

} Delegate;



/*==============================================================================
 * Texture
 *
 * Estrutura utilizada para cache de texturas carregadas.
 *============================================================================*/
struct Texture
{
    char            name[MAX_NAME_LENGTH];
    SDL_Texture     *texture;
    Texture         *next;
};



/*==============================================================================
 * App
 *
 * Estrutura principal da aplicação.
 *
 * Responsabilidades:
 * - Gerenciar janela e renderer SDL
 * - Controlar input do teclado
 * - Armazenar delegates do game loop
 * - Gerenciar cache de texturas
 *============================================================================*/
typedef struct
{
    SDL_Renderer    *renderer;
    SDL_Window      *window;
    Delegate        delegate;
    int             keyboard[MAX_KEYBOARD_KEYS];
    Texture         textureHead;
    Texture         *textureTail;
    char            inputText[MAX_LINE_LENGTH];
} App;



/*==============================================================================
 * Entity
 *
 * Estrutura base de entidades do jogo.
 *
 * Utilizada por:
 * - jogador
 * - inimigos
 * - projéteis
 * - cápsulas de pontos
 *============================================================================*/
struct Entity
{
    /* Posição */
    float       x;
    float       y;

    /* Dimensões */
    int         w;
    int         h;

    /* Velocidade */
    float       dx;
    float       dy;

    /* Estado */
    int         health;
    int         reload;

    /* Identificação */
    int         type;
    int         side;

    /* Rotação */
    float       angle;

    /* Renderização */
    SDL_Texture *texture;

    /* Próxima entidade da lista */
    Entity      *next;
};



/*==============================================================================
 * Explosion
 *
 * Estrutura de partículas de explosão.
 *============================================================================*/
struct Explosion
{
    /* Posição */
    float       x;
    float       y;

    /* Velocidade */
    float       dx;
    float       dy;

    /* Cor e transparência */
    int         r;
    int         g;
    int         b;
    int         a;

    /* Próxima explosão da lista */
    Explosion   *next;
};



/*==============================================================================
 * Debris
 *
 * Estrutura de fragmentos gerados após destruição
 * de entidades.
 *============================================================================*/
struct Debris
{
    /* Posição */
    float       x;
    float       y;

    /* Velocidade */
    float       dx;
    float       dy;

    /* Região da textura original */
    SDL_Rect    rect;

    /* Textura do fragmento */
    SDL_Texture *texture;

    /* Tempo de vida */
    int         life;

    /* Próximo debris da lista */
    Debris      *next;
};



/*==============================================================================
 * Stage
 *
 * Estrutura principal da fase.
 *
 * Responsabilidades:
 * - Gerenciar entidades ativas
 * - Gerenciar listas encadeadas
 * - Armazenar pontuação da fase
 *============================================================================*/
typedef struct
{
    /* Fighters */
    Entity      fighterHead;
    Entity      *fighterTail;

    /* Projéteis */
    Entity      bulletHead;
    Entity      *bulletTail;

    /* Cápsulas de pontos */
    Entity      pointsHead;
    Entity      *pointsTail;

    /* Explosões */
    Explosion   explosionHead;
    Explosion   *explosionTail;

    /* Debris */
    Debris      debrisHead;
    Debris      *debrisTail;

    /* Pontuação atual */
    int         score;

} Stage;



/*==============================================================================
 * Star
 *
 * Estrutura utilizada no sistema de starfield.
 *============================================================================*/
typedef struct
{
    int x;
    int y;
    int speed;
} Star;



/*==============================================================================
 * Highscore
 *
 * Estrutura de pontuação individual.
 *============================================================================*/
typedef struct
{
    char name[MAX_SCORE_NAME_LENGTH];
    int recent;
    int score;
} Highscore;



/*==============================================================================
 * Highscores
 *
 * Estrutura da tabela de highscores.
 *============================================================================*/
typedef struct
{
    Highscore highscore[NUM_HIGHSCORES];
} Highscores;