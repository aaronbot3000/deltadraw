#include "mbed.h"

#include "common.h"
#include "servo.h"
#include "positioner.h"
#include "planner.h"
#include "i2c.h"
#include "patterns.h"

#define moves_z (MAX(draw_z - 0.5, MIN_Z - 0.5))

#define UPDATE_INTERVAL 1000
#define PERIP_X 0x90

Serial pc(USBTX, USBRX); // tx, rx
Ticker runner;

// trololololol
DigitalIn go_troll_up(p20);
DigitalIn go_troll_dn(p19);
DigitalIn go_adj_z(p18);

DigitalIn go_run_pat(p17);

static Planner planner;
static F32 draw_z = MIN_Z; // inches

void setup() {
    servos_setup();
    int ret = planner_setup(&planner);
    pc.printf("Setup: %d\n", ret);
}

void adj_z() {
    goto_point(&planner, 0, 0, draw_z);
    
    if (go_troll_up) {
        troll_up(&planner);
        draw_z = planner.current_pos.z;
    }
    if (go_troll_dn) {
        troll_down(&planner);
        draw_z = planner.current_pos.z;
    }
}

void run_pattern() {
    Status status = SUCCESS;

    while (status == SUCCESS) {
        status = planner_process(&planner);
    }
}

void read_dials() {
    F32 adj;
    U08 a = i2c_read(PERIP_X);
    if (a != 128 && a != I2C_ERROR) {
        adj = ((F32)a - 128)/100.0;
        //pc.printf("input: %0.5f\n", adj);
        nudge_x(&planner, adj);
    }
}

int main() {
    S08 at_draw = 0, debounce = 0;
    setup();

    // adjust z
    while (1) {
        if (go_adj_z) {
            adj_z();
        }
        else if (go_run_pat) {
            break;
        }
        else {
            //pc.printf("Restting\n");
            int ret = reset_position(&planner);
            //pc.printf("resetting: %d\n", ret);
        }
    }
    
    
    while(1) {
        if (go_adj_z && debounce < 20) {
            debounce++;
            if (debounce == 20) {
                if (at_draw) {
                    goto_point(&planner, planner.current_pos.x, planner.current_pos.y, moves_z);
                    at_draw = 0;
                }
                else {
                    goto_point(&planner, planner.current_pos.x, planner.current_pos.y, draw_z);
                    at_draw = 1;
                }
                i2c_read(PERIP_X);
            }
        }
        if (!go_adj_z && debounce > 0)
            debounce--;
        read_dials();
    }
}