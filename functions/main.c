// main.c
#include "common.h"

App         app; // Declara uma variavel global 'app' do tipo 'App' que armazena o estado geral da aplicação
Stage       stage; // DEclara uma variavel global 'stage' do tipo 'Stage' geranciar os dados da fase/nivel do jogo
Entity      *player; // Declara um ponteiro global 'player' do tipo 'Entity'que aponta para o objeto do personagem do jogador
SDL_Texture *bulletTexture; // Declara um ponteiro global 'bulletTexture' para um SDL_Texture', usado para as texturas dos tiros
SDL_Texture *enemyTexture; // Declara um ponteiro global 'enenmyTexture' para um SDL_Texture', usado para texturas dos inimigos


int main(int argc, char *argv[]) // Funcao principal, o ponto de entrada do programa
{

    // Inicializa 'then' com a contagem atual de "ticks" (milissegundos desde a inicializacao do SDL), usado para limitar a taxa de quadros
    long then      = SDL_GetTicks();
    float remainder = 0.0f; // Inicializa 'remaider' com 0.0f, usado para acumular tempo fracionado para limite de taxa de quadros

    memset(&app,   0, sizeof(app)); // Zera toda a estrutura 'app', limpando seus membros
    memset(&stage, 0, sizeof(stage)); // Zera toda a estrutura 'stage', limpando seus membros

    initSDL(); // Chama a funcao para inicializar o SDL, configurando graficos, audio, etc
    atexit(cleanup); // Registra a funcao 'cleanup' para ser chamada automaticamente quando o programa for encerrado
    initStage(); // Chama a funcao para inicializar a fase ou nivel do jogo, configurando entidades e o estado inicial do jogo

    while (1) // Inicializa o loop infinito, que e o loop principal do jogo
{
        prepareScene(); // Chama uma funcao para preparar a cena de renderizacao (limpar a tela).
        doInput(); // Chama a funcao para lidar com a entrada do usuario (tecla, mouse, etc)
        app.delegate.logic(); // Chama a funcao 'logic' atraves de um delegate na struct 'app', que cuida das atualizacoes e mecanicas do jogo
        app.delegate.draw(); // Chama a funcao 'draw' atraves de um delegate na struct 'app', que renderiza os elementos do jogo
        presentScene(); // Chama a funcao para apresentar a cena renderizada na tela (trocar buffers)
        capFrameRate(&then, &remainder); // Chama uma funcao para limitar a taxa de quadros, garantindo uma velocidade de jogo consistente
    }
    return 0;
}

// Define uma funcao auxiliar static para limitar a taxa de quadros, recebendo ponteiros para 'then' e 'remaider'
static void capFrameRate(long *then, float *remainder)
{
    long wait, frameTime; // Declara variaveis locais 'wait'(PARA O TEMPO DE ESPERA) e 'frameTime' (PARA O TEMPO QUE O QUADRO ATUAL LEVOU)
    wait = 16 + *remainder; // Calcula o tempo inicial de 'wait': 16 milissegundos (para ~60 FPS) mais qualquer 'remaider' acumulado
    *remainder -= (int)*remainder; //Remove a parte inteira de 'remaider', mantendo apenas a parte fracionada
    frameTime = SDL_GetTicks() - *then; // Calcula o tempo decorrido desde o ultimo quadro
    wait -= frameTime; // Reduz o tempo de 'wait' pelo tempo real que o quadro levou

    if (wait < 1) //Verifica se o tempo de 'wait' calculado e menor que 1 milissegundos
    {
        wait = 1; // Se sim, define 'wait' par a um minimo de 1 milissegundos para garantir pelo menos algum atraso
    }

    SDL_Delay(wait); // Pausa a execucao do programa pela duracao calculada de 'wait'
    *remainder += 0.667f; // Adiciona uma constante (0.667f, aproximadamente 1/60 de segundo menos 1/16) a 'remaider' para ajustar a taxa de quadros
    *then = SDL_GetTicks(); // Atualiza 'then' para a contagem atual de "ticks" para o calculo do proximo quadro
}
