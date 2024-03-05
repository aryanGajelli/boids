#include "init.h"

#include <SDL.h>
#include <SDL_render.h>
#include <stdio.h>

#include "cfg.h"

void initSDL() {
    int rendererFlags, windowFlags;

    rendererFlags = SDL_RENDERER_ACCELERATED;

    windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    app.win = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, windowFlags);

    if (!app.win) {
        printf("Failed to open %d x %d window: %s\n", WIDTH, HEIGHT, SDL_GetError());
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");

    app.renderer = SDL_CreateRenderer(app.win, -1, rendererFlags);

    if (!app.renderer) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }
}

void cleanupSDL() {
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.win);
    SDL_Quit();
}