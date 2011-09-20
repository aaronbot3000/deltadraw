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
        trans.z = target_rot.z + SERVO_ZOFF + TOOL_ZOFF;

        lower_radius = sqrt(r2(ARM_LOWER_LEN) - r2(trans.y));

        dist     = sqrt(r2(trans.x) + r2(trans.z));
        // Inverse square root!!!
        inv_dist = 1 / dist;

        // Bounds checking
        if (dist > (ARM_UPPER_LEN + lower_radius) || 
                dist < (lower_radius - ARM_UPPER_LEN)) {
            printf("OH FFFFFFUUUUUUUU Inv Failure %.5f, %.5f, %.5f\r\n", dist, ARM_UPPER_LEN, lower_radius);
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

Status fwd_kinematics(Point* target, F32* angles) {
    F32 x1 = ARM_UPPER_LEN * cos(angles[0]) - SERVO_XOFF;
    F32 z1 = ARM_UPPER_LEN * sin(angles[0]) - SERVO_ZOFF;

    F32 x2 = -(ARM_UPPER_LEN * cos(angles[1]) - SERVO_XOFF) * COS_30;
    F32 y2 = x2 * TAN_60;
    F32 z2 = ARM_UPPER_LEN * sin(angles[1]) - SERVO_ZOFF;

    F32 x3 = -(ARM_UPPER_LEN * cos(angles[2]) - SERVO_XOFF) * COS_30;
    F32 y3 = -x3 * TAN_60;
    F32 z3 = ARM_UPPER_LEN * sin(angles[2]) - SERVO_ZOFF;

    F32 dnm = (x2 - x1) * y3 - (x3 - x1) * y2;

    F32 w1 = r2(x1) + r2(z1);
    F32 w2 = r2(y2) + r2(y2) + r2(z2);
    F32 w3 = r2(y3) + r2(x3) + r2(z3);

    // x = (a1*z + b1)/dnm
    F32 a1 = (z2 - z1) * (x3 - x1) - (z3 - z1) * (x2 - x1);
    F32 b1 = -((w2 - w1) * (x3 - x1) - (w3 - w1) * (x2 - x1))/2.0;

    // y = (a2 * z + b2)/dnm;
    F32 a2 = -(z2 - z1) * y3 + (z3 - z1) * y2;
    F32 b2 = ((w2 - w1) * y3 - (w3 - w1) * y2)/2.0;

    // a*z^2 + b*z + c = 0
    F32 a = r2(a1) + r2(a2) + r2(dnm);
    F32 b = 2 * (a1 * b1 + a2 * (b2 - x1 * dnm) - z1 * r2(dnm));
    F32 c = r2((b2 - x1 * dnm)) + r2(b1) + r2(dnm) * (r2(z1) - r2(ARM_LOWER_LEN));

    // discriminant
    F32 d = r2(b) - 4 * a * c;
    if (d < 0) {
        printf("FFUUUUU Fwd failure\r\n");
        return FAILURE; // non-existing point
    }

    target->z = -0.5 * (b - sqrt(d)) / a;
    target->y = (a1 * target->z + b1) / dnm;
    target->x = (a2 * target->z + b2) / dnm;
    return SUCCESS;
}
