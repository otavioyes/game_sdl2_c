/*
 * hud.c
 */

#include "common.h"
#include "text.h"
#include "player.h"

extern Stage stage;
extern Highscores highscores;



/*==============================================================================
 * Renderiza interface HUD (Heads-Up Display) da fase.
 *
 * Responsabilidades:
 * - Exibir pontuação atual do jogador
 * - Exibir barra de vida do jogador
 * - Exibir maior pontuação registrada
 * - Destacar novo recorde em tempo real
 *============================================================================*/
void drawHud(void)
{
    /* Renderiza pontuação atual */
    drawText(10,
             10,
             255,
             255,
             255,
             TEXT_LEFT,
             "SCORE: %03d",
             stage.score);

    /* Renderiza barra de vida do jogador */
    drawPlayerHealthBar();

    /*
     * Exibe highscore padrão enquanto
     * jogador não ultrapassar recorde atual.
     */
    if (stage.score < highscores.highscore[0].score) {

        drawText(SCREEN_WIDTH - 10,
                 10,
                 255,
                 255,
                 255,
                 TEXT_RIGHT,
                 "HIGHSCORE: %03d",
                 highscores.highscore[0].score);

    } else {

        /*
         * Destaca novo recorde utilizando
         * coloração verde.
         */
        drawText(SCREEN_WIDTH - 10,
                 10,
                 0,
                 255,
                 0,
                 TEXT_RIGHT,
                 "HIGHSCORE: %03d",
                 stage.score);
    }
}