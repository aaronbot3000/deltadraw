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
    S08 a = i2c_read(PERIP_X);
    if (a != I2C_NO_MOTION) {
        //pc.printf("input: %.5f\n", (F32)a);
        nudge_x(&planner, ((F32)a)/100.0);
    }
}

int main() {
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
    
    runner.attach_us(read_dials, UPDATE_INTERVAL);
    while(1) {
        run_pattern();
    }
}