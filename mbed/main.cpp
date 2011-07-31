#include "mbed.h"

#include "common.h"
#include "servo.h"
#include "positioner.h"
#include "planner.h"
#include "i2c.h"
#include "patterns.h"
#include "dials.h"

#define moves_z (MAX(draw_z - 0.5, MIN_Z - 0.5))
#define UPDATE_INTERVAL 10000
#define START_TRANS 'B'
#define END_TRANS 0xFFFF1111

Serial pc(USBTX, USBRX); // tx, rx
Ticker runner;
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

volatile U08 serial_buffer[16];
volatile S32 sbuffer_index;

// trololololol
DigitalIn go_troll_up(p20);
DigitalIn go_troll_dn(p19);
DigitalIn go_adj_z(p18);

DigitalIn go_run_pat(p17);

static Planner planner;
static F32 draw_z = MIN_Z; // inches

void serial_callback() {
    serial_buffer[sbuffer_index++] = pc.getc();
}

void setup() {
    servos_setup();
    int ret = planner_setup(&planner);
    pc.baud(115200);
    pc.attach(serial_callback);
    sbuffer_index = 0;
    //pc.printf("Setup: %d\n", ret);
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

void read_dials_callback() {
    read_dials(&planner, moves_z, draw_z);
}

void run_pattern() {
    Status status = SUCCESS;
    while (status == SUCCESS) {
        status = planner_process(&planner);
    }
}

Status fill_buffer() {
    int i;
    Point in;
    
    pc.putc(START_TRANS);
    for (i = 0; i < PLANNER_BUFFER_SIZE - 5; i++) {
        while (sbuffer_index < 9);
        if ((*(U32*)(&serial_buffer[0])) == END_TRANS)
            return END_PAT;
        sbuffer_index = 0;
        pc.putc(START_TRANS);
        
        in.x = *(F32*)(&serial_buffer[0]);
        in.y = *(F32*)(&serial_buffer[4]);
        
        if (serial_buffer[8] == 1)
            in.z = moves_z;
        else
            in.z = draw_z;
        add_point_to_buffer(&planner, in);        
    }
    
    return SUCCESS;
}
       

int main() {
    Status status;
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
            reset_position(&planner);
        }
    }
    
    wait_ms(500);
    
    while (1) {

        runner.attach_us(read_dials_callback, UPDATE_INTERVAL);
        while(1) {
            run_pattern();
            if (go_run_pat)
                break;
        }
        
        runner.detach();
        
        status = SUCCESS;
        sbuffer_index = 0;
        while (status == SUCCESS) {
            status = fill_buffer();
            run_pattern();
        }
        pc.putc('D');
    }
}
