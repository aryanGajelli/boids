#include "draw.h"

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <math.h>
// #include <gl.h>

#include <stdlib.h>

#include "boid.h"
#include "cfg.h"
#include "structs.h"

void initDraw(void) {
    boidTex = IMG_LoadTexture(app.renderer, "./assets/boid.png");
    if (boidTex == NULL) {
        printf("Error creating texture: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_SetTextureAlphaMod(boidTex, 0xdf);
}

void prepareScene(void) {
    // next 2 lines are basically a background color refresh
    SDL_SetRenderDrawColor(app.renderer, 0x3a, 0x3a, 0x3a, 0xff);
    SDL_RenderClear(app.renderer);
}

void presentScene(void) {
    SDL_RenderPresent(app.renderer);
}
