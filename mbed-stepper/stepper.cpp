#include "stepper.h"

static DigitalOut stepper[] = {DigitalOut(p10), DigitalOut(p11), DigitalOut(p12)};
static DigitalOut dir[] = {DigitalOut(p13), DigitalOut(p14), DigitalOut(p15)};
static AnalogIn posIn[] = {AnalogIn(p18), AnalogIn(p19), AnalogIn(p20)};

extern DigitalOut led1;
extern DigitalOut led2;
extern DigitalOut led3;
extern DigitalOut led4;

static F32 angles[3];

/*
1:
-.1184589 -> 36930
1.4523373 -> 14880
mult = 0.00007123792865
2:
-.1184589 -> 40450
1.4523373 -> 15730
mult = 0.00006354354072
3:
-.1184589 -> 41800
1.4523373 -> 17600
mult = 0.00006490893912
*/

static const F32 p_offset = -0.1184589;
static const F32 p_ref[3] = {36930, 40450, 41800};
static const F32 p_mult[3] = {0.00007123792865, 0.00006354354072, 0.00006490893912};
static const S32 samples = 6;

void update_pos() {
    int i;
    S32 ext[3][samples];
    for (int x = 0; x < 3; x++) {
        for (int j = 1; j < samples; j++) {
            ext[x][j] = posIn[x].read_u16();
            S32 index = ext[x][j];
            for  (i = j; i > 0 && ext[x][i] > ext[x][i - 1]; i--) {
                ext[x][i] = ext[x][i-1];
            }
            ext[x][i] = index;
        }
        angles[x] = (ext[x][samples / 2 - 1] + ext[x][samples / 2]) / 2;
        angles[x] = (p_ref[x] - angles[x]) * p_mult[x] + p_offset;
    }
}

F32* get_angles() {
    return angles;
}

extern Serial pc;
Status move_steppers(int steppers, int direction) {
    Status retcode = SUCCESS;
    for (int i = 0; i < 3; i++) {
        // Step to larger angle (down)
        if ((steppers >> i) & 0x1) {
            if ((direction >> i) & 0x1) {
                if (angles[i] > STEPPER_MAX_ANGLE) {
                    retcode = FAILURE;
                    steppers &= ~(0x1 << i);
                    led2 = 1;
                }
                else {
                    dir[i] = 0;
                    angles[i] += STEPPER_STEP_SIZE;
                }
            }
            // Step to smaller angle (up)
            else {
                if (angles[i] < STEPPER_MIN_ANGLE) {
                    retcode = FAILURE;
                    steppers &= ~(0x1 << i);
                    led2 = 1;
                }
                else {
                    dir[i] = 1;
                    angles[i] -= STEPPER_STEP_SIZE;
                }
            }
        }
    }
    wait_us(5);

    for (int i = 0; i < 3; i++) {
        stepper[i] = (steppers >> i) & 0x1;
    }
    wait_us(5);
    
    for (int i = 0; i < 3; i++) {
        stepper[i] = 0;
    }
    return retcode;
}
