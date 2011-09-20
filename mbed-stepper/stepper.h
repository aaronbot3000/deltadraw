#ifndef STEPPER_H
#define STEPPER_H

#include "mbed.h"
#include "common.h"

#define STEPPER_MIN_ANGLE -1.01922088
#define STEPPER_MAX_ANGLE  1.49077498
#define STEPPER_STEP_SIZE  0.00331612558

#define STEPPER_UP 1
#define STEPPER_DN 0

#ifdef DEBUG
extern Serial pc;
#endif

void reset_steppers();
void update_pos();
F32* get_angles();
Status move_steppers(int steppers, int directions);

#endif
