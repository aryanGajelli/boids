#include "boid.h"

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>

#include "cfg.h"
#include "util.h"

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
}

void drawBoid(Boid *t) {
    static SDL_FRect dst = {.w = 10, .h = 25};
    dst.x = t->pos.x - dst.w / 2;
    dst.y = t->pos.y - dst.h / 2;
    SDL_RenderCopyExF(app.renderer, boidTex, NULL, &dst, t->angle, NULL, SDL_FLIP_NONE);
}

void updateBoid(Boid *t) {
    static const float MAX_VEL = 7;
    t->vel.x += t->acc.x;
    t->vel.y += t->acc.y;
    clipVector(&t->vel, MAX_VEL);
    t->pos.x += t->vel.x;
    t->pos.y += t->vel.y;
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

void cohesion(Boid *t){
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

void separation(Boid *t){
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
        // setMag(&steer, );
        clipVector(&steer, MAX_FORCE);
    }
    t->acc.x += steer.x;
    t->acc.y += steer.y;


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
    align(t);
    cohesion(t);
    separation(t);
    respawnAtEdges(t);
}