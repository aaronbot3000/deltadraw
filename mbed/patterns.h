#ifndef PATTERNS_H
#define PATTERNS_H

#include "planner.h"
#include "common.h"

void draw_square_large(F32 moves_height, F32 draw_height, Planner* planner);
void draw_star(F32 moves_height, F32 draw_height, Planner* planner);
void draw_square_nn(F32 moves_height, F32 draw_height, Planner* planner);
void draw_ti(F32 moves_height, F32 draw_height, Point off, Planner* planner);

#endif