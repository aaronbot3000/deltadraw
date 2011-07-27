#ifndef WHEEL_H_
#define WHEEL_H_

#include "CTS_Layer.h"

#define MASK7                   BIT4
#define MASK6                   BIT5

#define HYSTERESIS 3

#define ABS(a) ((a) < 0 ? -(a) : (a))

void wheel_setup();
char get_wheel_reading();

#endif /*WHEEL_H_*/
