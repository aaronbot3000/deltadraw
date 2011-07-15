#include "servo.h"

static PwmOut servos[3] = {PwmOut(p21), PwmOut(p22), PwmOut(p23)};

void servo_setup() {
    servos[0].period(0.020);
    servos[1].period(0.020);
    servos[2].period(0.020);
}

Status servo_set_angle(Servo servo, S32 angle) {
    if (angle > SERVO_MAX_ANGLE || angle < SERVO_MIN_ANGLE)
        return FAILURE;
        
    // Map the servo's value in degrees to the value in usec
    F32 us = ((F32)(angle - SERVO_MIN_ANGLE) / (F32)(SERVO_MAX_ANGLE - SERVO_MIN_ANGLE)) *
               (SERVO_MAX_PULSE - SERVO_MIN_PULSE) + SERVO_MIN_PULSE;
    servos[servo].pulsewidth_us((S32)us);
    return SUCCESS;
}