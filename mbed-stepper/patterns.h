#ifndef PATTERNS_H
#define PATTERNS_H

#include "common.h"

int draw_star(F32 moves_height, F32 draw_height, Point* planner);
int draw_square_large(F32 moves_height, F32 draw_height, Point* planner);
int draw_square_nn(F32 moves_height, F32 draw_height, Point* planner);
int draw_ti(F32 moves_height, F32 draw_height, Point off, Point* planner);

#endif
