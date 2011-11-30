#include "common.h"
#include "kinematics.h"
#include <cstdio>

#define TODEG (180.0 / 3.14159265)

int main() {
	Point in;
	in.x = 0;
	in.y = 0;
	in.z = 11;
	F32 angles[3];
	printf("point: %f %f %f\n", in.x, in.y, in.z);
	inv_kinematics(angles, in);
	printf("angles: %f %f %f\n", angles[0] * TODEG, angles[1] * TODEG, angles[2] * TODEG);

	fwd_kinematics(&in, angles);
	printf("point: %f %f %f\n", in.x, in.y, in.z);
	inv_kinematics(angles, in);
	printf("angles: %f %f %f\n", angles[0] * TODEG, angles[1] * TODEG, angles[2] * TODEG);

	in.x = 3;
	in.y = 4;
	in.z = 10;
	angles[3];
	printf("point: %f %f %f\n", in.x, in.y, in.z);
	inv_kinematics(angles, in);
	printf("angles: %f %f %f\n", angles[0] * TODEG, angles[1] * TODEG, angles[2] * TODEG);

	fwd_kinematics(&in, angles);
	printf("point: %f %f %f\n", in.x, in.y, in.z);
	inv_kinematics(angles, in);
	printf("angles: %f %f %f\n", angles[0] * TODEG, angles[1] * TODEG, angles[2] * TODEG);

	in.x = 3;
	in.y = 4;
	in.z = 11;
	angles[3];
	printf("point: %f %f %f\n", in.x, in.y, in.z);
	inv_kinematics(angles, in);
	printf("angles: %f %f %f\n", angles[0] * TODEG, angles[1] * TODEG, angles[2] * TODEG);

	fwd_kinematics(&in, angles);
	printf("point: %f %f %f\n", in.x, in.y, in.z);
	inv_kinematics(angles, in);
	printf("angles: %f %f %f\n", angles[0] * TODEG, angles[1] * TODEG, angles[2] * TODEG);
	return 0;
}
