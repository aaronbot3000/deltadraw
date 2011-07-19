#ifndef PLANNER_H
#define PLANNER_H

#include "common.h"
#include "positioner.h"

#define PLANNER_BUFFER_SIZE 10

#define MAX_STEP_SIZE   0.0025
#define MIN_STEP_SIZE   0.0005

#define ACCL_ZONE 1 // inches

#define INC_ONE(a) (((a) + 1) % PLANNER_BUFFER_SIZE)

enum Planner_State {
    PLR_ACCL,
    PLR_FULL,
    PLR_DECL,
    PLR_NEXT,
    PLR_WAIT
};

struct Planner {
    Point buffer[PLANNER_BUFFER_SIZE];
    
    S08 current;
    S08 next;
    
    Point current_pos;
    F32 dx;
    F32 dy;
    F32 dz;
    
    F32 prev_dist;
    F32 full_dist;
    
    Planner_State state;
};

#ifdef DEBUG
extern Serial pc;
#endif

void planner_setup(Planner* planner);
Status add_point_to_buffer(Planner* planner, Point in);
Status planner_process(Planner* planner);

#endif
