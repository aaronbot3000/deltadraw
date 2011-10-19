#ifndef COMMON_H
#define COMMON_H

#include "mbed.h"
#include "math.h"
 
#define ARM_UPPER_LEN 2.75
#define ARM_LOWER_LEN 11.71875

#define HAND_XOFF  1.09725
#define HAND_ZOFF -0.20975

#define TOOL_ZOFF -1.78000

#define SERVO_XOFF -4.0096
#define SERVO_ZOFF  0.7087

#define MAX_Z 13
#define MIN_Z 7.9

#define MAX_X  3.1
#define MIN_X -3.1

#define MAX_Y  3.1
#define MIN_Y -3.1

#define START_Z (MIN_Z + 0.1)

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define RESTRICT(a, b1, b2) (MIN(b2, MAX(a, b1)))

#define MAP(a, b1, b2, x1, x2) (((F32)((a) - (b1)) / (F32)((b2) - (b1))) * ((x2) - (x1)) + (x1)) 
#define MAPEXP(a, b1, b2, x1, x2) (x1 * pow((float)1.5, -(((F32)((a) - (b1)) / (F32)((b2) - (b1))) * x2)))

#define DEBUG

typedef enum {
    SUCCESS,
    FAILURE,
    END_PAT,
    RUN_MORE
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
    
    Point& operator=(const Point &rhs) {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        return *this;
    }
    
    Point(volatile Point& in) {
        x = in.x;
        y = in.y;
        z = in.z;
    }
    
    Point() {
        x = y = z = 0;
    }
    
    Point(F32 a, F32 b, F32 c) {
        x = a;
        y = b;
        z = c;
    }
};

#endif