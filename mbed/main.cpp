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
   while(1) {
       for (F32 x=-2; x<2; x+=.001) {
           pos.x = x;
           pos.y = 2;
           pos.z = 10;
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
           pos.z = 10;
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
           pos.z = 10;
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
           pos.z = 10;
           timer.start();
           ret = set_position(pos);
           timer.stop();
           //pc.printf("Returned %d in %d us\n", ret, timer.read_us());
           timer.reset();
           //wait_ms(100);
       }
   }
}