#ifndef PLAN_POSITION_H
#define PLAN_POSITION_H

#include "common.h"
#include "kinematics.h"
#include <cmath>

#define MAX_STEP_SIZE   1
#define MIN_STEP_SIZE   0.1

#define MIN_DIST 0.005

#define EST_STEP_SIZE 0.05 // inches

#define ACCL_ZONE .5 // inches

enum Planner_State {
    PLR_ACCL,
    PLR_FULL,
    PLR_DECL,
    PLR_NEXT,
    PLR_WAIT
};

void catch_interrupt();
void set_goal(Point in);
void take_step();

#endif
