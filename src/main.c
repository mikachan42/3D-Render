#include <stdio.h>
#include "mesh.h"
#include "renderer.h"
#include "config.h"


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;



void init() {
     if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Fehler: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS, &window, &renderer);
}


void terminate() {
    printf("debugggggging");
    // Wait for windows to close, without blocking Wayland
    SDL_Event event;
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
            if (event.type == SDL_KEYDOWN) running = 0; // exit button
        }
        SDL_Delay(16); // ~60fps, CPU Conservation
    }

    SDL_DestroyRenderer(renderer); // frees up renderer and its memory
    SDL_DestroyWindow(window); // closes windows and releases it
    SDL_Quit(); // Shuts down SDL, frees up all internal resources
}


void video () {
    init();
    int timer = 600;
    int running = 1;

    while (running) {

         SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;
        }


        if (timer >= 0) {
            frame(renderer);
            timer--;
        } else {
            running = 0;
        }

        SDL_Delay(1000 / FPS);
    }

    terminate();


}



int main(void) {


    video();

    
    return 0;   
}