#ifndef DIALS_H
#define DIALS_H

#include "i2c.h"
#include "planner.h"
#include "common.h"

#define PERIP_X 0x90
#define PERIP_Y 0x80

void read_dials(Planner* planner, F32 moves_z, F32 draw_z);

#endif