#ifndef SERVO_H
#define SERVO_H

#include "mbed.h"

#include "common.h"

#define SERVO_MIN_PULSE 760
#define SERVO_MAX_PULSE 2200

#define SERVO_MIN_ANGLE -90
#define SERVO_MAX_ANGLE 90

typedef enum {
    SERVO_0,
    SERVO_120,
    SERVO_240
} Servo;

void servo_setup();
Status servo_set_angle(Servo servo, S32 angle);

#endif