// main.c
#include "common.h"

App app;
Entity player;
Entity bullets [MAX_BULLETS];

int main(int argc, char *argv[])
{
    memset(&app, 0, sizeof(App));
    memset(&player, 0, sizeof(Entity)); /*declaracao da Entidade chamada player*/
    for(int i = 0; i < MAX_BULLETS; i++)
    {
        memset(&bullets[i], 0, sizeof(Entity));
        SDL_QueryTexture(bullets[i].texture, NULL, NULL, &bullets[i].width, &bullets[i].height);
    }

    initSDL();

    atexit(cleanup);

    player.texture = loadTexture("gfx/player1.png");

    for(int i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i].texture = loadTexture("gfx/playerBullet.png");
    }


    SDL_QueryTexture(player.texture, NULL, NULL, &player.width, &player.height);

    player.scale = 0.2f;
    player.speed = 4;

    player.x = 0;
    player.y = 0;

    int lastBulletTime = 0;
    const int bulletCooldown = 250; //tempo ate a proxima bala

    while (1) {
        prepareScene();

         doInput();

        player.x += player.dx;
        player.y += player.dy;

            if(player.texture == NULL)
            {
                SDL_Log("A textura do player está NULL! Verificar o caminho ou o formato da imagem.");
            }
            //velocidade do player
            if(app.up)
            {
                player.y -= player.speed;
            }
            if(app.down)
            {
                player.y += player.speed;
            }
            if(app.left)
            {
                player.x -= player.speed;
            }
            if(app.right)
            {
                player.x += player.speed;
            }
            //fazendo a bala ser disparada pelo meio do player
           int centerX = player.x + (player.width * player.scale) / 2;
           int centerY = player.y + (player.height * player.scale) / 2;

            if(app.fire && SDL_GetTicks() - lastBulletTime >= bulletCooldown)
            {
                for(int i = 0; i < MAX_BULLETS; i++)
                    {
                        if(bullets[i].health == 0)
                            {
                                bullets[i].x = centerX - (bullets[i].width * player.scale) / 2;
                                bullets[i].y = centerY - (bullets[i].height * player.scale) / 2;
                                bullets[i].dx = 8;
                                bullets[i].dy = 0;
                                bullets[i].health = 1;
                                lastBulletTime = SDL_GetTicks(); //atualiza o tempo do ultimo tiro
                                break;
                            }
                    }
            }


            for(int i = 0; i < MAX_BULLETS; i++)
            {
                if(bullets[i].health > 0)
                {
                    bullets[i].x += bullets[i].dx;
                    bullets[i].y += bullets[i].dy;

                    //Desativa a bala se sair da tela
                    if(bullets[i].x > SCREEN_WIDTH || bullets[i].x < 0 || bullets[i].y > SCREEN_HEIGHT || bullets[i].y < 0)
                    {
                        bullets[i].health = 0;
                    }

                        blit(bullets[i].texture, bullets[i].x, bullets[i].y, player.scale);
                }
            }


        //impede que o player passe da largura da tela para a direita.
        if(player.x + player.width * player.scale > SCREEN_WIDTH)
        {
            player.x = SCREEN_WIDTH - player.width * player.scale;
        }
        //impede que o player passe da largura para e esquerda.
        if(player.x < 0)
        {
            player.x = 0;
        }

        //impede que  o player passe da altura Y
        if(player.y + player.height * player.scale > SCREEN_HEIGHT)
        {
            player.y = SCREEN_HEIGHT - player.height * player.scale;
        }
        //impede que player para baixo Y
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
