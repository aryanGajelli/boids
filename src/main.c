#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "boid.h"
#include "cfg.h"
#include "draw.h"
#include "init.h"
#include "input.h"

App app;
SDL_Texture* boidTex;
Boid boids[NUM_BOIDS];
double dT = 0;

int main(int argc, char* argv[]) {
    memset(&app, 0, sizeof(App));
    srand(time(NULL));

    initSDL();
    initDraw();
    initBoids();
    atexit(cleanupSDL);

    Uint32 physLast = SDL_GetPerformanceCounter();
    while (1) {
        Uint32 start = SDL_GetPerformanceCounter();
        // get physics time delta
        Uint32 physNow = start;
        dT = 100 * (physNow - physLast) / (double)SDL_GetPerformanceFrequency();
        physLast = physNow;
        prepareScene();
        processEvents();
        updateGrid();
        
        for (int i = 0; i < NUM_BOIDS; i++) {
            flock(&boids[i]);
            
            drawBoid(&boids[i]);
        }
        presentScene();
       

        Uint32 end = SDL_GetPerformanceCounter();
        double elapsed = (end - start) / (double)SDL_GetPerformanceFrequency();
        while (elapsed < 1.0 / FPS){
            end = SDL_GetPerformanceCounter();
            elapsed = (end - start) / (double)SDL_GetPerformanceFrequency();
        }
        printf("FPS: %.3f\n", 1.0/elapsed);
    }

    return 0;
}