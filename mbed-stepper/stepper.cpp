#include "stepper.h"

static DigitalOut stepper[] = {DigitalOut(p10), DigitalOut(p11), DigitalOut(p12)};
static DigitalOut dir[] = {DigitalOut(p13), DigitalOut(p14), DigitalOut(p15)};
static AnalogIn posIn[] = {AnalogIn(p18), AnalogIn(p19), AnalogIn(p20)};

static F32 angles[3];
static const F32 p_offset[3] = {0, 0, 0};
static const F32 p_mult[3] = {1, 1, 1};
static const S32 samples = 6;

void reset_steppers() {
    angles[0] = 0;
    angles[1] = 0;
    angles[2] = 0;
}

void update_pos() {
    int i;
    S32 ext[3][samples];
    for (int x = 0; x < 3; x++) {
        for (int j = 1; j < samples; j++) {
            ext[x][j] = posIn[x].read_u16() * p_mult[x] + p_offset[x];
            S32 index = ext[x][j];
            for  (i = j; i > 0 && ext[x][i] > ext[x][i - 1]; i--) {
                ext[x][i] = ext[x][i-1];
            }
            ext[x][i] = index;
        }
        angles[x] = (ext[x][samples / 2 - 1] + ext[x][samples / 2]) >> 1;
    }
}

F32* get_angles() {
    return angles;
}

extern Serial pc;
Status move_steppers(int steppers, int direction) {
    Status retcode = SUCCESS;
    //pc.printf("stepper code: %d %d\r\n", steppers, direction);
    for (int i = 0; i < 3; i++) {
        // Step up
        if ((direction >> i) & STEPPER_UP) {
            if (angles[i] > STEPPER_MAX_ANGLE) {
                retcode = FAILURE;
                steppers &= ~(0x1 << i);
            }
            else {
                dir[i] = 1;
                angles[i] += STEPPER_STEP_SIZE;
            }
        }
        // Step down
        else if ((steppers >> i) & 0x1) {
            if (angles[i] < STEPPER_MIN_ANGLE) {
                retcode = FAILURE;
                steppers &= ~(0x1 << i);
            }
            else {
                dir[i] = 0;
                angles[i] -= STEPPER_STEP_SIZE;
            }
        }
    }
    
    wait_us(1);
    
    for (int i = 0; i < 3; i++) {
        stepper[i] = (steppers >> i) & 0x1;
    }
    
    wait_us(1);
    
    for (int i = 0; i < 3; i++) {
        stepper[i] = 0;
    }
    return retcode;
}
