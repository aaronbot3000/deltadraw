#ifndef PATTERNS_H
#define PATTERNS_H

#include "common.h"
#include "plan-position.h"

int draw_star(F32 moves_height, F32 draw_height, Planner* planner);
int draw_square_large(F32 moves_height, F32 draw_height, Planner* planner);
int draw_square_nn(F32 moves_height, F32 draw_height, Planner* planner);
int draw_ti(F32 moves_height, F32 draw_height, Point off, Planner* planner);

#endif
