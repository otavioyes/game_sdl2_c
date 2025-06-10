// defs.h
// Arquivo de definicoes e constantes globais para o jogo

#ifndef DEFS_H // Prevencao contra multiplas inclusoes do cabecalho
#define DEFS_H // Define o identificador para a protecao contra multiplas inclusoes

#define SCREEN_WIDTH        1200 // Largura da janela do jogo em pixels
#define SCREEN_HEIGHT       720 // Altura da janela do jogo em pixels

#define PLAYER_SPEED        4 // Velocidade de movimento do jogador (pixel por frame)
#define PLAYER_BULLET_SPEED 16 // Velocidade dos projeteis disparados pelo jogador

#define MAX_KEYBOARD_KEYS   350 //Numero maximo de teclas do teclado que podem ser rastreados
#define MAX_MOUSE_BUTTONS   3 // Numero maximo de botoes do mouse (esquerdo, direito e meio)

#define MAX_BULLETS         128 //Numeros maximo de projeteis (bullets) ativos ao mesmo tempo

//colisao
/*
#define SIDE_PLAYER     0 // Identificador de lado para o jogador em colisao
#define SIDE_ALIEN      1 // Identificador de lado para o inimigo em colisao
*/


#endif // DEFS_H // Fim da protecao contra multiplas inclusoes
