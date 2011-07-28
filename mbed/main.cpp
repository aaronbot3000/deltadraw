#include "mbed.h"

#include "common.h"
#include "servo.h"
#include "positioner.h"
#include "planner.h"
#include "i2c.h"
#include "patterns.h"

#define moves_z (MAX(draw_z - 0.5, MIN_Z - 0.5))

#define UPDATE_INTERVAL 10000
#define PERIP_X 0x90
#define PERIP_Y 0x80

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

static S08 at_draw = 0, debounce = 0;
void read_dials() {
    F32 adj_x = 0, adj_y = 0;
    Point next;
    U08 in_x, in_y, zch = 0;
        
    next = planner.next_pos;
        
    // Debounce the switches
    if (go_adj_z && debounce < 20) {
        debounce++;
        if (debounce == 20) {
            if (at_draw) {
                next.z = moves_z;
                at_draw = 0;
            }
            else {
                next.z = draw_z;
                at_draw = 1;
            }
            zch = 1;
        }
    }
    if (!go_adj_z && debounce > 0)
        debounce--;
    
    in_x = i2c_read(PERIP_X);
    in_y = i2c_read(PERIP_Y);
    if (in_x != 128 && in_x != I2C_ERROR) {
        adj_x = ((F32)RESTRICT(in_x - 128, -16, 16))/64.0;
        //pc.printf("input: %0.5f\n", adj_x);
        next.x += adj_x;
    }
    if (in_y != 128 && in_y != I2C_ERROR) {
        adj_y = ((F32)RESTRICT(in_y - 128, -16, 16))/64.0;
        //pc.printf("input: %0.5f\n", adj_y);
        next.y += adj_y;
    }
    if ((adj_x != 0 ||adj_y != 0 || zch) && get_num_in_buffer(&planner) < 1) {
        //pc.printf("added to buffer");
        add_point_to_buffer(&planner, next);
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
    
    // Clear the peripheral buffers
    i2c_read(PERIP_X);
    i2c_read(PERIP_Y);
    
    runner.attach_us(read_dials, UPDATE_INTERVAL);
    while(1) {
        run_pattern();
        
    }
}