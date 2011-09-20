#include "positioner.h"

extern DigitalOut led1;
extern DigitalOut led2;
extern DigitalOut led3;
extern DigitalOut led4;

static Point cur_position;

inline F32 max(F32 a, F32 b) {
    return (a > b ? a : b);
}

inline Point rotate_xy(Point coord, F32 sin_theta, F32 cos_theta) {
    Point ret;
    ret.x = coord.x * cos_theta - coord.y * sin_theta;
    ret.y = coord.x * sin_theta + coord.y * cos_theta;
    ret.z = coord.z;
    return ret;
}

inline F32 r2(F32 in) {
    return in * in;
}

Status set_position(Point target) {
    F32 dist, inv_dist, alpha;
    F32 angle;
    F32 x1, z1, x2, z2, h;
    F32 lower_radius;
    Point target_rot, trans;
    F32 result[3];
    
    // Bounds checking
    // Shaddup bout the Z offset
    if (target.x < MIN_X || target.x > MAX_X ||
        target.y < MIN_Y || target.y > MAX_Y ||
        target.z < MIN_Z - 0.75 || target.z > MAX_Z) {
        led3 = 1;
        return FAILURE;
    }
    
    target_rot.x = target.x;
    target_rot.y = target.y;
    target_rot.z = target.z;
    
    Timer time;
    time.start();

    for (int servo = 0; servo < 3; servo++) {
        // rotate coordinates
        if (servo == SERVO_120)
            target_rot = rotate_xy(target, SIN_120, COS_120);
        if (servo == SERVO_240)
            target_rot = rotate_xy(target, SIN_240, COS_240);
    
        // Add servo offset and hand offset
        trans.x = target_rot.x + SERVO_XOFF + HAND_XOFF;
        trans.y = target_rot.y;
        // Add servo offset and tool offset
        trans.z = target_rot.z + SERVO_ZOFF + TOOL_ZOFF;

        lower_radius = sqrt(r2(ARM_LOWER_LEN) - r2(trans.y));

        dist     = r2(trans.x) + r2(trans.z);
        // Inverse square root!!!
        inv_dist = 1 / dist;

        // Bounds checking
        if (dist > (ARM_UPPER_LEN + lower_radius) || 
            dist < (lower_radius - ARM_UPPER_LEN)) {
            //pc.printf("OH FFFFFFUUUUUUUU %.5f, %.5f, %.5f\n", dist, ARM_UPPER_LEN, lower_radius);
            led2 = 1;
            return FAILURE;
        }

        alpha = (r2(ARM_UPPER_LEN) - r2(lower_radius) + r2(dist)) * 0.5 * inv_dist;
        
        x1 = (trans.x * alpha * inv_dist);
        z1 = (trans.z * alpha * inv_dist);

        h = sqrt(r2(ARM_UPPER_LEN) - r2(alpha));

        x2 = -trans.z * (h * inv_dist);
        z2 =  trans.x * (h * inv_dist);

        angle = atan2(z1 - z2, x1 - x2);

        result[servo] = angle;
    }
    time.stop();
    
    pc.printf("%d\n", time.read_us());

    servo_set_angle(SERVO_0,   result[0]);
    servo_set_angle(SERVO_120, result[1]);
    servo_set_angle(SERVO_240, result[2]);

    return SUCCESS;
}

Point position() {
    return cur_position;
}

