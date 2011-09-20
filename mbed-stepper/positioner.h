#ifndef POSITIONER_H
#define POSITIONER_H

#include "common.h"
#include "stepper.h"

#define SIN_120  0.866025404
#define COS_120 -0.5
#define SIN_240 -0.866025404
#define COS_240 -0.5

#ifdef DEBUG
extern Serial pc;
#endif

// Returns if possible to set position to this location
Status set_position(Point coord);
Point position();

#endif