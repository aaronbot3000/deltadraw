#include "plan-position.h"

//extern Serial pc;
extern DigitalOut led1;
extern DigitalOut led2;
extern DigitalOut led4;

static Planner* cur_plan;

F32 dist_between(Point a, Point b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
}

void conform_goal(Point* in) {
    in->x = RESTRICT(in->x, MIN_X, MAX_X);
    in->y = RESTRICT(in->y, MIN_Y, MAX_Y);
    in->z = RESTRICT(in->z, MIN_Z, MAX_Z);
}

F32 max(F32 a, F32 b) {
    return (a > b ? a : b);
}

F32 max(F32 a, F32 b, F32 c) {
    a = fabs(a);
    b = fabs(b);
    c = fabs(c);
    if (a > b) {
        return b > c ? a : a > c ? a : c;
    }
    else { // b > a
        return a > c ? b : b > c ? b : c;
    }
}

Status wait_for_pattern(Planner* planner) {
    while (true) {
        if (planner->finished) {
            return SUCCESS;
        }
        if (planner->errored) {
            //pc.printf("FAILURE");
            led2 = 1;
            return FAILURE;
        }
    }
}

void setup_planner(Planner* planner) {
    update_pos();
    
    planner->angles_actual = get_angles();
    
    planner->buf_ind = 0;
    planner->buf_next = 1;

    planner->steps_to_next = 0;
    
    planner->buffer[0].x = 0;
    planner->buffer[0].y = 0;
    planner->buffer[0].z = START_Z;
    
    planner->current_pos.x = 0;
    planner->current_pos.y = 0;
    planner->current_pos.z = START_Z + 0.01;

    cur_plan = planner;

    planner->finished = false;
    planner->errored = false;

    resume_steppers(planner);
    planner->state = PLR_ACCL;
}

bool add_point_to_buffer(Planner* planner, Point in) {
    if (INC_ONE(planner->buf_next) == planner->buf_ind)
        return false;
        
    planner->buffer[planner->buf_next].x = in.x;
    planner->buffer[planner->buf_next].y = in.y;
    planner->buffer[planner->buf_next].z = in.z;
    
    planner->buf_next = INC_ONE(planner->buf_next);
    
    planner->finished = false;
    return true;
}

void pause_steppers(Planner* planner) {
    planner->runner.detach();
}

void resume_steppers(Planner* planner) {
    planner->runner.attach_us(take_step, TIMING_INTERVAL);
}

Status get_next_steps(Planner* planner, bool reset_dist_flag) {
    Point goal = planner->buffer[planner->buf_ind];
    Point cur_pos = planner->current_pos;
    
    /*
    pc.printf("goal: %f %f %f\r\n", goal.x, goal.y, goal.z);
    pc.printf("curr: %f %f %f\r\n", cur_pos.x, cur_pos.y, cur_pos.z);
    */

    conform_goal(&goal);

    F32 dx = goal.x - cur_pos.x;
    F32 dy = goal.y - cur_pos.y;
    F32 dz = goal.z - cur_pos.z;

    F32 dist = dist_between(cur_pos, goal);
    F32 step = 0;
    Point test;

    if (reset_dist_flag) {
        planner->full_dist = dist;
        planner->prev_dist = dist;
        pause_steppers(planner);
        //update_pos();
        resume_steppers(planner);
        for (int i = 0; i < 3; i++)
            planner->angles_ideal[i] = planner->angles_actual[i];
    }
    F32 full_dist = planner->full_dist;
    F32 prev_dist = planner->prev_dist;

    F32 new_angles[3];

    if (dist < MIN_DIST) {
        return END_PAT;
    }

    // Inverse square root!!!
    F32 inv_vec_mag = 1 / dist;
    dx *= inv_vec_mag;
    dy *= inv_vec_mag;
    dz *= inv_vec_mag;
    
    // Apply acceleration and deceleration and
    // acquire next step size
    if (planner->state == PLR_ACCL) {
        if (full_dist - prev_dist > ACCL_ZONE)
            planner->state = PLR_FULL;

        else if (prev_dist < ACCL_ZONE && prev_dist * 2 < full_dist)
            planner->state = PLR_DECL;

        else
            //step = MAPEXP(full_dist - prev_dist, 0, ACCL_ZONE, MAX_STEP_SIZE, MIN_STEP_FRAC);
            step = MAP(fabs(full_dist - prev_dist), 0, ACCL_ZONE, MIN_STEP_SIZE, MAX_STEP_SIZE);
    }
    if (planner->state == PLR_FULL) {
        if (prev_dist < ACCL_ZONE)
            planner->state = PLR_DECL;

        else
            step = MAX_STEP_SIZE;
    }
    if (planner->state == PLR_DECL) {
        Point test;
        //step = MAPEXP(ACCL_ZONE - (prev_dist), 0, ACCL_ZONE, MAX_STEP_SIZE, MIN_STEP_FRAC);
        step = MAP(prev_dist, 0, ACCL_ZONE, MIN_STEP_SIZE, MAX_STEP_SIZE);

        test.x = cur_pos.x + dx * step;
        test.y = cur_pos.y + dy * step;
        test.z = cur_pos.z + dz * step;
        
        F32 tdist = dist_between(test, goal);
        if (tdist >= prev_dist || tdist < MIN_DIST) {
            planner->state = PLR_ACCL;
            return END_PAT;
        }
    }
    
    // Generate a step
    cur_pos.x += dx * step;
    cur_pos.y += dy * step;
    cur_pos.z += dz * step;
    planner->current_pos = cur_pos;
    
    /*
    pc.printf("goal: %f %f %f\r\n", goal.x, goal.y, goal.z);
    pc.printf("curr: %f %f %f\r\n", cur_pos.x, cur_pos.y, cur_pos.z);
    pc.printf("d,step: %f %f %f %f\r\n", dx * step, dy, dz, step);
    pc.printf("dist, prev: %f %f\r\n", dist, prev_dist);
    */

    if (inv_kinematics(new_angles, cur_pos) != SUCCESS) {
        led2 = 1;
        return FAILURE;
    }
    
    /*
    pc.printf("actaangles: %f %f %f\r\n", planner->angles_actual[0], planner->angles_actual[1], planner->angles_actual[2]);
    pc.printf("newangles: %f %f %f\r\n", new_angles[0], new_angles[1], new_angles[2]);
    */

    // Get the difference in angles
    for (int i = 0; i < 3; i++) {
        //planner->angles_ideal[i] = planner->angles_actual[i];
        new_angles[i] -= planner->angles_actual[i];
    }

    F32 max_diff = max(new_angles[0], new_angles[1], new_angles[2]);

    // Number of steps to move max_diff amount
    planner->steps_to_next = floor(max_diff / STEPPER_STEP_SIZE + 0.500001);
    if (planner->steps_to_next <= 0) {
        return SUCCESS;
    }
    
    //pc.printf("max, steps, size, raw %f %d %f %f\r\n", max_diff, planner->steps_to_next, STEPPER_STEP_SIZE, max_diff / STEPPER_STEP_SIZE + 0.500001);
    
    for (int i = 0; i < 3; i++) {
        planner->angles_step[i] = new_angles[i] / planner->steps_to_next;
    }
    
    planner->prev_dist = dist;

    return SUCCESS;
}

