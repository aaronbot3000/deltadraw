#include "planner.h"

inline F32 dist_between(Point a, Point b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
}

inline void conform_goal(Point* in) {
    in->x = RESTRICT(in->x, MIN_X, MAX_X);
    in->y = RESTRICT(in->y, MIN_Y, MAX_Y);
    in->z = RESTRICT(in->z, MIN_Z, MAX_Z);
}

Status planner_setup(Planner* planner) {
    // Initialize circular buffer
    planner->current = 0;
    planner->next = 0;
    
    planner->current_pos.x = 0;
    planner->current_pos.y = 0;
    planner->current_pos.z = START_Z;
    
    planner->prev_dist = 0;
    
    return set_position(planner->current_pos);
}

Status reset_position(Planner* planner) {
    Point goal;
    goal.x = 0;
    goal.y = 0;
    goal.z = START_Z;
    return goto_point(planner, goal);
}

Status add_point_to_buffer(Planner* planner, Point in) {
    if (INC_ONE(planner->next) == planner->current)
        return FAILURE;
    planner->buffer[planner->next] = in;
    planner->next = INC_ONE(planner->next);
    return SUCCESS;
}

void clear_buffer(Planner* planner) {
    // Initial location
    planner->next = planner->current;
}

Status goto_point(Planner* planner, F32 x, F32 y, F32 z) {
    Point goal;
    goal.x = x;
    goal.y = y;
    goal.z = z;
    return goto_point(planner, goal);
}

Status goto_point(Planner* planner, Point goal) {
    Point cur  = planner->current_pos;
    F32 step = 0, inv_vec_mag;
    F32 dx, dy, dz;
    F32 dist, full_dist, prev_dist;
    
    Planner_State state = PLR_ACCL;
    
    conform_goal(&goal);

    dx = goal.x - cur.x;
    dy = goal.y - cur.y;
    dz = goal.z - cur.z;

    dist = dist_between(cur, goal);
    full_dist = dist;
    prev_dist = dist;
    
    if (dist < MIN_STEP_SIZE)
        return SUCCESS;

    // Inverse square root!!!
    inv_vec_mag = 1 / dist;
    dx = dx * inv_vec_mag;
    dy = dy * inv_vec_mag;
    dz = dz * inv_vec_mag;
    
    // TODO: think of a better way to exit
    while (1) {
        if (state == PLR_ACCL) {
            if (full_dist - prev_dist > ACCL_ZONE)
                state = PLR_FULL;
    
            else if (prev_dist < ACCL_ZONE && prev_dist * 2 < full_dist)
                state = PLR_DECL;
    
            else
                step = MAP(full_dist - prev_dist, 0, ACCL_ZONE, MIN_STEP_SIZE, MAX_STEP_SIZE);
        }
        if (state == PLR_FULL) {
            if (prev_dist < ACCL_ZONE)
                state = PLR_DECL;
    
            else
                step = MAX_STEP_SIZE;
        }
        if (state == PLR_DECL) {
            Point test_step;
            step = MAP(prev_dist, 0, ACCL_ZONE, MIN_STEP_SIZE, MAX_STEP_SIZE);
    
            test_step.x = cur.x + dx * step;
            test_step.y = cur.y + dy * step;
            test_step.z = cur.z + dz * step;
            
            dist = dist_between(test_step, goal);
            
//            pc.printf("dist: %.15f, prev_dist: %.15f\n", dist, prev_dist);
            if (dist >= prev_dist || dist < MIN_STEP_SIZE) {
                // This corrects accumulated error
                cur.x = goal.x;
                cur.y = goal.y;
                cur.z = goal.z;
                break;
            }
        }
        
        cur.x += dx * step;
        cur.y += dy * step;
        cur.z += dz * step;
    
        if (set_position(cur) != SUCCESS)
            return FAILURE;
            
        prev_dist = dist_between(cur, goal);
    }
    planner->current_pos = cur;
    return SUCCESS;
}

Status planner_process(Planner* planner) {
    if (planner->current == planner->next)
        return SUCCESS;
    if (goto_point(planner, planner->buffer[planner->current]) == SUCCESS) {

        if (INC_ONE(planner->current) == planner->next)
            return END_PAT;

        planner->current = INC_ONE(planner->current);

        return SUCCESS;
    }
    return FAILURE;
}

Status troll_up(Planner* planner) {
    if (planner->current_pos.z - 0.0001 > MIN_Z) {
        planner->current_pos.z -= MIN_STEP_SIZE;
        return set_position(planner->current_pos);
    }
    return FAILURE;
}

Status troll_down(Planner* planner) {
    if (planner->current_pos.z + MIN_STEP_SIZE < MAX_Z) {
        planner->current_pos.z += MIN_STEP_SIZE;
        return set_position(planner->current_pos);
    }
    return FAILURE;
}

Status nudge_x(Planner* planner, F32 amount) {
    Point goal = planner->current_pos;
    goal.x += amount;
    conform_goal(&goal);
    //pc.printf("X at: %.5f delta: %.5f\n", goal.x, amount);
    return goto_point(planner, goal);
}
