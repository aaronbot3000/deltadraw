#include "mbed.h"

#include "common.h"
#include "plan-position.h"
#include "patterns.h"

#define moves_z (MAX(draw_z - 0.5, MIN_Z - 0.5))
#define UPDATE_INTERVAL 10000
#define START_TRANS 'B'
#define END_TRANS 0xFFFF1111

Serial pc(USBTX, USBRX); // tx, rx
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

volatile U08 serial_buffer[16];
volatile S32 sbuffer_index;

static F32 draw_z = MIN_Z; // inches

Planner planner;

void serial_callback() {
    serial_buffer[sbuffer_index++] = pc.getc();
}

void setup() {
    sbuffer_index = 0;
    pc.baud(115200);
    pc.attach(serial_callback);

    setup_planner(&planner);

    pc.printf("Setup\r\n");
}

Status wait_for_pattern() {
    while (true) {
        if (planner.finished) {
			return SUCCESS;
        }
		if (planner.errored) {
			return FAILURE;
		}
    }
}

Status fill_buffer() {
    int i;
    Point in;
    
    pause_steppers(&planner);
    pc.putc(START_TRANS);
    for (i = 0; i < BUFFER_SIZE - 5; i++) {
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
    resume_steppers(&planner);
    
    return SUCCESS;
}

int main() {
    //Status status;
    setup();

    // adjust z
    /*
    while (1) {
        if (go_adj_z) {
            adj_z();
        }
        else if (go_run_pat) {
            break;
        }
        else {
            reset_pen();
        }
    }
    
    wait_ms(500);
    */
    
    while (true) {
        pc.printf("starting pattern\r\n");
        draw_square_large(moves_z, draw_z, &planner);
        wait_for_pattern();
    }
    
    /*
    while (1) {

        while(1) {
            run_pattern();
            if (go_run_pat)
                break;
        }
        
        status = SUCCESS;
        sbuffer_index = 0;
        while (status == SUCCESS) {
            status = fill_buffer();
            run_pattern();
        }
        pc.putc('D');
    }
    */
}
