#ifndef I2C_H_
#define I2C_H_

#include <msp430g2452.h>

#define INVALID_POS 0x7F

void i2c_setup();
void set_i2c_data(char in);

#endif /*I2C_H_*/
