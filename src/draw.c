#include "draw.h"

#include <SDL.h>
#include <SDL_render.h>

#include "cfg.h"
#include "structs.h"

Triangle t;
void initDraw(void) {
    t = (Triangle){
        .a = {320, 100},
        .b = {100, 540},
        .c = {540, 540}};
}

void prepareScene(void) {
    // next 2 lines are basically a background color refresh
    SDL_SetRenderDrawColor(app.renderer, 0x3a, 0x3a, 0x3a, 0xff);
    SDL_RenderClear(app.renderer);

    SDL_SetRenderDrawColor(app.renderer, 0xff, 0xff, 0xff, 0xff);
    drawTriangle(&t);
}

void presentScene(void) {
    SDL_RenderPresent(app.renderer);
}

void drawTriangle(Triangle *t) {
    SDL_RenderDrawLine(app.renderer, t->a.x, t->a.y, t->b.x, t->b.y);
    SDL_RenderDrawLine(app.renderer, t->b.x, t->b.y, t->c.x, t->c.y);
    SDL_RenderDrawLine(app.renderer, t->c.x, t->c.y, t->a.x, t->a.y);
}