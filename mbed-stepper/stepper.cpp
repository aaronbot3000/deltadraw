#include "stepper.h"

static DigitalOut dir[] = {DigitalOut(p23), DigitalOut(p22), DigitalOut(p21)};
static DigitalOut stepper[] = {DigitalOut(p26), DigitalOut(p25), DigitalOut(p24)};
static AnalogIn posIn[] = {AnalogIn(p18), AnalogIn(p17), AnalogIn(p20)};

extern DigitalOut led1;
extern DigitalOut led2;
extern DigitalOut led3;
extern DigitalOut led4;

static F32 angles[3];

//extern Serial pc;

/*
45 top: -.90386 rad
1: 13471
2: 13729
3: 16502

1: 13440
2: 13731
3: 16307

angle:
13.5744 deg or 6.7872 deg
*/

static float raw_angles[3];
static const float calib_angles[3] = {13440, 13731, 16307};
static const float calib_angle = -0.90386;

static const S32 samples = 24;

void update_pos() {
    int i;
    S32 ext[3][samples];
    //static int counter = 0;
    
    // Wait for LPF to settle
    wait_us(100);
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
        raw_angles[x] = 0;
        for (int i = 8; i < 16; i++)
            raw_angles[x] += ext[x][i];
        
        raw_angles[x] /= 8;
        //if (counter == 10)
          //  pc.printf("rawangles: %f\r\n", raw_angles[x]);
    }
    //counter= (counter + 1) % 11;
    //pc.printf("actaangles: %f %f %f\r\n", angles[0], angles[1], angles[2]);
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

void calib_arm(int arm_no) {
    int make_step = 0;
    while (true) {
        update_pos();
        angles[arm_no] = 0;
        make_step = 0;

        make_step |= (raw_angles[arm_no] > calib_angles[arm_no]) << arm_no;
        if (!make_step)
            break;
    
        move_steppers(make_step, 0);
        
        if (raw_angles[arm_no] - calib_angles[arm_no] > 500) {
            for (int i = 0; i < 5; i++) {
                wait_us(300);
                move_steppers(make_step, 0);
            }
        }
    }
    angles[arm_no] = calib_angle;
    for (int i = 0; i < 300; i++) {
        move_steppers(1 << arm_no, 1 << arm_no);
        wait_us(400);
    }
}

void calibrate() {
    for (int i = 0; i < 3; i++) {
        calib_arm(i);
    }
}
        
        
        
        
        
        
        
        
        
        