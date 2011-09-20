#include "mbed.h"

#include "common.h"
#include "stepper.h"
#include "plan-position.h"
#include "patterns.h"

#define moves_z (MAX(draw_z - 0.5, MIN_Z - 0.5))
#define UPDATE_INTERVAL 10000
#define START_TRANS 'B'
#define END_TRANS 0xFFFF1111

#define BUFFER_SIZE 256
#define INC_ONE(a) (((a) + 1) % BUFFER_SIZE)

Serial pc(USBTX, USBRX); // tx, rx
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

volatile U08 serial_buffer[16];
volatile S32 sbuffer_index;

Point input_buffer[BUFFER_SIZE];
S32 cur_buf_index;
S32 nxt_buf_index;

Ticker runner;

static F32 draw_z = MIN_Z; // inches

void serial_callback() {
    serial_buffer[sbuffer_index++] = pc.getc();
}

void setup() {
    reset_pen();
    pc.baud(115200);
    pc.attach(serial_callback);
    sbuffer_index = 0;
    cur_buf_index = 0;
    nxt_buf_index = 0;
    pc.printf("Setup\r\n");
}

void run_pattern() {
    Status status = SUCCESS;
    while (status == SUCCESS) {
        if (robot_met_goal()) {
            if (cur_buf_index != nxt_buf_index) {
                set_goal(input_buffer[cur_buf_index]);
                cur_buf_index = INC_ONE(cur_buf_index);
            }
            else {
                break;
            }
        }
    }
}

Status fill_buffer() {
    int i;
    Point in;
    
    runner.detach();
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

        if (INC_ONE(nxt_buf_index) == cur_buf_index)
            return FAILURE;
        input_buffer[nxt_buf_index] = in;
        nxt_buf_index = INC_ONE(nxt_buf_index);
    }
    runner.attach_us(catch_interrupt, 200000);
    
    return SUCCESS;
}

int main() {
    //Status status;
    setup();
    runner.attach_us(catch_interrupt, 200000);
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
        nxt_buf_index = draw_square_large(moves_z, draw_z, input_buffer);
        cur_buf_index = 0;
        run_pattern();
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
