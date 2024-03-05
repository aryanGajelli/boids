#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <SDL.h>

typedef struct App {
    SDL_Window *win;
    SDL_Renderer *renderer;
} App;

typedef struct Triangle {
    SDL_FPoint a;
    SDL_FPoint b;
    SDL_FPoint c;
} Triangle;

#endif  // __STRUCTS_H__