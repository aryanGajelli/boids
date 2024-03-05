#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "cfg.h"
#include "draw.h"
#include "init.h"
#include "input.h"

App app;
int main(int argc, char* argv[]) {
    memset(&app, 0, sizeof(App));

    initSDL();
    initDraw();
    atexit(cleanupSDL);

    while (1) {
        uint64_t start = SDL_GetPerformanceCounter();
        prepareScene();
        processEvents();

        presentScene();
        

        SDL_Delay(5);
        uint64_t end = SDL_GetPerformanceCounter();
        double elapsed = (end - start) / (double)SDL_GetPerformanceFrequency();

        // printf("FPS: %lf\n", 1 / elapsed);
    }

    return 0;
}