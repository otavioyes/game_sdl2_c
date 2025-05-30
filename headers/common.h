// common.h

#ifndef COMMON_H
#define COMMON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "structs.h"

#include "draw.h"
#include "init.h"
#include "input.h"
#include "stage.h"

extern App         app;
extern Stage       stage;
extern Entity     *player;
extern SDL_Texture *bulletTexture;

void capFrameRate(long *then, float *remainder);

#endif // COMMON_H

