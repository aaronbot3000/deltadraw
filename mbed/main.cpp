#include "mbed.h"

#include "common.h"
#include "servo.h"
#include "positioner.h"
#include "planner.h"

#define moves_height (MAX(draw_height - 1, MIN_HEIGHT - 0.5))

Serial pc(USBTX, USBRX); // tx, rx

// trololololol
DigitalIn go_troll_up(p16);
DigitalIn go_troll_dn(p18);
DigitalIn go_adj_height(p19);

DigitalIn go_run_pat(p20);

static Planner planner;
static F32 draw_height = MIN_HEIGHT; // inches

void setup() {
    servos_setup();
    planner_setup(&planner);
}

void adj_height() {
    goto_point(&planner, 0, 0, draw_height);
    
    if (go_troll_up) {
        troll_up(&planner);
        draw_height = planner.current_pos.z;
    }
    if (go_troll_dn) {
        troll_down(&planner);
        draw_height = planner.current_pos.z;
    }
}

void run_pattern() {
    Status status = SUCCESS;

    while (status == SUCCESS) {
        status = planner_process(&planner);
    }
}

int main() {
    setup();

    Point a, b, c, d, e, f, g;
    /*
    a.x =  3; a.y =  3; a.z = 11;
    b.x = -3; b.y = -3; b.z = 8;
    c.x =  3; c.y = -3; c.z = 9;
    d.x = -3; d.y =  3; d.z = 11;
    */

    // main while loop
    while (1) {
        if (go_adj_height) {
            adj_height();
        }
        else if (go_run_pat) {
            clear_buffer(&planner);
            g.x =  0;
            g.y =  2;
            g.z = moves_height;
            a.x =  0;
            a.y =  2;
            a.z = draw_height;
            b.x =  2;
            b.y = -2;
            b.z = draw_height;
            c.x = -2;
            c.y =  1;
            c.z = draw_height;
            d.x =  2;
            d.y =  1;
            d.z = draw_height;
            e.x = -2;
            e.y = -2;
            e.z = draw_height;
            f.x =  0;
            f.y =  0;
            f.z = moves_height;

            add_point_to_buffer(&planner, g);
            add_point_to_buffer(&planner, a);
            add_point_to_buffer(&planner, b);
            add_point_to_buffer(&planner, c);
            add_point_to_buffer(&planner, d);
            add_point_to_buffer(&planner, e);
            add_point_to_buffer(&planner, a);
            add_point_to_buffer(&planner, g);
            add_point_to_buffer(&planner, f);

            run_pattern();
        }
        else {
            //pc.printf("Restting\n");
            reset_position(&planner);
        }
    }
}