#ifndef PLANNER_H
#define PLANNER_H

#include "common.h"
#include "positioner.h"

#define PLANNER_BUFFER_SIZE 16

#define MAX_STEP_SIZE   0.0040
#define MIN_STEP_SIZE   0.0001

#define ACCL_ZONE .75 // inches


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
};

#ifdef DEBUG
extern Serial pc;
#endif

void planner_setup(Planner* planner);
Status reset_position(Planner* planner);

Status add_point_to_buffer(Planner* planner, Point in);
void clear_buffer(Planner* planner);
Status goto_point(Planner* planner, F32 x, F32 y, F32 z);
Status goto_point(Planner* planner, Point goal);
Status planner_process(Planner* planner);

Status troll_up(Planner* planner);
Status troll_down(Planner* planner);

#endif
