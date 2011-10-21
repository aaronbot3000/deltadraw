#include "stepper.h"

static DigitalOut dir[] = {DigitalOut(p23), DigitalOut(p22), DigitalOut(p21)};
static DigitalOut stepper[] = {DigitalOut(p26), DigitalOut(p25), DigitalOut(p24)};
static AnalogIn posIn[] = {AnalogIn(p18), AnalogIn(p17), AnalogIn(p20)};

extern DigitalOut led1;
extern DigitalOut led2;
extern DigitalOut led3;
extern DigitalOut led4;

static F32 angles[3];

/*
level top: -.1184589 rad
1: 25250
2: 23965
3: 28536

vertical top, down: 1.452337 rad
1: 49868
2: 48211
3: 51076

angle:
13.5744 deg or 6.7872 deg

scaling factor: for pi/2 rad
1: 0.00006380682
2: 0.00006478579
3: 0.00006968927

offset to zero:
1: -1.729581105
2: -1.671050457
3: -2.107112009
*/

// tic const float p_ref[3] = {-1.729581105, -1.671050457, -2.107112009};
static const float p_ref[3] = {-1.729581105, -1.671050457, -2.007112009};
static const float p_mult[3] = {0.00006380682, 0.00006478579, 0.00006968927};
static const S32 samples = 24;

void update_pos() {
    int i;
    S32 ext[3][samples];
    
    // Wait for LPF to settle
    wait_ms(10);
    for (int x = 0; x < 3; x++) {
        for (int j = 1; j < samples; j++) {
            ext[x][j] = posIn[x].read_u16();
            S32 index = ext[x][j];
            for  (i = j; i > 0 && ext[x][i] > ext[x][i - 1]; i--) {
                ext[x][i] = ext[x][i-1];
            }
            ext[x][i] = index;
            wait_us(100);
        }
        for (int i = 8; i < 16; i++)
            angles[x] += ext[x][i];
        angles[x] /= 8;
        angles[x] = angles[x] * p_mult[x] + p_ref[x];
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
