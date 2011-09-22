#ifndef KINEMATICS_H
#define KINEMATICS_H

#include "common.h"
#include <cstdio>
#include <cmath>

#define COS_30   0.866025404
#define COS_60   0.5
#define COS_120 -0.5
#define COS_240 -0.5

#define SIN_60   0.866025404
#define SIN_120  0.866025404
#define SIN_240 -0.866025404

#define TAN_30   1.73205081
#define TAN_60   0.577350269

Status inv_kinematics(F32* result, Point target);
Status fwd_kinematics(Point* target, F32* angles);

#endif
