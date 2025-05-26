// main.c
#include "common.h"

App app;
Entity player;

int main(int argc, char *argv[])
{
    memset(&app, 0, sizeof(App));
    memset(&player, 0, sizeof(Entity)); /*declaracao da Entidade chamada player*/

    initSDL();

    player.x = 0;
    player.y = 0;
    player.texture = loadTexture("gfx/1st_nave.png");

    atexit(cleanup);

    while (1) {
        prepareScene();
        doInput();

             if (player.texture == NULL) {
                 SDL_Log("A textura do player está NULL! Verifique o caminho ou o formato da imagem.");
                }

        blit(player.texture, player.x, player.y, 0.2f);
        presentScene();
        SDL_Delay(16);  // ~60 FPS
    }

    return 0;
}
