#include "mbed.h"

#include "common.h"
#include "plan-position.h"
#include "patterns.h"

#define moves_z (MAX(draw_z - 0.5, MIN_Z - 0.5))

#define START_TRANS 'B'
#define END_TRANS 0xFFFF1111

Serial pc(USBTX, USBRX); // tx, rx
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

DigitalIn goto_draw_height(p16);
DigitalIn troll_up(p15);
DigitalIn troll_down(p14);
DigitalIn start_pat(p13);

DigitalOut enablers[] = {DigitalOut(p27), DigitalOut(p28), DigitalOut(p29)};

volatile U08 serial_buffer[16];
volatile S32 sbuffer_index;

static F32 draw_z = MIN_Z + 0.6; // inches
bool pen_needs_reset = false;

Planner planner;

void serial_callback() {
    serial_buffer[sbuffer_index++] = pc.getc();
}

void startup_steppers() {
    enablers[0] = 0;
    for (int i = 1; i < 3; i++) {
        enablers[i] = 1;
    }
    wait_ms(100);
    enablers[1] = 0;
    wait_ms(100);
    enablers[2] = 0;
    wait_ms(100);
}

void setup() {
    startup_steppers();
    
    sbuffer_index = 0;
    pc.baud(115200);
    pc.attach(serial_callback);
    
    //pc.printf("reset to %d %d %d\r\n", home.x, home.y, home.z);

    setup_planner(&planner);

    ///pc.printf("Setup\r\n");
}


Status fill_buffer() {
    int i;
    Point in;
    
    led3 = 1;
    pause_steppers(&planner);
    pc.putc(START_TRANS);
    for (i = 0; i < BUFFER_SIZE - 2; i++) {
        while (sbuffer_index < 9);
        
        if ((*(U32*)(&serial_buffer[0])) == END_TRANS) {
            resume_steppers(&planner);
            return END_PAT;
        }
        sbuffer_index = 0;
        pc.putc(START_TRANS);
        
        in.x = *(F32*)(&serial_buffer[0]);
        in.y = *(F32*)(&serial_buffer[4]);
        
        if (serial_buffer[8] == 1)
            in.z = moves_z;
        else
            in.z = draw_z;
        
        if (!add_point_to_buffer(&planner, in)) {
            led2 = 1;
        }
    }
    led3 = 0;
    update_pos();
    resume_steppers(&planner);
    
    return SUCCESS;
}

void adj_z() {
    Point next_pos = planner.current_pos;
    if (troll_up && draw_z > (MIN_Z + 0.6)) {
        //pc.printf("up\r\n");
        draw_z -= 0.1; 
        next_pos.z = draw_z;
        add_point_to_buffer(&planner, next_pos);
    }
    else if (troll_down && draw_z < (MAX_Z)) {
        //pc.printf("down\r\n");
        draw_z += 0.1; 
        next_pos.z = draw_z;
        add_point_to_buffer(&planner, next_pos);
    }
    else {
        next_pos.z = draw_z;
        add_point_to_buffer(&planner, next_pos);
    }
    wait_for_pattern(&planner);
    pen_needs_reset = true;
    wait_us(10000);
}

void reset_pen() {
    Point a = planner.current_pos;
    a.x = 0;
    a.y = 0;
    a.z = START_Z;
    add_point_to_buffer(&planner, a);
    
    wait_for_pattern(&planner);
}

int main() {
    //Status status;
    setup();
    
    // adjust z
    while (1) {
        if (goto_draw_height) {
            adj_z();
        }
        else if (start_pat) {
            pen_needs_reset = true;
            update_pos();
            //pc.printf("starting pattern\r\n");
            Status status = SUCCESS;
            sbuffer_index = 0;
            while (status != END_PAT) {
                status = fill_buffer();
                wait_for_pattern(&planner);
            }
            pc.putc('D');
        }
        else if (pen_needs_reset) {
            reset_pen();
            pen_needs_reset = false;
        }
    }
}
