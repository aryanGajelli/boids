#include "boid.h"

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>

#include "cfg.h"
#include "util.h"

typedef struct LLNode {
    Boid *boid;
    struct LLNode *next;
} LLNode;

#define GRID_SIZE (100)
constexpr size_t GRID_WIDTH = (size_t)WIDTH / GRID_SIZE;
constexpr size_t GRID_HEIGHT = (size_t)HEIGHT / GRID_SIZE;
Boid *grid[GRID_WIDTH][GRID_HEIGHT];

void initBoids(void) {
    float maxInitVel = 3;
    for (int i = 0; i < NUM_BOIDS; i++) {
        boids[i].pos.x = randFloat(0, WIDTH);
        boids[i].pos.y = randFloat(0, HEIGHT);
        boids[i].vel.x = randFloat(-maxInitVel, maxInitVel);
        boids[i].vel.y = randFloat(-maxInitVel, maxInitVel);
        boids[i].acc.x = 0;
        boids[i].acc.y = 0;
        boids[i].angle = atan2(boids[i].vel.y, boids[i].vel.x) * 180 / M_PI + 90;
    }
    memset(grid, 0, sizeof(grid));
}

void updateGrid() {
    // clear grid and boids' next pointers
    memset(grid, 0, sizeof(grid));
    for (int i = 0; i < NUM_BOIDS; i++){
        boids[i].next = NULL;
    }
    // update grid
    for (int i = 0; i < NUM_BOIDS; i++) {
        // get boid location on grid
        size_t x = (size_t)boids[i].pos.x / GRID_SIZE;
        size_t y = (size_t)boids[i].pos.y / GRID_SIZE;
        if (x >= GRID_WIDTH) x = GRID_WIDTH - 1;
        if (y >= GRID_HEIGHT) y = GRID_HEIGHT - 1;
        // add boid to grid
        if (grid[x][y] == NULL) {
            grid[x][y] = &boids[i];
            grid[x][y]->next = NULL;
        } else {
            boids[i].next = grid[x][y];
            grid[x][y] = &boids[i];
        }
    }
}

void drawBoid(Boid *t) {
    static SDL_FRect dst = {.w = 10, .h = 25};
    dst.x = t->pos.x - dst.w / 2;
    dst.y = t->pos.y - dst.h / 2;
    SDL_RenderCopyExF(app.renderer, boidTex, NULL, &dst, t->angle, NULL, SDL_FLIP_NONE);
}

void updateBoid(Boid *t) {
    static const float MAX_VEL = 5;
    t->vel.x += t->acc.x * dT;
    t->vel.y += t->acc.y * dT;
    clipVector(&t->vel, MAX_VEL);
    t->pos.x += t->vel.x * dT;
    t->pos.y += t->vel.y * dT;
    t->angle = atan2(t->vel.y, t->vel.x) * 180 / M_PI + 90;
    t->acc.x = 0;
    t->acc.y = 0;
}

void align(Boid *t) {
    static const float ALIGN_RADIUS = 50;
    static const float MAX_FORCE = 0.1;
    SDL_FPoint steer = {0, 0};
    int total = 0;
    for (int i = 0; i < NUM_BOIDS; i++) {
        if (&boids[i] != t) {
            float dx = t->pos.x - boids[i].pos.x;
            float dy = t->pos.y - boids[i].pos.y;
            float d_sqr = dx * dx + dy * dy;
            if (d_sqr < ALIGN_RADIUS * ALIGN_RADIUS) {
                steer.x += boids[i].vel.x;
                steer.y += boids[i].vel.y;
                total++;
            }
        }
    }

    if (total > 0) {
        steer.x /= total;
        steer.y /= total;
        steer.x -= t->vel.x;
        steer.y -= t->vel.y;
        clipVector(&steer, MAX_FORCE);
    }
    t->acc.x += steer.x;
    t->acc.y += steer.y;
}

void cohesion(Boid *t) {
    static const float COHESION_RADIUS = 100;
    static const float MAX_FORCE = 0.1;
    SDL_FPoint steer = {0, 0};
    int total = 0;
    for (int i = 0; i < NUM_BOIDS; i++) {
        if (&boids[i] != t) {
            float dx = t->pos.x - boids[i].pos.x;
            float dy = t->pos.y - boids[i].pos.y;
            float d_sqr = dx * dx + dy * dy;
            if (d_sqr < COHESION_RADIUS * COHESION_RADIUS) {
                steer.x += boids[i].pos.x;
                steer.y += boids[i].pos.y;
                total++;
            }
        }
    }

    if (total > 0) {
        steer.x /= total;
        steer.y /= total;
        steer.x -= t->pos.x;
        steer.y -= t->pos.y;
        // setMag(&steer, );
        clipVector(&steer, MAX_FORCE);
    }
    t->acc.x += steer.x;
    t->acc.y += steer.y;
}

