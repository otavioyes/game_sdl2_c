/*
 * defs.h
 *
 * Arquivo central de definições globais do jogo.
 *
 * Responsabilidades:
 * - Definir constantes globais
 * - Configurar limites do sistema
 * - Padronizar valores de gameplay
 * - Declarar enums compartilhados
 * - Fornecer macros utilitárias
 */


/*==============================================================================
 * Macros utilitárias
 *============================================================================*/

/* Retorna menor valor entre dois elementos */
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

/* Retorna maior valor entre dois elementos */
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

/*
 * Cópia segura de string.
 *
 * Garante terminação nula para evitar
 * problemas de buffer overflow.
 */
#define STRNCPY(dest, src, n) \
        strncpy(dest, src, n); \
        dest[n - 1] = '\0';


/*==============================================================================
 * Configurações de resolução
 *============================================================================*/

/* Largura da janela do jogo */
#define SCREEN_WIDTH 1280

/* Altura da janela do jogo */
#define SCREEN_HEIGHT 720


/*==============================================================================
 * Limites de buffers e strings
 *============================================================================*/

/* Tamanho máximo do nome do jogador no ranking */
#define MAX_SCORE_NAME_LENGTH 16

/* Tamanho máximo para nomes genéricos */
#define MAX_NAME_LENGTH       32

/* Tamanho máximo de linhas de texto */
#define MAX_LINE_LENGTH       1024


/*==============================================================================
 * Configurações gerais do jogo
 *============================================================================*/

/* Frames por segundo */
#define FPS 60


/*==============================================================================
 * Configurações de movimentação
 *============================================================================*/

/* Velocidade do jogador */
#define PLAYER_SPEED        4

/* Velocidade do projétil do jogador */
#define PLAYER_BULLET_SPEED 20

/* Velocidade do projétil inimigo */
#define ALIEN_BULLET_SPEED  8


/*==============================================================================
 * Configurações de entrada
 *============================================================================*/

/* Quantidade máxima de teclas monitoradas */
#define MAX_KEYBOARD_KEYS   350


/*==============================================================================
 * Facções das entidades
 *============================================================================*/

/* Entidades controladas pelo jogador */
#define SIDE_PLAYER 0

/* Entidades inimigas */
#define SIDE_ALIEN  1


/*==============================================================================
 * Configurações visuais
 *============================================================================*/

/* Quantidade máxima de estrelas do fundo */
#define MAX_STARS   500


/*==============================================================================
 * Configurações de áudio
 *============================================================================*/

/* Número máximo de canais simultâneos */
#define MAX_SND_CHANNELS 8


/*==============================================================================
 * Configurações de ranking
 *============================================================================*/

/* Quantidade máxima de highscores */
#define NUM_HIGHSCORES 8


/*==============================================================================
 * Configurações da fonte bitmap
 *============================================================================*/

/* Altura de cada caractere */
#define GLYPH_HEIGHT    28

/* Largura de cada caractere */
#define GLYPH_WIDTH     18


/*==============================================================================
 * Configurações do jogador
 *============================================================================*/

/* Vida máxima do jogador */
#define PLAYER_MAX_HEALTH 100


/*==============================================================================
 * Tipos de entidades do jogo
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
 *============================================================================*/
enum
{
    TEXT_LEFT,
    TEXT_CENTER,
    TEXT_RIGHT
};