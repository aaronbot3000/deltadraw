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
    
    Point a, b, c, d, e, f, g;
    /*
    a.x =  3; a.y =  3; a.z = 11;
    b.x = -3; b.y = -3; b.z = 8;
    c.x =  3; c.y = -3; c.z = 9;
    d.x = -3; d.y =  3; d.z = 11;
    */
    g.x =  0; g.y =  2; g.z = 7.5;
    a.x =  0; a.y =  2; a.z = 8.05;
    b.x =  2; b.y = -2; b.z = 8.05;
    c.x = -2; c.y =  1; c.z = 8.05;
    d.x =  2; d.y =  1; d.z = 8.05;
    e.x = -2; e.y = -2; e.z = 8.05;
    f.x =  0; f.y =  0; f.z = 7.5;
    
    add_point_to_buffer(&planner, g);
    add_point_to_buffer(&planner, a);
    add_point_to_buffer(&planner, b);
    add_point_to_buffer(&planner, c);
    add_point_to_buffer(&planner, d);
    add_point_to_buffer(&planner, e);
    add_point_to_buffer(&planner, a);
    add_point_to_buffer(&planner, g);
    add_point_to_buffer(&planner, f);
        
    S32 counter = 0;
    wait(1);
    while (1) {
        //if (counter % 128 == 0)
        //    pc.printf("%.2f, %.2f, %.2f\n", planner.current_pos.x, planner.current_pos.y, planner.current_pos.z);
        planner_process(&planner);
        counter++;
    }
}