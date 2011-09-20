#ifndef KINEMATICS_H
#define KINEMATICS_H

#include "common.h"

#define COS_30   0.866025404
#define TAN_60   0.577350269
#define SIN_120  0.866025404
#define COS_120 -0.5
#define SIN_240 -0.866025404
#define COS_240 -0.5

Status inv_kinematics(F32* result, Point target);
Status fwd_kinematics(Point target, F32* angles);

#endif
