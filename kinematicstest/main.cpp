#include "common.h"
#include "kinematics.h"
#include <cstdio>

#define TODEG (180.0 / 3.14159265)

int main() {
	Point in;
	in.z = 10;
	F32 angles[3];
	inv_kinematics(angles, in);
	printf("point: %f %f %f\n", in.x, in.y, in.z);
	printf("angles: %f %f %f\n", angles[0] * TODEG, angles[1] * TODEG, angles[2] * TODEG);

	fwd_kinematics(&in, angles);
	printf("point: %f %f %f\n", in.x, in.y, in.z);
	printf("angles: %f %f %f\n", angles[0] * TODEG, angles[1] * TODEG, angles[2] * TODEG);

	return 0;
}
