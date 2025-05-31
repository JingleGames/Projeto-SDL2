#include <SDL2/SDL.h>
#include <stdio.h>

#define WINDOW_WIDTH 800     // Largura da janela em pixels
#define WINDOW_HEIGHT 600    // Altura da janela em pixels

#define NUM_BARS 5           // Número de barras no gráfico

int main(int argc, char *argv[]) {
    // Inicializa o subsistema de vídeo do SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erro SDL_Init: %s\n", SDL_GetError());
        return 1;           // Sai do programa se a inicialização falhar
    }

    // Cria uma janela SDL com título, posição centralizada e tamanho definido
    SDL_Window *window = SDL_CreateWindow("Gráfico de Barras",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    // Verifica se a criação da janela falhou
    if(!window) {
        printf("Erro janela: %s\n", SDL_GetError());
        SDL_Quit();         // Finaliza SDL antes de sair
        return 1;
    }

    // Cria um renderizador acelerado para a janela criada
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Verifica se a criação do renderizador falhou
    if(!renderer) {
        printf("Erro renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window); // Destroi a janela antes de sair
        SDL_Quit();
        return 1;
    }

    // Array com os dados do gráfico: quantidade de hambúrgueres consumidos
    int data[NUM_BARS] = {120, 75, 200, 50, 150};

    // Labels dos tipos de hambúrguer (não usados para desenhar aqui)
    const char* labels[NUM_BARS] = {"Classic", "Cheese", "Bacon", "Veggie", "Double"};

    // Encontrar o valor máximo para normalizar a altura das barras
    int max_data = 0;
    for(int i=0; i < NUM_BARS; i++) {
        if(data[i] > max_data) max_data = data[i];
    }

    int running = 1;      // Controle do loop principal
    SDL_Event event;      // Estrutura para eventos SDL (teclado, mouse, etc.)

    // Loop principal do programa (até fechar a janela)
    while(running) {
        // Verifica eventos na fila (fechar janela, etc.)
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) running = 0; // Sai do loop se fechar janela
        }

        // Define a cor para limpar a tela (branco)
        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        SDL_RenderClear(renderer);  // Limpa a tela com a cor definida

        // Desenha os eixos X e Y do gráfico na cor preta
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderDrawLine(renderer, 50, 50, 50, WINDOW_HEIGHT - 50);           // eixo Y vertical
        SDL_RenderDrawLine(renderer, 50, WINDOW_HEIGHT - 50, WINDOW_WIDTH - 50, WINDOW_HEIGHT - 50); // eixo X horizontal

        // Desenha pequenas linhas (tic marks) horizontais no eixo Y para referência visual
        int num_lines = 5;  // Quantidade de linhas de referência
        for(int i=0; i<=num_lines; i++) {
            int y = 50 + i * (WINDOW_HEIGHT - 100) / num_lines;  // Calcula posição Y da linha
            SDL_RenderDrawLine(renderer, 45, y, 50, y);           // Linha pequena (tic) na borda esquerda do eixo Y
        }

        // Calcula a largura da barra considerando espaçamento entre elas
        int bar_width = (WINDOW_WIDTH - 100) / (NUM_BARS * 2);

        // Desenha cada barra do gráfico
        for(int i=0; i<NUM_BARS; i++) {
            // Calcula altura da barra proporcional ao valor dos dados e tamanho da janela
            int bar_height = (data[i] * (WINDOW_HEIGHT - 100)) / max_data;

            // Define cor diferente para cada barra usando switch
            switch(i) {
                case 0: SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); break;    // vermelho
                case 1: SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255); break;  // laranja
                case 2: SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); break;  // amarelo
                case 3: SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255); break;    // verde
                case 4: SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); break;    // azul
                default: SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); break;
            }

            // Define o retângulo da barra (posição x, y, largura e altura)
            SDL_Rect bar = {
                .x = 50 + (i * 2 + 1) * bar_width,                 // espaçamento horizontal entre barras
                .y = WINDOW_HEIGHT - 50 - bar_height,              // altura posicionada de baixo para cima
                .w = bar_width,
                .h = bar_height
            };

            SDL_RenderFillRect(renderer, &bar);   // Desenha a barra preenchida
        }

        SDL_RenderPresent(renderer); // Apresenta o que foi desenhado na tela
        SDL_Delay(16);               // Delay para ~60 FPS
    }

    // Libera recursos usados pelo SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
