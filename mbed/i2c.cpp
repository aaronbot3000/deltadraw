#include "i2c.h"

static I2C i2c(p28, p27);        // sda, scl

S32 i2c_read(S32 addr) {
    S32 ret_val;
    i2c.start();
    i2c.write(addr + 1);
    
    // Acknowledge the incoming byte
    ret_val = i2c.read(1);
    
    i2c.stop();
    return ret_val;
}