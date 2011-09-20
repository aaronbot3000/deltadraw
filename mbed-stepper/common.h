#ifndef COMMON_H
#define COMMON_H

#include "mbed.h"
 
#define ARM_UPPER_LEN 2.489
#define ARM_LOWER_LEN 10.546875

#define HAND_XOFF  1.09725
#define HAND_ZOFF -0.20975

#define TOOL_ZOFF -1.78000

#define SERVO_XOFF -2.2085
#define SERVO_ZOFF  0.51

#define MAX_Z 11.5
#define MIN_Z 10.15

#define MAX_X  3.1
#define MIN_X -3.1

#define MAX_Y  3.1
#define MIN_Y -3.1

#define START_Z (MIN_Z - 0.5)

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define RESTRICT(a, b1, b2) (MIN(b2, MAX(a, b1)))
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
    
    Point& operator=(const Point &rhs) {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        return *this;
    }
    
    /*
    Point& operator=(Point &rhs) {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        return *this;
    }
    */
    
    /*
    Point(const Point& in) {
        x = in.x;
        y = in.y;
        z = in.z;
    }
    */
    
    Point(volatile Point& in) {
        x = in.x;
        y = in.y;
        z = in.z;
    }
    
    Point() {
        x = y = z = 0;
    }
};

#endif