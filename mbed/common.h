#ifndef COMMON_H
#define COMMON_H

#include "mbed.h"

#define ARM_UPPER_LEN 5
#define ARM_LOWER_LEN 10.5

#define HAND_XOFF  1.09725
#define HAND_ZOFF -0.20975

#define TOOL_ZOFF -1.78000

#define SERVO_XOFF -2.2085
#define SERVO_ZOFF  0.51

#define START_HEIGHT 7 // inches
#define MIN_HEIGHT 7.5
#define MAX_HEIGHT 12

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MAP(a, b1, b2, x1, x2) (((F32)((a) - (b1)) / (F32)((b2) - (b1))) * ((x2) - (x1)) + (x1)) 

#define DEBUG

typedef enum {
    SUCCESS,
    FAILURE,
    END_PAT
} Status;

typedef unsigned char  U08;
typedef unsigned short U16;
typedef unsigned int U32;
typedef unsigned long long U64;

typedef signed char  S08;
typedef short S16;
typedef int S32;
typedef long long S64;

typedef float F32;
typedef double F64;

struct Point {
    F32 x;
    F32 y;
    F32 z;
};

#endif