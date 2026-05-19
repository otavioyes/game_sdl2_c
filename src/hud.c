/*
 * hud.c
 */

#include "common.h"

#include "text.h"
#include "player.h"

extern Stage      stage;
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
    int currentHighscore;

    currentHighscore = highscores.highscore[0].score;

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
     * Exibe o highscore atual.
     *
     * Quando a pontuação da partida ultrapassa o recorde,
     * o HUD passa a exibir o score atual em destaque.
     */
    if (stage.score < currentHighscore) {

        drawText(SCREEN_WIDTH - 10,
                 10,
                 255,
                 255,
                 255,
                 TEXT_RIGHT,
                 "HIGHSCORE: %03d",
                 currentHighscore);

    } else {

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