#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <SDL.h>

typedef struct Boid {
    float angle;
    SDL_FPoint pos;
    SDL_FPoint vel;
    SDL_FPoint acc;
    struct Boid* next;
} Boid;

extern SDL_Texture* boidTex;
#define NUM_BOIDS (500)
extern Boid boids[NUM_BOIDS];

void initBoids(void);
void updateGrid();
void drawBoid(Boid* t);
void updateBoid(Boid* t);
void flock(Boid *t);
void flock_threaded(void* varg);
#endif  // __GEOMETRY_H__