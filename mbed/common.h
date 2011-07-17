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

#define DEBUG

typedef enum {
    SUCCESS,
    FAILURE
} Status;

typedef unsigned char  U08;
typedef unsigned short U16;
typedef unsigned int U32;
typedef unsigned long long U64;

typedef char  S08;
typedef short S16;
typedef int S32;
typedef long long S64;

typedef float F32;
typedef double F64;

struct Point{
    F32 x;
    F32 y;
    F32 z;
};



#endif