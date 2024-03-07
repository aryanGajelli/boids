#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "boid.h"
#include "cfg.h"
#include "draw.h"
#include "init.h"
#include "input.h"

App app;
SDL_Texture* boidTex;
Boid boids[NUM_BOIDS];

int main(int argc, char* argv[]) {
    memset(&app, 0, sizeof(App));
    srand(time(NULL));

    initSDL();
    initDraw();
    initBoids();
    atexit(cleanupSDL);

    while (1) {
        Uint32 start = SDL_GetPerformanceCounter();
        prepareScene();
        processEvents();
        for (int i = 0; i < NUM_BOIDS; i++) {
            flock(&boids[i]);
            updateBoid(&boids[i]);
            drawBoid(&boids[i]);
        }
        presentScene();

        // SDL_framerateDelay(&fpsm);
        Uint32 end = SDL_GetPerformanceCounter();
        double elapsed = (end - start) / (double)SDL_GetPerformanceFrequency();
        while (elapsed < 1.0 / FPS){
            end = SDL_GetPerformanceCounter();
            elapsed = (end - start) / (double)SDL_GetPerformanceFrequency();
        }
        printf("Render time: %fms\n", elapsed * 1000.0);
    }

    return 0;
}