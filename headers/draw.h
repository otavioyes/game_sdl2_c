// draw.h
#ifndef DRAW_H
#define DRAW_H

void prepareScene(void);
void presentScene(void);

SDL_Texture *loadTexture(char *filename);
void blit(SDL_Texture *texture, int x, int y, float scale);

#endif
