#include "patterns.h"

int draw_star(F32 moves_z, F32 draw_z, Point* buffer) {
    Point a, b, c, d, e, g;
    g.x =  0;
    g.y =  2;
    g.z = moves_z;
    a.x =  0;
    a.y =  2;
    a.z = draw_z;
    b.x =  2;
    b.y = -2;
    b.z = draw_z;
    c.x = -2;
    c.y =  1;
    c.z = draw_z;
    d.x =  2;
    d.y =  1;
    d.z = draw_z;
    e.x = -2;
    e.y = -2;
    e.z = draw_z;
    int index = 0;
    buffer[index++] = g;
    buffer[index++] = a;
    buffer[index++] = b;
    buffer[index++] = c;
    buffer[index++] = d;
    buffer[index++] = e;
    buffer[index++] = a;
    buffer[index++] = g;
    return index;
}

int draw_square_large(F32 moves_z, F32 draw_z, Point* buffer) {
    int index = 0;
    Point a, b, c, d, g;
    g.x =  0;
    g.y =  4;
    g.z = moves_z;
    a.x =  0;
    a.y =  4;
    a.z = draw_z;
    b.x =  4;
    b.y =  0;
    b.z = draw_z;
    c.x =  0;
    c.y = -4;
    c.z = draw_z;
    d.x = -4;
    d.y =  0;
    d.z = draw_z;

    buffer[index++] = g;
    buffer[index++] = a;
    buffer[index++] = b;
    buffer[index++] = c;
    buffer[index++] = d;
    buffer[index++] = a;
    buffer[index++] = g;
    return index;
}

int draw_square_nn(F32 moves_z, F32 draw_z, Point* buffer) {
    int index = 0;
    Point a, b, c, d, g;
    g.x = -3;
    g.y = -3;
    g.z = moves_z;
    a.x = -3;
    a.y = -3;
    a.z = draw_z;
    b.x = -1;
    b.y = -3;
    b.z = draw_z;
    c.x = -1;
    c.y = -1;
    c.z = draw_z;
    d.x = -3;
    d.y = -1;
    d.z = draw_z;

    buffer[index++] = g;
    buffer[index++] = a;
    buffer[index++] = b;
    buffer[index++] = c;
    buffer[index++] = d;
    buffer[index++] = a;
    buffer[index++] = g;
    return index;
}

int draw_ti(F32 moves_height, F32 draw_height, Point off, Point* buffer){
    int index = 0;
    Point a;
    // H
    a.x = 0; a.y = 0; a.z = moves_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 0; a.y = 0; a.z = draw_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 0; a.y = 2; a.z = draw_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 0; a.y = 2; a.z = moves_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 0; a.y = 1; a.z = moves_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 0; a.y = 1; a.z = draw_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 0.5; a.y = 1; a.z = draw_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 0.5; a.y = 1; a.z = moves_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 0.5; a.y = 2; a.z = moves_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 0.5; a.y = 2; a.z = draw_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 0.5; a.y = 0; a.z = draw_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 0.5; a.y = 0; a.z = moves_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    
    // I
    a.x = 0.75; a.y = 2; a.z = moves_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 0.75; a.y = 2; a.z = draw_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 0.75; a.y = 1.5; a.z = draw_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 0.75; a.y = 1.5; a.z = moves_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 0.75; a.y = 1; a.z = moves_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 0.75; a.y = 1; a.z = draw_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 0.75; a.y = 0; a.z = draw_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 0.75; a.y = 0; a.z = moves_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    
    // T
    a.x = 1.5; a.y = 2; a.z = moves_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 1.5; a.y = 2; a.z = draw_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 2; a.y = 2; a.z = draw_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 1.75; a.y = 2; a.z = draw_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 1.75; a.y = 0; a.z = draw_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 1.75; a.y = 0; a.z = moves_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    
    // I
    a.x = 2.25; a.y = 0; a.z = moves_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 2.25; a.y = 0; a.z = draw_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 2.75; a.y = 0; a.z = draw_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 2.5; a.y = 0; a.z = draw_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 2.5; a.y = 2; a.z = draw_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 2.25; a.y = 2; a.z = draw_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 2.75; a.y = 2; a.z = draw_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    a.x = 2.75; a.y = 2; a.z = moves_height;
    a.x += off.x; a.y += off.y;
    buffer[index++] = a;
    return index;
}
