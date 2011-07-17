#include "positioner.h"

static Point cur_position;

inline F32 max(F32 a, F32 b) {
    return (a > b ? a : b);
}

inline Point rotate_xy(Point coord, F32 sin_theta, F32 cos_theta) {
    coord.x = coord.x * cos_theta - coord.y * sin_theta;
    coord.y = coord.x * sin_theta + coord.y * cos_theta;
    return coord;
}

inline F32 r2(F32 in) {
    return in * in;
}

inline F32 r4(F32 in) {
    return in * in * in * in;
}

inline F32 dist(Point c1, Point c2) {
    return sqrt(r2(c2.x - c1.x) + r2(c2.y - c1.y) +r2(c2.z - c1.z));
}

inline F32 dist_o(Point c) {
    return sqrt(r2(c.x) + r2(c.y) +r2(c.z));
}

Status set_position(Point target) {
    F32 distance;
    F32 alpha;
    F32 discriminant;
    F32 angle;
    Point trans, joint, joint_b;
    F32 result[3];

    // Add hand offset
    target.x += HAND_XOFF;
    target.z += HAND_ZOFF;

    // Add tool offset
    target.z += TOOL_ZOFF;

    for (int servo = 0; servo < 3; servo++) {
        trans.x = target.x + SERVO_XOFF;
        trans.y = target.y;
        trans.z = target.z + SERVO_ZOFF;
        distance = dist_o(trans);
        alpha = (ARM_LOWER_LEN * ARM_LOWER_LEN) - 
            (ARM_UPPER_LEN * ARM_UPPER_LEN);
        alpha -= distance;
        alpha /= 2;

        discriminant = r2(trans.x) * r2(trans.z) * r2(ARM_UPPER_LEN) + 
            r4(trans.z) * r2(ARM_UPPER_LEN) - 
            r2(trans.z) * r2(alpha);

        if (discriminant < 0)
            return FAILURE;

        discriminant = sqrt(discriminant);

        joint.x = max((discriminant - trans.x * alpha) / (r2(trans.x) + r2(trans.z)),
                (-discriminant - trans.x * alpha) / (r2(trans.x) + r2(trans.z)));
        joint.y = 0;
        joint.z = sqrt(r2(ARM_UPPER_LEN) - r2(joint.x));

        joint_b.x = joint.x;
        joint_b.y = joint.y;
        joint_b.z = -joint.z;

        if (abs(dist(joint, trans) - ARM_LOWER_LEN) > abs(dist(joint_b, trans) - ARM_LOWER_LEN))
            joint = joint_b;

        angle = atan2(joint.z, joint.x);

        result[servo] = angle;
        
        target = rotate_xy(target, SIN_120, COS_120);
    }

    servo_set_angle(SERVO_0,   result[0]);
    servo_set_angle(SERVO_120, result[1]);
    servo_set_angle(SERVO_240, result[2]);

    return SUCCESS;
}

Point position() {
    return cur_position;
}

