/*
 * defs.h
 *
 * Arquivo central de definições globais do projeto.
 *
 * Responsabilidades:
 * - Definir constantes globais
 * - Configurar limites do sistema
 * - Padronizar valores de gameplay
 * - Declarar enums compartilhados
 * - Fornecer macros utilitárias
 *
 * Observação:
 * - Este arquivo deve conter apenas definições globais,
 *   evitando dependências desnecessárias entre módulos.
 */


/*==============================================================================
 * Macros utilitárias
 *============================================================================*/

/*
 * Retorna o menor valor entre dois elementos.
 */
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

/*
 * Retorna o maior valor entre dois elementos.
 */
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

/*
 * Cópia segura de string.
 *
 * Garante terminação nula para evitar
 * problemas de buffer overflow.
 */
#define STRNCPY(dest, src, n)         \
    strncpy(dest, src, n);            \
    dest[(n) - 1] = '\0';


/*==============================================================================
 * Configurações de resolução
 *============================================================================*/

/* Largura da janela */
#define SCREEN_WIDTH 1280

/* Altura da janela */
#define SCREEN_HEIGHT 720


/*==============================================================================
 * Configurações gerais do jogo
 *============================================================================*/

/* Frames por segundo */
#define FPS 60


/*==============================================================================
 * Configurações de entrada
 *============================================================================*/

/*
 * Quantidade máxima de teclas monitoradas.
 *
 * SDL_Scancode utiliza índices inteiros.
 */
#define MAX_KEYBOARD_KEYS 350


/*==============================================================================
 * Configurações de movimentação
 *============================================================================*/

/* Velocidade da nave do jogador */
#define PLAYER_SPEED 4

/* Velocidade dos projéteis do jogador */
#define PLAYER_BULLET_SPEED 20

/* Velocidade dos projéteis inimigos */
#define ALIEN_BULLET_SPEED 8


/*==============================================================================
 * Configurações do jogador
 *============================================================================*/

/* Vida máxima do jogador */
#define PLAYER_MAX_HEALTH 100


/*==============================================================================
 * Configurações visuais
 *============================================================================*/

/* Quantidade máxima de estrelas do starfield */
#define MAX_STARS 500


/*==============================================================================
 * Configurações da fonte bitmap
 *============================================================================*/

/* Largura de cada caractere */
#define GLYPH_WIDTH 18

/* Altura de cada caractere */
#define GLYPH_HEIGHT 28


/*==============================================================================
 * Configurações de buffers e strings
 *============================================================================*/

/* Tamanho máximo do nome no ranking */
#define MAX_SCORE_NAME_LENGTH 16

/* Tamanho máximo de nomes genéricos */
#define MAX_NAME_LENGTH 32

/* Tamanho máximo de linhas de texto */
#define MAX_LINE_LENGTH 1024


/*==============================================================================
 * Configurações de áudio
 *============================================================================*/

/* Quantidade máxima de canais simultâneos */
#define MAX_SND_CHANNELS 8


/*==============================================================================
 * Configurações de ranking
 *============================================================================*/

/* Quantidade máxima de highscores */
#define NUM_HIGHSCORES 8


/*==============================================================================
 * Facções das entidades
 *
 * Utilizadas para:
 * - colisão
 * - dano
 * - identificação de aliados/inimigos
 *============================================================================*/
enum
{
    SIDE_PLAYER,
    SIDE_ALIEN
};


/*==============================================================================
 * Tipos de entidades
 *
 * Identificam o comportamento lógico de cada entidade.
 *============================================================================*/
enum
{
    ET_PLAYER,
    ET_ENEMY,
    ET_PLAYER_BULLET,
    ET_ALIEN_BULLET,
    ET_POINTS_POD
};


/*==============================================================================
 * Canais de áudio
 *
 * Permitem separar diferentes categorias de som.
 *============================================================================*/
enum
{
    CH_ANY = -1,

    CH_PLAYER,
    CH_ALIEN_FIRE,
    CH_POINTS
};


/*==============================================================================
 * Identificadores de efeitos sonoros
 *
 * Utilizados para indexar o array global de sons.
 *============================================================================*/
enum
{
    SND_PLAYER_FIRE,
    SND_ALIEN_FIRE,
    SND_PLAYER_DIE,
    SND_ALIEN_DIE,
    SND_POINTS,

    SND_MAX
};


/*==============================================================================
 * Alinhamento de texto
 *
 * Utilizado pelo sistema de renderização bitmap.
 *============================================================================*/
enum
{
    TEXT_LEFT,
    TEXT_CENTER,
    TEXT_RIGHT
};