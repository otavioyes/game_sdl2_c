/*
 * sound.c
 */

#include <SDL2/SDL_mixer.h>

#include "common.h"

#include "sound.h"

static void loadSounds(void);

static Mix_Chunk *sounds[SND_MAX];
static Mix_Chunk *music;

void initSounds(void) {
    memset(sounds, 0, sizeof(Mix_Chunk *) * SND_MAX);
    music = NULL;
    loadSounds();
}

void loadSounds(char *filename) {
    if (music != NULL) {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
        music = NULL;
    }
    music = Mix_LoadMUS(filename);
}

