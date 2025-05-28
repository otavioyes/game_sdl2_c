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

    SDL_QueryTexture(player.texture, NULL, NULL, &player.width, &player.height);

    player.scale = 0.2f;

    player.x = 0;
    player.y = 0;


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

        if(player.x + player.width * player.scale > SCREEN_WIDTH)
        {
            player.x = SCREEN_WIDTH - player.width * player.scale;
        }

        if(player.x < 0)
        {
            player.x = 0;
        }

        if(player.y + player.height * player.scale > SCREEN_HEIGHT)
        {
            player.y = SCREEN_HEIGHT - player.height * player.scale;
        }

        if(player.y < 0)
        {
            player.y = 0;
        }

        blit(player.texture, player.x, player.y, player.scale);

        presentScene();

        SDL_Delay(16);  // ~60 FPS
    }

    return 0;
}
