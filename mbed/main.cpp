#include "mbed.h"

I2C i2c(p28, p27);        // sda, scl
Serial pc(USBTX, USBRX); // tx, rx
DigitalOut hb(LED1);

const int addr = 0x90; // define the I2C Address

int main() {
    char cmd[2];
    int8_t retcode;
    memset(cmd, 0, sizeof(char) * 2);
    i2c.frequency(100000);
    hb = 1;

    while(1) {
        i2c.start();
        i2c.write(0x91);
        retcode = i2c.read(true);
        i2c.stop();


        pc.printf("return %x, %x\n", retcode, cmd[0]);
        wait(2);

        hb = hb ^ 1;
    }
}
