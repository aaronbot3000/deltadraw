#include "planner.h"

inline F32 dist_between(Point a, Point b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
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
    planner->prev_dist = 0;
    planner->state = PLR_ACCL;
    set_position(planner->current_pos);
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
    F32 step = 0, dist;

    if (planner->state == PLR_ACCL) {
        if (planner->full_dist - planner->prev_dist > ACCL_ZONE)
            planner->state = PLR_FULL;

        else if (planner->prev_dist < ACCL_ZONE)
            planner->state = PLR_DECL;

        else
            step = MAP(planner->full_dist - planner->prev_dist, 0, ACCL_ZONE, MIN_STEP_SIZE, MAX_STEP_SIZE);
    }
    if (planner->state == PLR_FULL) {
        if (planner->prev_dist < ACCL_ZONE)
            planner->state = PLR_DECL;

        else
            step = MAX_STEP_SIZE;
    }
    if (planner->state == PLR_DECL) {
        Point test_step;
        step = MAP(planner->prev_dist, 0, ACCL_ZONE, MIN_STEP_SIZE, MAX_STEP_SIZE);

        test_step.x = cur.x + planner->dx * step;
        test_step.y = cur.y + planner->dy * step;
        test_step.z = cur.z + planner->dz * step;
        
        dist = dist_between(test_step, goal);
        if (dist >= planner->prev_dist) {
            planner->state = PLR_NEXT;
        }
    }
    if (planner->state == PLR_NEXT) {
        F32 inv_vec_mag, dx, dy, dz;

        if (INC_ONE(planner->current) == planner->next)
            return SUCCESS;

        cur.x = goal.x;
        cur.y = goal.y;
        cur.z = goal.z;

        planner->current = INC_ONE(planner->current);
        goal = planner->buffer[planner->current];

        dx = goal.x - cur.x;
        dy = goal.y - cur.y;
        dz = goal.z - cur.z;

        dist = dist_between(cur, goal);
        planner->full_dist = dist;

        // Inverse square root!!!
        inv_vec_mag = 1 / dist;
        planner->dx = dx * inv_vec_mag;
        planner->dy = dy * inv_vec_mag;
        planner->dz = dz * inv_vec_mag;
        
        planner->state = PLR_WAIT;
    }

    if (planner->state != PLR_WAIT) {
        cur.x += planner->dx * step;
        cur.y += planner->dy * step;
        cur.z += planner->dz * step;
    
        set_position(cur);
    }
    else
        planner->state = PLR_ACCL;
        
    planner->current_pos = cur;
    planner->prev_dist = dist_between(cur, goal);
    
    return SUCCESS;
}
