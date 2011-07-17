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

inline F32 dist_o2(Point c) {
    return r2(c.x) + r2(c.y) +r2(c.z);
}

Status set_position(Point target) {
    F32 distance;
    F32 alpha_c, alpha;
    F32 discriminant;
    F32 angle;
    Point target_rot, trans, joint, joint_b;
    F32 result[3];
    
    alpha_c = (ARM_LOWER_LEN * ARM_LOWER_LEN) - 
            (ARM_UPPER_LEN * ARM_UPPER_LEN);
            
    target_rot.x = target.x;
    target_rot.y = target.y;
    target_rot.z = target.z;

    for (int servo = 0; servo < 3; servo++) {
        // rotate coordinates
        if (servo == 1)
            target_rot = rotate_xy(target, SIN_120, COS_120);
        if (servo == 2)
            target_rot = rotate_xy(target, SIN_240, COS_240);
    
        // Add servo offset and hand offset
        trans.x = target_rot.x + SERVO_XOFF + HAND_XOFF;
        trans.y = target_rot.y;
        // Add servo offset and tool offset
        trans.z = target_rot.z + SERVO_ZOFF + TOOL_ZOFF;
        distance = dist_o2(trans);
       
        alpha = alpha_c - distance;
        alpha /= 2;

        discriminant = r2(trans.x) * r2(trans.z) * r2(ARM_UPPER_LEN) + 
            r4(trans.z) * r2(ARM_UPPER_LEN) - 
            r2(trans.z) * r2(alpha);
            
       // pc.printf("%f, %f, %f, %f\n", target.x, target.y, target.z, discriminant);

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
        
        
    }

    servo_set_angle(SERVO_0,   result[0]);
    servo_set_angle(SERVO_120, result[1]);
    servo_set_angle(SERVO_240, result[2]);

    return SUCCESS;
}

Point position() {
    return cur_position;
}
