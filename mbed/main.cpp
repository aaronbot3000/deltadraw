#include "mbed.h"

#include "common.h"
#include "servo.h"
#include "positioner.h"
#include "planner.h"
#include "i2c.h"
#include "patterns.h"

#define moves_height (MAX(draw_height - 0.5, MIN_HEIGHT - 0.5))

Serial pc(USBTX, USBRX); // tx, rx

// trololololol
DigitalIn go_troll_up(p20);
DigitalIn go_troll_dn(p19);
DigitalIn go_adj_height(p18);

DigitalIn go_run_pat(p17);

static Planner planner;
static F32 draw_height = MIN_HEIGHT; // inches

void setup() {
    servos_setup();
    int ret = planner_setup(&planner);
    pc.printf("Setup: %d\n", ret);
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
    
    // main while loop
    while (1) {
        if (go_adj_height) {
            adj_height();
        }
        else if (go_run_pat) {
            //clear_buffer(&planner);
            //draw_square_nn(moves_height, draw_height, &planner);
            draw_star(moves_height, draw_height, &planner);

            run_pattern();
        }
        else {
            //pc.printf("Restting\n");
            int ret = reset_position(&planner);
            //pc.printf("resetting: %d\n", ret);
        }
    }
}