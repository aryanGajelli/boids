#ifndef __DRAW_H__
#define __DRAW_H__

#include "structs.h"
void initDraw(void);

void prepareScene(void);

void presentScene(void);

void drawTriangle(Triangle* t);

#endif  // __DRAW_H__
