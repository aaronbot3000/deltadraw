#include "kinematics.h"

Point rotate_xy(Point coord, F32 sin_theta, F32 cos_theta) {
    Point ret;
    ret.x = coord.x * cos_theta - coord.y * sin_theta;
    ret.y = coord.x * sin_theta + coord.y * cos_theta;
    ret.z = coord.z;
    return ret;
}

inline F32 r2(F32 in) {
    return in * in;
}

extern Serial pc;

Status inv_kinematics(F32* result, Point target) {
    F32 dist, inv_dist, alpha;
    F32 angle;
    F32 x1, z1, x2, z2, h;
    F32 lower_radius;
    Point target_rot, trans;

    target_rot.x = target.x;
    target_rot.y = target.y;
    target_rot.z = target.z;

    for (int i = 0; i < 3; i++) {
        // rotate coordinates
        if (i == 1)
            target_rot = rotate_xy(target, SIN_120, COS_120);
        if (i == 2)
            target_rot = rotate_xy(target, SIN_240, COS_240);

        // Add servo offset and hand offset
        trans.x = target_rot.x + SERVO_XOFF + HAND_XOFF;
        trans.y = target_rot.y;
        // Add servo offset and tool offset
        trans.z = target_rot.z + SERVO_ZOFF;

        lower_radius = sqrt(r2(ARM_LOWER_LEN) - r2(trans.y));

        dist     = sqrt(r2(trans.x) + r2(trans.z));
        // Inverse square root!!!
        inv_dist = 1 / dist;

        // Bounds checking
        if (dist > (ARM_UPPER_LEN + lower_radius) || 
                dist < (lower_radius - ARM_UPPER_LEN)) {
            pc.printf("OH FFFFFFUUUUUUUU Inv Failure %.5f, %.5f, %.5f\r\n", dist, ARM_UPPER_LEN, lower_radius);
            //led2 = 1;
            return FAILURE;
        }

        alpha = (r2(ARM_UPPER_LEN) - r2(lower_radius) + r2(dist)) * 0.5 * inv_dist;

        x1 = (trans.x * alpha * inv_dist);
        z1 = (trans.z * alpha * inv_dist);

        h = sqrt(r2(ARM_UPPER_LEN) - r2(alpha));

        x2 = -trans.z * (h * inv_dist);
        z2 =  trans.x * (h * inv_dist);

        angle = atan2(z1 - z2, x1 - x2);

        result[i] = angle;
    }
    return SUCCESS;
}