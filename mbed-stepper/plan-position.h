#ifndef PLAN_POSITION_H
#define PLAN_POSITION_H

#include "common.h"
#include "kinematics.h"
#include "stepper.h"
#include <cmath>

#define BUFFER_SIZE 256
#define INC_ONE(a) (((a) + 1) % BUFFER_SIZE)

#define MAX_STEP_SIZE   0.040
#define MIN_STEP_SIZE   0.0005
#define MIN_STEP_FRAC   3


#define MIN_DIST 0.002

#define ACCL_ZONE 0.5 // inches

#define TIMING_INTERVAL 2500

enum Planner_State {
    PLR_ACCL,
    PLR_FULL,
    PLR_DECL,
    PLR_NEXT,
    PLR_WAIT
};

struct Planner {
    volatile Point buffer[BUFFER_SIZE];
    volatile U32 buf_ind;
    volatile U32 buf_next;

    Planner_State state;
    S32 steps_to_next;
    
    Ticker runner;
    
    F32 angles_step[3];
    F32* angles_actual;
    F32 angles_ideal[3];
    Point current_pos;

    F32 full_dist;
    F32 prev_dist;

    volatile bool finished;
    volatile bool errored;
};

void go_to_home(Planner* planner);
Status wait_for_pattern(Planner* planner);
void setup_planner(Planner* planner);
bool add_point_to_buffer(Planner* planner, Point in);
void pause_steppers(Planner* planner);
void resume_steppers(Planner* planner);
void take_step();

#endif
