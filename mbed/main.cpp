#include "mbed.h"

#include "common.h"
#include "servo.h"

#define I2C_ERROR 0xffffffff

I2C i2c(p28, p27);        // sda, scl
Serial pc(USBTX, USBRX); // tx, rx
DigitalOut hb(LED1);

PwmOut servo1(p21);
PwmOut servo2(p22);
PwmOut servo3(p23);

const int addr = 0x91; // define the I2C Address

int main() {
    pc.printf("sizeof float: %d %d %d\n", sizeof(int), sizeof(short), sizeof(long long));
    S08 ret;
    S16 pw, d;
    i2c.frequency(100000);
    hb = 1;
    pw = -90;
    d = 1;

    servo_setup();

    while (1) {
        servo_set_angle(SERVO_0,   pw);
        servo_set_angle(SERVO_120, pw);
        servo_set_angle(SERVO_240, pw);

        if (pw >= 90)
            d = -1;
        if (pw <= -90)
            d = 1;


        i2c.start();
        i2c.write(addr);
        ret = i2c.read(true);
        i2c.stop();

        pc.printf("return %x, %x, %d\n", ret == I2C_ERROR, ret, pw);
        wait(1);

        hb = hb ^ 1;
        pw += d;
    }
}