#include "util.h"

#include <SDL.h>
#include <math.h>

float randFloat(float a, float b) {
    return ((b - a) * ((float)rand() / RAND_MAX)) + a;
}


void setMag(SDL_FPoint *v, float mag) {
    float len = sqrtf(v->x * v->x + v->y * v->y);
    v->x = (v->x / len) * mag;
    v->y = (v->y / len) * mag;
}

void clipVector(SDL_FPoint *v, float max) {
    float len = sqrtf(v->x * v->x + v->y * v->y);
    if (len > max) {
        v->x = (v->x / len) * max;
        v->y = (v->y / len) * max;
    }
}