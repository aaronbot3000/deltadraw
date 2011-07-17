#ifndef SERVO_H
#define SERVO_H

#include "mbed.h"

#include "common.h"

#define SERVO_MIN_PULSE 760
#define SERVO_MAX_PULSE 2200

#define SERVO_MIN_ANGLE -1.01922088
#define SERVO_MAX_ANGLE  1.49077498

typedef enum {
    SERVO_0,
    SERVO_120,
    SERVO_240
} Servo;

#ifdef DEBUG
extern Serial pc;
#endif

void servos_setup();
Status servo_set_angle(Servo servo, F32 angle);

#endif