Status make_next_step(Planner* planner) {
    S32 make_step = 0;
    S32 direction = 0;
    if (planner->steps_to_next == 0)
        return SUCCESS;
    //static int counter = 0;
    //if (counter % 128 == 0) {
    //    counter = 1;
    //}
    
    // For all steppers, if the ideal angles are 1 step or greater from the actual angles,
    // move the steppers to them.
    for (int i = 0; i < 3; i++) {
        planner->angles_ideal[i] += planner->angles_step[i];
        
        // Set the bits for the stepper mover
        if (fabs(planner->angles_ideal[i] - planner->angles_actual[i]) >= STEPPER_STEP_SIZE) {
            make_step |= 1 << i;
            direction |= (planner->angles_ideal[i] > planner->angles_actual[i]) << i;
        }
    }
    
    if (move_steppers(make_step, direction) != SUCCESS) {
    /*
        pc.printf("FFFUUUUU Stepper failure\r\n");
        pc.printf("anglesa: %f %f %f\r\n", planner->angles_actual[0], planner->angles_actual[1], planner->angles_actual[2]);
        pc.printf("anglesi: %f %f %f\r\n", planner->angles_ideal[0], planner->angles_ideal[1], planner->angles_ideal[2]);
        pc.printf("position: %f %f %f\r\n", planner->current_pos.x, planner->current_pos.y, planner->current_pos.z);
        */
        led2 = 1;
        return FAILURE;
    }
    planner->steps_to_next--;
    return SUCCESS;
}

Timer timer;
void take_step() {
    bool reset_dist_flag;
    static bool firstrun = true;
    
    static int counter = 0;
    if (counter++ > 200) {
        led4 = led4 ^ 1;
        counter = 0;
    }
    
    led1 = 0;
    make_next_step(cur_plan);
    if (cur_plan->steps_to_next == 0) {
        if (cur_plan->buf_ind == cur_plan->buf_next) {
            cur_plan->finished = true;
            return;
        }
        
        reset_dist_flag = 0;
        if (firstrun) {
            firstrun = false;
            reset_dist_flag = 1;
            cur_plan->buf_ind = INC_ONE(cur_plan->buf_ind);
        }
        
        while (get_next_steps(cur_plan, reset_dist_flag) == END_PAT) {
            led1 = 1;
            reset_dist_flag = 1;
           
            if (INC_ONE(cur_plan->buf_ind) == cur_plan->buf_next) {
                cur_plan->finished = true;
                led1 = 0;
                return;
            }
            cur_plan->buf_ind = INC_ONE(cur_plan->buf_ind);
        }
    }
    //timer.stop();
    //pc.printf("Timer: %d\r\n", timer.read_us());
}
