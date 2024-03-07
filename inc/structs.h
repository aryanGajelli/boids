#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <SDL.h>

typedef struct App {
    SDL_Window *win;
    SDL_Renderer *renderer;
} App;

#endif  // __STRUCTS_H__