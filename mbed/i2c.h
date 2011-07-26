#ifndef I2C_H
#define I2C_H

#include "mbed.h"
#include "common.h"

#define I2C_ERROR 0xFFFFFFFF
#define I2C_NO_MOTION 0x7F

#define WHEEL1_ADDR 0x90

S32 i2c_read(S32 address);

#endif