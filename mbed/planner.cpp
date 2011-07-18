#include "planner.h"

inline F32 dist_2(Point a, Point b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
}

void planner_setup(Planner* planner) {
    planner->current = 0;
    planner->next = 1;
    planner->buffer[planner->current].x = 0;
    planner->buffer[planner->current].y = 0;
    planner->buffer[planner->current].z = 7.5;
    planner->current_pos.x = 0;
    planner->current_pos.y = 0;
    planner->current_pos.z = 7.5;
}

Status add_point_to_buffer(Planner* planner, Point in) {
    if (planner->next == planner->current)
        return FAILURE;
    planner->buffer[planner->next] = in;
    planner->next = INC_ONE(planner->next);
    return SUCCESS;
}

Status planner_process(Planner* planner) {
    Point goal = planner->buffer[planner->current];
    Point cur  = planner->current_pos;
    F32 dist = dist_2(cur, goal);
    
    // Is our current location within a step size of the goal?
    if (dist < MAX_STEP_SIZE * MAX_STEP_SIZE) {
        F32 inv_vec_mag, dx, dy, dz;

        if (INC_ONE(planner->current) == planner->next)
            return SUCCESS;
        planner->current = INC_ONE(planner->current);
        goal = planner->buffer[planner->current];
        
        dx = goal.x - cur.x;
        dy = goal.y - cur.y;
        dz = goal.z - cur.z;
        
        dist = dist_2(cur, goal);
        // Inverse square root!!!
        inv_vec_mag = 1 / sqrt(dist);
        planner->dx = dx * inv_vec_mag;
        planner->dy = dy * inv_vec_mag;
        planner->dz = dz * inv_vec_mag;
        //pc.printf("%.5f, %.5f, %.5f\n", planner->dx, planner->dx, planner->dx);
    }
    
    // Move toward the goal one step size
    cur.x += planner->dx * MIN_STEP_SIZE;
    cur.y += planner->dy * MIN_STEP_SIZE;
    cur.z += planner->dz * MIN_STEP_SIZE;
    
    
    
    set_position(cur);
    
    planner->current_pos = cur;
    
    return SUCCESS;
}
