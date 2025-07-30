// common.h
// Arquivo de cabecalho que reune includes comnuns e declara variaveis globais usadas em todo o jogo

#ifndef COMMON_H // Prevencao contra inclusoes multiplas do mesmo cabecalho
#define COMMON_H // Define o identificador para a protecao contra multiplas inclusoes

#include <SDL2/SDL.h> // Inclui lib principal do SDL (janela , eventos renderizacao, etc)
#include <SDL2/SDL_image.h> // Inclui suporte a imagem como PNG, JPG, etc
#include <stdio.h> // Inclui funcao padrao de entrada/saida, como printf()
#include <stdlib.h>// Inclui funcoes padrao de alocacao de memoria, conversao, etc
#include <string.h>// Inclui funcoes para manipulacao de strings (strlen, strcpy, etc)

#include "defs.h" // Inclui definicoes e constantes do projeto (macros e limites)
#include "structs.h" // Inclui declaracoes de struct como App, Stage, Entity, Delegate

#include "draw.h" // Inclui funcoes relacionada a renderizacao (desenho)
#include "init.h" // Inclui funcoes de inicilizacao do jogo e SDL
#include "input.h" // Inclui funcoes relacionadas a entrada do teclado/mouse
#include "stage.h" // Inclui logica especifica da fase (como atualizacao e controle de entidades)
#include "util.h" // Inclui funcoes utilitarias

extern App          app; // Declara a variavel global 'app', que contem o estado principal da aplicacao
extern Stage        stage; // Declara a variavel global 'stage', que armazena os dados da fase (inimigo, tiros)
extern Entity       *player; // Declara um ponteiro global para a entidade do jogador
extern SDL_Texture  *bulletTexture; // Declara ponteiro global para a textura dos tiros
extern SDL_Texture  *enemyTexture; // Declara ponteiro global para a textura do inimigo
extern SDL_Texture  *alienBulletTexture;
extern SDL_Texture  *playerTexture;

static void capFrameRate(long *then, float *remainder);
//Declara uma funcao static para limitar o frame rate (usado no loop principal do jogo)

#endif // COMMON_H // Fim da protecao contra multiplas inclusoes

