#include <SDL2/SDL.h>
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define NUM_BARS 5

int main(int argc, char *argv[]) {
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erro SDL_Init: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Gráfico de Hambúrgueres",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if(!window) {
        printf("Erro janela: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if(!renderer) {
        printf("Erro renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Dados (quantidade de hambúrgueres consumidos)
    int data[NUM_BARS] = {120, 75, 200, 50, 150};
    const char* labels[NUM_BARS] = {"Classic", "Cheese", "Bacon", "Veggie", "Double"};

    // Descobrir máximo para escala
    int max_data = 0;
    for(int i=0; i < NUM_BARS; i++) {
        if(data[i] > max_data) max_data = data[i];
    }

    int running = 1;
    SDL_Event event;

    while(running) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) running = 0;
        }

        // Fundo branco
        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        SDL_RenderClear(renderer);

        // Desenhar eixos
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        // eixo Y
        SDL_RenderDrawLine(renderer, 50, 50, 50, WINDOW_HEIGHT - 50);
        // eixo X
        SDL_RenderDrawLine(renderer, 50, WINDOW_HEIGHT - 50, WINDOW_WIDTH - 50, WINDOW_HEIGHT - 50);

        // Desenhar linhas horizontais (tic marks) no eixo Y, sem números
        int num_lines = 5;
        for(int i=0; i<=num_lines; i++) {
            int y = 50 + i * (WINDOW_HEIGHT - 100) / num_lines;
            SDL_RenderDrawLine(renderer, 45, y, 50, y); // tic no eixo Y
        }

        // Desenhar barras
        int bar_width = (WINDOW_WIDTH - 100) / (NUM_BARS * 2);
        for(int i=0; i<NUM_BARS; i++) {
            int bar_height = (data[i] * (WINDOW_HEIGHT - 100)) / max_data;

            // Cores diferentes por barra
            switch(i) {
                case 0: SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); break;    // vermelho
                case 1: SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255); break;  // laranja
                case 2: SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); break;  // amarelo
                case 3: SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255); break;    // verde
                case 4: SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); break;    // azul
                default: SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); break;
            }

            SDL_Rect bar = {
                .x = 50 + (i * 2 + 1) * bar_width,
                .y = WINDOW_HEIGHT - 50 - bar_height,
                .w = bar_width,
                .h = bar_height
            };

            SDL_RenderFillRect(renderer, &bar);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}