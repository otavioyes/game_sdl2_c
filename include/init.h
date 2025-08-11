// init.h
// Cabecalho com funcoes de inicializacao e finalizacao do SDL e do jogo

#ifndef INIT_H // Prevencao contra multiplas inclusoes do arquivo
#define INIT_H // Define a flag de inclusoes unica

void initSDL(void); // Inicializa a lib SDL, a janela, renderizador, e outros necessarios
void cleanup(void); // Libera recursos e encerra corretamente a aplicacao (fecha janela, detroi texturas, etc)

#endif // Finaliza a protecao contra multiplas inclusoes
