#ifndef __UTIL_H__
#define __UTIL_H__
#include <SDL.h>
float randFloat(float a, float b);
void setMag(SDL_FPoint* v, float mag);

void clipVector(SDL_FPoint* v, float max);

#endif  // __UTIL_H__
