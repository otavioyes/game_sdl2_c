/*
 * structs.h
 *
 * Define as estruturas centrais utilizadas pela aplicação.
 *
 * Este arquivo representa a base de dados compartilhada
 * entre os principais sistemas do jogo:
 * - aplicação
 * - entidades
 * - fase
 * - efeitos visuais
 * - cache de texturas
 * - highscores
 */


/*==============================================================================
 * Forward declarations
 *
 * Permitem que ponteiros para estruturas sejam usados antes
 * da definição completa da struct.
 *============================================================================*/
typedef struct Entity Entity;
typedef struct Explosion Explosion;
typedef struct Debris Debris;
typedef struct Texture Texture;


/*==============================================================================
 * Delegate
 *
 * Armazena ponteiros para as funções principais do estado atual.
 *
 * Esse padrão permite alternar entre telas diferentes, como:
 * - title screen
 * - gameplay
 * - highscore
 *
 * Cada estado define sua própria função de lógica e renderização.
 *============================================================================*/
typedef struct
{
    void (*logic)(void);
    void (*draw)(void);

} Delegate;


/*==============================================================================
 * Texture
 *
 * Nó de lista encadeada utilizado para cache de texturas.
 *
 * Evita carregar a mesma imagem múltiplas vezes durante a execução.
 *============================================================================*/
struct Texture
{
    char        name[MAX_NAME_LENGTH];
    SDL_Texture *texture;
    Texture    *next;
};

/**/
typedef struct{
    int x;
    int y;
} Mouse;

/*==============================================================================
 * App
 *
 * Estrutura global da aplicação.
 *
 * Responsabilidades:
 * - Armazenar janela SDL
 * - Armazenar renderer SDL
 * - Controlar estado atual via delegate
 * - Guardar cache de texturas
 * - Guardar texto digitado no frame atual
 * - Guardar estado das teclas
 *============================================================================*/
typedef struct
{
    SDL_Renderer *renderer;
    SDL_Window   *window;

    Delegate     delegate;

    Texture      textureHead;
    Texture      *textureTail;

    char         inputText[MAX_LINE_LENGTH];

    /*
     * Array de estado do teclado.
     *
     * Cada índice representa um SDL_Scancode.
     * Valor 1 = tecla pressionada.
     * Valor 0 = tecla solta.
     */
    int          keyboard[MAX_KEYBOARD_KEYS];
    Mouse        mouse;

} App;


/*==============================================================================
 * Entity
 *
 * Estrutura base reutilizada por múltiplos sistemas de gameplay.
 *
 * Dependendo do campo "type", uma Entity pode representar:
 * - jogador
 * - inimigo
 * - projétil
 * - cápsula de pontos
 *
 * O campo "side" define a facção da entidade, permitindo lógica
 * de colisão entre lados opostos.
 *============================================================================*/
struct Entity
{
    /* Posição no mundo/tela */
    float       x;
    float       y;

    /* Velocidade aplicada por frame */
    float       dx;
    float       dy;

    /* Rotação em graus */
    float       angle;

    /* Dimensões do sprite */
    int         w;
    int         h;

    /* Estado de gameplay */
    int         health;
    int         reload;

    /* Identificação lógica */
    int         type;
    int         side;

    /* Textura usada na renderização */
    SDL_Texture *texture;

    /* Próxima entidade da lista encadeada */
    Entity      *next;
};


/*==============================================================================
 * Explosion
 *
 * Partícula visual usada para efeitos de explosão/impacto.
 *
 * Cada explosão possui:
 * - posição
 * - velocidade
 * - cor
 * - alpha
 *
 * O alpha também é usado como tempo de vida visual.
 *============================================================================*/
struct Explosion
{
    /* Posição */
    float      x;
    float      y;

    /* Velocidade */
    float      dx;
    float      dy;

    /* Cor e transparência */
    int        r;
    int        g;
    int        b;
    int        a;

    /* Próxima explosão da lista encadeada */
    Explosion *next;
};


/*==============================================================================
 * Debris
 *
 * Fragmento visual gerado quando uma entidade é destruída.
 *
 * Usa uma região da textura original para simular pedaços
 * do sprite quebrando após a explosão.
 *============================================================================*/
struct Debris
{
    /* Posição */
    float       x;
    float       y;

    /* Velocidade */
    float       dx;
    float       dy;

    /* Região da textura original usada pelo fragmento */
    SDL_Rect    rect;

    /* Textura original da entidade destruída */
    SDL_Texture *texture;

    /* Tempo de vida em frames */
    int         life;

    /* Próximo debris da lista encadeada */
    Debris      *next;
};


/*==============================================================================
 * Stage
 *
 * Estrutura principal da fase.
 *
 * Responsabilidades:
 * - Armazenar listas de entidades ativas
 * - Armazenar listas de efeitos visuais
 * - Controlar pontuação atual
 *
 * Cada lista usa o padrão head/tail para inserção eficiente no final.
 *============================================================================*/
typedef struct
{
    /* Lista unificada de entidades - migracao futura*/
    Entity    entityHead;
    Entity    *entityTail;

    /* Lista de jogador e inimigos */
    Entity    fighterHead;
    Entity    *fighterTail;

    /* Lista de projéteis */
    Entity    bulletHead;
    Entity    *bulletTail;

    /* Lista de cápsulas de pontos */
    Entity    pointsHead;
    Entity    *pointsTail;

    /* Lista de partículas de explosão */
    Explosion explosionHead;
    Explosion *explosionTail;

    /* Lista de fragmentos visuais */
    Debris    debrisHead;
    Debris    *debrisTail;

    /* Pontuação atual da fase */
    int       score;


} Stage;


/*==============================================================================
 * Star
 *
 * Representa uma estrela do starfield.
 *
 * Utilizada para criar sensação de profundidade e movimento
 * no background espacial.
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
 * Representa uma pontuação individual da tabela.
 *
 * O campo "recent" indica se esse score foi registrado
 * na partida atual, permitindo destaque visual na tela.
 *============================================================================*/
typedef struct
{
    int  score;
    int  recent;
    char name[MAX_SCORE_NAME_LENGTH];

} Highscore;


/*==============================================================================
 * Highscores
 *
 * Tabela fixa contendo os melhores scores do jogo.
 *============================================================================*/
typedef struct
{
    Highscore highscore[NUM_HIGHSCORES];

} Highscores;