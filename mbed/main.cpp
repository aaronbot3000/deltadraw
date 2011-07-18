#include "mbed.h"

#include "common.h"
#include "servo.h"
#include "positioner.h"
#include "planner.h"

Serial pc(USBTX, USBRX); // tx, rx

int main() {
    Planner planner;
    
    servos_setup();
    planner_setup(&planner);
    
    Point a, b, c, d;
    a.x =  2; a.y =  2; a.z = 8;
    b.x = -2; b.y =  2; b.z = 9;
    c.x = 2; c.y = -2; c.z = 8;
    d.x =  2; d.y = -2; d.z = 10;
    
    planner.buffer[0] = a;
    planner.buffer[1] = b;
    planner.buffer[2] = c;
    planner.buffer[3] = d;
    
    planner.current = 0;
    planner.next = 9;
    planner.current_pos = a;
    
    S32 counter = 0;
    while (1) {
        //if (counter % 128 == 0)
        //    pc.printf("%.2f, %.2f, %.2f\n", planner.current_pos.x, planner.current_pos.y, planner.current_pos.z);
        planner_process(&planner);
        counter++;
    }
}