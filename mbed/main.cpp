#include "mbed.h"

#include "common.h"
#include "servo.h"
#include "positioner.h"

Serial pc(USBTX, USBRX); // tx, rx

int main() {
    Timer timer;
    Point pos;
    Status ret;
    servos_setup();
    pos.x = 1;
    pos.y = 0;
    pos.z = 7.5;
    F32 height = 9.5;
    while (1) {
    /*
        pos.x = 2;
        pos.y = 0;
        pos.z = 9.5;
        
        set_position(pos);
        */
        
        for (F32 x=-2; x<2; x+=.001) {
           pos.x = x;
           pos.y = 2;
           pos.z = height;
           timer.start();
           ret = set_position(pos);
           timer.stop();
           //pc.printf("Returned %d in %d us\n", ret, timer.read_us());
           timer.reset();
           //wait_ms(100);
        }
        for (F32 x=2; x>-2; x-=.001) {
           pos.x = 2;
           pos.y = x;
           pos.z = height;
           timer.start();
           ret = set_position(pos);
           timer.stop();
           //pc.printf("Returned %d in %d us\n", ret, timer.read_us());
           timer.reset();
           //wait_ms(100);
        }
        for (F32 x=2; x>-2; x-=.001) {
           pos.x = x;
           pos.y = -2;
           pos.z = height;
           timer.start();
           ret = set_position(pos);
           timer.stop();
           //pc.printf("Returned %d in %d us\n", ret, timer.read_us());
           timer.reset();
           //wait_ms(100);
        }
        
        for (F32 x=-2; x<2; x+=.001) {
           pos.x = -2;
           pos.y = x;
           pos.z = height;
           timer.start();
           ret = set_position(pos);
           timer.stop();
           //pc.printf("Returned %d in %d us\n", ret, timer.read_us());
           timer.reset();
           //wait_ms(100);
        }
        /*
         for (F32 x=2; x>-2; x-=.001) {
           pos.x = -2;
           pos.y = x;
           pos.z = 10.5;
           timer.start();
           ret = set_position(pos);
           timer.stop();
           //pc.printf("Returned %d in %d us\n", ret, timer.read_us());
           timer.reset();
           //wait_ms(100);
        }
        */
    }
}