void separation(Boid *t) {
    static const float SEPARATION_RADIUS = 30;
    static const float MAX_FORCE = 0.5;
    SDL_FPoint steer = {0, 0};
    int total = 0;
    for (int i = 0; i < NUM_BOIDS; i++) {
        if (&boids[i] != t) {
            float dx = t->pos.x - boids[i].pos.x;
            float dy = t->pos.y - boids[i].pos.y;
            float d_sqr = dx * dx + dy * dy;
            if (d_sqr < SEPARATION_RADIUS * SEPARATION_RADIUS) {
                steer.x += dx;
                steer.y += dy;
                total++;
            }
        }
    }

    if (total > 0) {
        steer.x /= total;
        steer.y /= total;
        clipVector(&steer, MAX_FORCE);
    }
    t->acc.x += steer.x;
    t->acc.y += steer.y;
}

void combined(Boid *t) {
    static const float ALIGN_RADIUS = 50, COHESION_RADIUS = 50, SEPARATION_RADIUS = 30;
    static const float MAX_FORCE_A = 0.1, MAX_FORCE_C = 0.1, MAX_FORCE_S = 0.5;
    SDL_FPoint steerA = {0, 0}, steerC = {0, 0}, steerS = {0, 0};
    int totalA = 0, totalC = 0, totalS = 0;

    size_t x = (size_t)t->pos.x / GRID_SIZE;
    size_t y = (size_t)t->pos.y / GRID_SIZE;
    if (x >= GRID_WIDTH) x = GRID_WIDTH - 1;
    if (y >= GRID_HEIGHT) y = GRID_HEIGHT - 1;
    
    // check 9 cells around boid
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            size_t x_ = x + i;
            size_t y_ = y + j;
            if (x_ >= GRID_WIDTH || y_ >= GRID_HEIGHT) continue;
            Boid *curr = grid[x_][y_];
            while (curr != NULL) {
                Boid boid = *curr;
                curr = curr->next;
                if (&boid == t) continue;
                float dx = t->pos.x - boid.pos.x;
                float dy = t->pos.y - boid.pos.y;
                float d_sqr = dx * dx + dy * dy;
                if (d_sqr < COHESION_RADIUS * COHESION_RADIUS) {
                    steerC.x += boid.pos.x;
                    steerC.y += boid.pos.y;
                    totalC++;
                } else
                    continue;
                if (d_sqr < ALIGN_RADIUS * ALIGN_RADIUS) {
                    steerA.x += boid.vel.x;
                    steerA.y += boid.vel.y;
                    totalA++;
                } else
                    continue;
                if (d_sqr < SEPARATION_RADIUS * SEPARATION_RADIUS) {
                    steerS.x += dx;
                    steerS.y += dy;
                    totalS++;
                }
            }
        }
    }

    if (totalA > 0) {
        steerA.x /= totalA;
        steerA.y /= totalA;
        steerA.x -= t->vel.x;
        steerA.y -= t->vel.y;
        clipVector(&steerA, MAX_FORCE_A);
    }
    if (totalC > 0) {
        steerC.x /= totalC;
        steerC.y /= totalC;
        steerC.x -= t->pos.x;
        steerC.y -= t->pos.y;
        clipVector(&steerC, MAX_FORCE_C);
    }
    if (totalS > 0) {
        steerS.x /= totalS;
        steerS.y /= totalS;
        clipVector(&steerS, MAX_FORCE_S);
    }
    t->acc.x += steerA.x + steerC.x + steerS.x;
    t->acc.y += steerA.y + steerC.y + steerS.y;
}

void respawnAtEdges(Boid *t) {
    if (t->pos.x < 0) {
        t->pos.x = WIDTH;
    } else if (t->pos.x > WIDTH) {
        t->pos.x = 0;
    }
    if (t->pos.y < 0) {
        t->pos.y = HEIGHT;
    } else if (t->pos.y > HEIGHT) {
        t->pos.y = 0;
    }
}

void flock(Boid *t) {
    // align(t);
    // cohesion(t);
    // separation(t);
    combined(t);
    respawnAtEdges(t);
    updateBoid(t);
}

void flock_threaded(void *varg) {
    Boid *t = (Boid *)varg;
    combined(t);
    respawnAtEdges(t);
    updateBoid(t);
}