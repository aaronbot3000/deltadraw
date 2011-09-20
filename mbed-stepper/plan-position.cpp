#include "plan-position.h"

static volatile Point goal;
static volatile bool new_goal = false;
static volatile bool met_goal = true;

static Point cur_pos;
static Planner_State state = PLR_ACCL;
static F32 ideal_angles[3];
static F32 prev_dist;
static F32 full_dist;

extern Serial pc;

inline F32 dist_between(Point a, Point b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
}

inline void conform_goal(volatile Point* in) {
    in->x = RESTRICT(in->x, MIN_X, MAX_X);
    in->y = RESTRICT(in->y, MIN_Y, MAX_Y);
    in->z = RESTRICT(in->z, MIN_Z, MAX_Z);
}

inline F32 max(F32 a, F32 b) {
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

void catch_interrupt() {
    if (met_goal && !new_goal) {
        return;
    }
    else if (met_goal && new_goal) {
        //update_pos();
        fwd_kinematics(&cur_pos, get_angles());
        pc.printf("position: %f %f %f\r\n", cur_pos.x, cur_pos.y, cur_pos.z);
        full_dist = prev_dist = dist_between(cur_pos, goal);
        met_goal = false;
        take_step();
    }
    else if (!met_goal) {
        take_step();
    }
}

void set_goal(const Point in) {
    (Point)goal = in;
    new_goal = true;
}

void reset_pen() {
    goal.x = 0;
    goal.y = 0;
    goal.z = START_Z;
    state = PLR_ACCL;
    full_dist = 0;
    prev_dist = 0;
    new_goal = true;
    
    
    cur_pos.x = 0;
    cur_pos.y = 0;
    cur_pos.z = START_Z;
    ideal_angles[0] = 0;
    ideal_angles[1] = 0;
    ideal_angles[2] = 0;
    reset_steppers();
}

bool robot_met_goal() {
    return met_goal;
}
Timer timer;
Status take_step() {
    //pc.printf("goal: %f %f %f\r\n", goal.x, goal.y, goal.z);
    timer.reset();
    timer.start();
    conform_goal(&goal);

    F32 dx = goal.x - cur_pos.x;
    F32 dy = goal.y - cur_pos.y;
    F32 dz = goal.z - cur_pos.z;

    F32 dist = dist_between(cur_pos, goal);
    F32 new_angles[3];
    F32 step = 0;
    F32* actual_angles = get_angles();
    Point test;
    
    if (dist < MIN_DIST)
        return SUCCESS;

    // Inverse square root!!!
    F32 inv_vec_mag = 1 / dist;
    dx *= inv_vec_mag;
    dy *= inv_vec_mag;
    dz *= inv_vec_mag;
    
    // Apply acceleration and deceleration and
    // acquire next step size
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
        Point test;
        step = MAP(prev_dist, 0, ACCL_ZONE, MIN_STEP_SIZE, MAX_STEP_SIZE);

        test.x = cur_pos.x + dx * EST_STEP_SIZE * 0.5;
        test.y = cur_pos.y + dy * EST_STEP_SIZE * 0.5;
        test.z = cur_pos.z + dz * EST_STEP_SIZE * 0.5;
        
        dist = dist_between(test, goal);
        
        if (dist >= prev_dist || dist < MIN_DIST) {
            met_goal = true;
            return SUCCESS;
        }
    }
    
    // Generate an overestimated step
    test.x = cur_pos.x + dx * EST_STEP_SIZE;
    test.y = cur_pos.y + dy * EST_STEP_SIZE;
    test.z = cur_pos.z + dz * EST_STEP_SIZE;
    
    pc.printf("test: %f %f %f\r\n", test.x, test.y, test.z);

    inv_kinematics(new_angles, test);
    
    pc.printf("newangles: %f %f %f\r\n", new_angles[0], new_angles[1], new_angles[2]);

    // Get the difference in angles
    for (int i = 0; i < 3; i++) {
        new_angles[i] -= ideal_angles[i];
    }

    F32 max_diff = max(new_angles[0], new_angles[1], new_angles[2]);
    
    // Scale down so the largest step = 1 stepper step size
    F32 scaling_factor = 1;
    if (max_diff > STEPPER_STEP_SIZE)
        scaling_factor = STEPPER_STEP_SIZE * step / max_diff;
        
    printf("max diff: %f scaling factor %f\r\n", max_diff, scaling_factor);

    int direction = 0;
    int make_step = 0;

    // For all steppers, if the ideal angles are 1 step or greater from the actual angles,
    // move the steppers to them.
    for (int i = 0; i < 3; i++) {
        new_angles[i] *= scaling_factor;
        ideal_angles[i] += new_angles[i];
        
        // Set the bits for the stepper mover
        if (fabs(new_angles[i] - actual_angles[i]) >= STEPPER_STEP_SIZE) {
            make_step |= 1 << i;
            direction |= (new_angles[i] > actual_angles[i]) << i;
        }
    }

    if (move_steppers(direction, make_step) != SUCCESS) {
        pc.printf("FFFUUUUU Stepper failure\r\n");
        return FAILURE;
    }
        
    fwd_kinematics(&cur_pos, ideal_angles);

    prev_dist = dist_between(cur_pos, goal);
    
    timer.stop();
    //pc.printf("Timer: %d\r\n", timer.read_us());
    pc.printf("position: %f %f %f\r\n", cur_pos.x, cur_pos.y, cur_pos.z);
    pc.printf("dist: %f\r\n", prev_dist);
    pc.printf("angles: %f %f %f\r\n", ideal_angles[0], ideal_angles[1], ideal_angles[2]);
    pc.printf("aangles: %f %f %f\r\n", actual_angles[0], actual_angles[1], actual_angles[2]);

    return SUCCESS;
}
