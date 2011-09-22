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
    F32 a1 = ARM_UPPER_LEN * cos(angles[0]) - SERVO_XOFF - HAND_XOFF;
    F32 c1 = ARM_UPPER_LEN * sin(angles[0]) - SERVO_ZOFF - HAND_ZOFF;

	F32 arm2 = (ARM_UPPER_LEN * cos(angles[1]) - SERVO_XOFF - HAND_XOFF);
    F32 a2 = -arm2 * COS_60;
    F32 b2 = arm2 * SIN_60;
    F32 c2 = ARM_UPPER_LEN * sin(angles[1]) - SERVO_ZOFF - HAND_ZOFF;

	F32 arm3 = (ARM_UPPER_LEN * cos(angles[2]) - SERVO_XOFF - HAND_XOFF);
    F32 a3 = -arm3 * COS_60;
    F32 b3 = -arm3 * SIN_60;
    F32 c3 = ARM_UPPER_LEN * sin(angles[2]) - SERVO_ZOFF - HAND_ZOFF;

    F32 w1 = r2(a1) +          r2(c1);
    F32 w2 = r2(a2) + r2(b2) + r2(c2);
    F32 w3 = r2(a3) + r2(b3) + r2(c3);

	F32 A = 2 * (a2 - a1);
	F32 B = 2 * (b2);
	F32 C = 2 * (c2 - c1);

	F32 D = 2 * (a3 - a1);
	F32 E = 2 * (b3);
	F32 F = 2 * (c3 - c1);

    F32 dnm = B * D - A * E;
    F32 dnm2 = r2(dnm);

    // a = (m1 * z + n1)/dnm
	F32 m1 = -(C * D - A * F);
	F32 n1 = D * (w2 - w1) - A * (w3 - w1);

    // b = (m2 * z + n2)/dnm;
	F32 m2 = -(C * E - B * F);
	F32 n2 = E * (w2 - w1) - B * (w3 - w1);

    // m*z^2 + n*z + z = 0
	F32 a = (r2(m1) + r2(m2)) / dnm2 + 1;
	F32 b = 2 * ((m1 * n1 + m2 * n2) / dnm2 - (a1 * m1) / dnm - c1);
	F32 c = r2(a1) + r2(c1) + (r2(n1) + r2(n2)) / dnm2 - 2 * a1 * n1 / dnm - r2(ARM_LOWER_LEN);

    // discriminant
    F32 d = r2(b) - 4 * a * c;
    if (d < 0) {
        printf("FFFFFFFUUUUUUUUUUUU Fwd failure\r\n");
        return FAILURE; // non-existing point
    }

    target->z = 0.5 * (-b + sqrt(d)) / a;
    target->y = (m1 * target->z + n1) / dnm;
    target->x = (m2 * target->z + n2) / dnm;
    return SUCCESS;
}
