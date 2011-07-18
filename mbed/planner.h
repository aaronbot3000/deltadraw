#ifndef PLANNER_H
#define PLANNER_H

#include "common.h"
#include "positioner.h"

#define PLANNER_BUFFER_SIZE 4

#define MAX_STEP_SIZE   0.002
#define MIN_STEP_SIZE   0.001

#define INC_ONE(a) (((a) + 1) % PLANNER_BUFFER_SIZE)

struct Planner {
    Point buffer[PLANNER_BUFFER_SIZE];
    S08 current;
    S08 next;
    Point current_pos;
    F32 dx;
    F32 dy;
    F32 dz;
};

#ifdef DEBUG
extern Serial pc;
#endif

void planner_setup(Planner* planner);
Status add_point_to_buffer(Planner* planner, Point in);
Status planner_process(Planner* planner);

#endif
