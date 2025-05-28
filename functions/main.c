// main.c
#include "common.h"

App app;
Entity player;

int main(int argc, char *argv[])
{
    memset(&app, 0, sizeof(App));
    memset(&player, 0, sizeof(Entity)); /*declaracao da Entidade chamada player*/

    initSDL();

    atexit(cleanup);

    player.texture = loadTexture("gfx/player1.png");
    player.x = 100;
    player.y =100;


    while (1) {
        prepareScene();

         doInput();

            if(player.texture == NULL)
            {
                SDL_Log("A textura do player está NULL! Verificar o caminho ou o formato da imagem.");
            }

            if(app.up)
            {
                player.y -= 4;
            }
            if(app.down)
            {
                player.y += 4;
            }
            if(app.left)
            {
                player.x -= 4;
            }
            if(app.right)
            {
                player.x += 4;
            }

        blit(player.texture, player.x, player.y, 0.2f);

        presentScene();

        SDL_Delay(16);  // ~60 FPS
    }

    return 0;
}
