/*
 * sound.c
 */

#include <SDL2/SDL_mixer.h>

#include "common.h"
#include "sound.h"

static void loadSounds(void);

static Mix_Chunk *sounds[SND_MAX];
static Mix_Music *music;


/*==============================================================================
 * Inicializa o sistema de áudio do jogo.
 *
 * Responsabilidades:
 * - Limpar lista de efeitos sonoros
 * - Reiniciar ponteiro da música atual
 * - Carregar efeitos sonoros utilizados no gameplay
 *============================================================================*/
void initSounds(void)
{
    memset(sounds, 0, sizeof(Mix_Chunk *) * SND_MAX);

    music = NULL;

    loadSounds();
}


/*==============================================================================
 * Carrega uma música para reprodução.
 *
 * Responsabilidades:
 * - Parar música anterior, se existir
 * - Liberar memória da música anterior
 * - Carregar novo arquivo de música
 * - Validar falha de carregamento
 *============================================================================*/
void loadMusic(char *filename)
{
    if (music != NULL) {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
        music = NULL;
    }

    music = Mix_LoadMUS(filename);

    if (music == NULL) {
        SDL_Log("Mix_LoadMUS falhou para '%s': %s",
                filename,
                Mix_GetError());
    }
}


/*==============================================================================
 * Reproduz a música atualmente carregada.
 *
 * Responsabilidades:
 * - Validar existência da música
 * - Reproduzir em loop ou apenas uma vez
 *============================================================================*/
void playMusic(int loop)
{
    if (music == NULL) {
        return;
    }

    Mix_PlayMusic(music, (loop) ? -1 : 0);
}


/*==============================================================================
 * Reproduz um efeito sonoro.
 *
 * Responsabilidades:
 * - Validar identificador do som
 * - Validar existência do efeito carregado
 * - Tocar áudio no canal solicitado
 *============================================================================*/
void playerSound(int id, int channel)
{
    if (id < 0 || id >= SND_MAX) {
        SDL_Log("ID de som inválido em playerSound: %d", id);
        return;
    }

    if (sounds[id] == NULL) {
        SDL_Log("Som não carregado em playerSound. ID: %d", id);
        return;
    }

    Mix_PlayChannel(channel, sounds[id], 0);
}


/*==============================================================================
 * Carrega todos os efeitos sonoros do jogo.
 *
 * Responsabilidades:
 * - Carregar arquivos de áudio do gameplay
 * - Associar arquivos aos IDs definidos no enum de sons
 * - Validar falhas individuais de carregamento
 *============================================================================*/
static void loadSounds(void)
{
    sounds[SND_PLAYER_FIRE] = Mix_LoadWAV("sound/334227__jradcoolness__laser.ogg");
    sounds[SND_ALIEN_FIRE]  = Mix_LoadWAV("sound/196914__dpoggioli__laser-gun.ogg");
    sounds[SND_PLAYER_DIE]  = Mix_LoadWAV("sound/245372__quaker540__hq-explosion.ogg");
    sounds[SND_ALIEN_DIE]   = Mix_LoadWAV("sound/10 Guage Shotgun-SoundBible.com-74120584.ogg");
    sounds[SND_POINTS]      = Mix_LoadWAV("sound/342749__rhodesmas__notification-01.ogg");

    if (sounds[SND_PLAYER_FIRE] == NULL) {
        SDL_Log("Erro ao carregar SND_PLAYER_FIRE: %s", Mix_GetError());
    }

    if (sounds[SND_ALIEN_FIRE] == NULL) {
        SDL_Log("Erro ao carregar SND_ALIEN_FIRE: %s", Mix_GetError());
    }

    if (sounds[SND_PLAYER_DIE] == NULL) {
        SDL_Log("Erro ao carregar SND_PLAYER_DIE: %s", Mix_GetError());
    }

    if (sounds[SND_ALIEN_DIE] == NULL) {
        SDL_Log("Erro ao carregar SND_ALIEN_DIE: %s", Mix_GetError());
    }

    if (sounds[SND_POINTS] == NULL) {
        SDL_Log("Erro ao carregar SND_POINTS: %s", Mix_GetError());
    }
}