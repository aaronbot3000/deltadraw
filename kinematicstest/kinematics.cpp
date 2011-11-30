#include "kinematics.h"

Point rotate_xy(Point coord, F32 sin_theta, F32 cos_theta) {
	Point ret;
	ret.x = coord.x * cos_theta - coord.y * sin_theta;
	ret.y = coord.x * sin_theta + coord.y * cos_theta;
	ret.z = coord.z;
	return ret;
}

F32 r2(F32 in) {
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
		trans.z = target_rot.z + SERVO_ZOFF;

		lower_radius = sqrt(r2(ARM_LOWER_LEN) - r2(trans.y));

		dist	 = sqrt(r2(trans.x) + r2(trans.z));
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
	F32 arm1 = ARM_UPPER_LEN * cos(angles[0]);
	F32 a1 = (arm1 - SERVO_XOFF - HAND_XOFF);
	F32 c1 = ARM_UPPER_LEN * sin(angles[0]) - SERVO_ZOFF;

	F32 arm2 = ARM_UPPER_LEN * cos(angles[2]);
	F32 a2 = (arm2 - SERVO_XOFF - HAND_XOFF) * COS_120;
	F32 b2 = (arm2 - SERVO_XOFF - HAND_XOFF) * SIN_120;
	F32 c2 = ARM_UPPER_LEN * sin(angles[2]) - SERVO_ZOFF;

	F32 arm3 = ARM_UPPER_LEN * cos(angles[1]);
	F32 a3 = (arm3 - SERVO_XOFF - HAND_XOFF) * COS_240;
	F32 b3 = (arm3 - SERVO_XOFF - HAND_XOFF) * SIN_240;
	F32 c3 = ARM_UPPER_LEN * sin(angles[1]) - SERVO_ZOFF;

	F32 p11 = 2 * (-a1 + a2);
	F32 p12 = 2 * b2;
	F32 p13 = 2 * (-c1 + c2);

	F32 d1 = -(r2(a1) + r2(c1)) + (r2(a2) + r2(b2) + r2(c2));

	F32 p21 = 2 * (-a2 + a3);
	F32 p22 = 2 * (-b2 + b3);
	F32 p23 = 2 * (-c2 + c3);

	F32 d2 = -(r2(a2) + r2(b2) + r2(c2)) + (r2(a3) + r2(b3) + r2(c3));

	F32 s1 = p12 * p23 - p13 * p22;
	F32 s2 = p13 * p21 - p11 * p23;
	F32 s3 = p11 * p22 - p12 * p21;

	//printf("p11: %f p12: %f p21 %f p22 %f\n", p11, p12, p21, p22);
	F32 py;
	F32 px;

	if (p21 < 1E-6 || -p21 < 1E-6) {
		py = d2 / p22;
		// TODO: if p11 < 1E-6
		px = (d1 - py * p12) / p11;
	}
	// TODO: if p11 < 1E-6 or (p12 * mult - p22) for some reason is zero
	else {
		F32 mult = p21 / p11;
		py = (d1 * mult - d2) / (p12 * mult - p22);
		px = (d2 - py * p22) / p21;
	}

	F32 A = r2(s1) + r2(s2) + r2(s3);
	F32 B = 2 * (s1 * (-a1 + px) + s2 * py - s3 * c1);
	F32 C = r2(-a1 + px) + r2(py) + r2(c1) - r2(ARM_LOWER_LEN);

	/*
	printf("%f\n", A);
	printf("%f\n", B);
	printf("%f\n", C);
	*/

	F32 t = (-B + sqrt(r2(B) - 4 * A * C)) / (2 * A);

	target->x = s1 * t + px;
	target->y = s2 * t + py;
	target->z = s3 * t;

	return SUCCESS;
}


