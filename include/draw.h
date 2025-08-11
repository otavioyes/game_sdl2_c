// draw.h
// Cabecalho com funcoes relacionada a renderizacao e carregamento de texturas

#ifndef DRAW_H // Prevencao contra multiplas inclusoes de arquivos
#define DRAW_H // Define a flag de inclusoes unica

void prepareScene(void); // Prepara a cena para renderizacao (normalmente limpa a tela)
void presentScene(void); // Apresenta a cena renderizada na tela (swap de buffers)
void draw(void); // Funcao principal de desenho, geralmente chamada a cada frame

SDL_Texture *loadTexture(char *filename); // Carrega uma textura de imagem a partir de um arquivo
void blit(SDL_Texture *texture, int x, int y, float scale); // Desenha yna textura na posicao x, y, aplicando fator scale

#endif // DRAW_H // Finaliza a protecao contra multiplas inclusoes

