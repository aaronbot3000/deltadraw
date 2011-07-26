#include "patterns.h"

void draw_star(F32 moves_height, F32 draw_height, Planner* planner) {
    Point a, b, c, d, e, f, g;
    g.x =  0;
    g.y =  2;
    g.z = moves_height;
    a.x =  0;
    a.y =  2;
    a.z = draw_height;
    b.x =  2;
    b.y = -2;
    b.z = draw_height;
    c.x = -2;
    c.y =  1;
    c.z = draw_height;
    d.x =  2;
    d.y =  1;
    d.z = draw_height;
    e.x = -2;
    e.y = -2;
    e.z = draw_height;
    f.x =  0;
    f.y =  0;
    f.z = moves_height;
    add_point_to_buffer(planner, g);
    add_point_to_buffer(planner, a);
    add_point_to_buffer(planner, b);
    add_point_to_buffer(planner, c);
    add_point_to_buffer(planner, d);
    add_point_to_buffer(planner, e);
    add_point_to_buffer(planner, a);
    add_point_to_buffer(planner, g);
    //add_point_to_buffer(planner, f);
}

void draw_square_large(F32 moves_height, F32 draw_height, Planner* planner) {
    Point a, b, c, d, g;
    g.x =  0;
    g.y =  4;
    g.z = moves_height;
    a.x =  0;
    a.y =  4;
    a.z = draw_height;
    b.x =  4;
    b.y =  0;
    b.z = draw_height;
    c.x =  0;
    c.y = -4;
    c.z = draw_height;
    d.x = -4;
    d.y =  0;
    d.z = draw_height;

    add_point_to_buffer(planner, g);
    add_point_to_buffer(planner, a);
    add_point_to_buffer(planner, b);
    add_point_to_buffer(planner, c);
    add_point_to_buffer(planner, d);
    add_point_to_buffer(planner, a);
    add_point_to_buffer(planner, g);
}

void draw_square_nn(F32 moves_height, F32 draw_height, Planner* planner) {
    Point a, b, c, d, g;
    g.x = -3;
    g.y = -3;
    g.z = moves_height;
    a.x = -3;
    a.y = -3;
    a.z = draw_height;
    b.x = -1;
    b.y = -3;
    b.z = draw_height;
    c.x = -1;
    c.y = -1;
    c.z = draw_height;
    d.x = -3;
    d.y = -1;
    d.z = draw_height;

    add_point_to_buffer(planner, g);
    add_point_to_buffer(planner, a);
    add_point_to_buffer(planner, b);
    add_point_to_buffer(planner, c);
    add_point_to_buffer(planner, d);
    add_point_to_buffer(planner, a);
    add_point_to_buffer(planner, g);
}
