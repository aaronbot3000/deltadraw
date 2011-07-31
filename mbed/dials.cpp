#include "dials.h"

// trololololol
extern DigitalIn go_troll_up;
extern DigitalIn go_troll_dn;
extern DigitalIn go_adj_z;
extern DigitalIn go_run_pat;

static S08 at_draw = 0, debounce = 0;
void read_dials(Planner* planner, F32 moves_z, F32 draw_z) {
    F32 adj_x = 0, adj_y = 0;
    Point next;
    U08 in_x, in_y, zch = 0;
        
    next = planner->next_pos;
        
    // Debounce the switches
    if (go_adj_z && debounce < 10) {
        debounce++;
        if (debounce == 10) {
            if (at_draw) {
                next.z = moves_z;
                at_draw = 0;
            }
            else {
                next.z = draw_z;
                at_draw = 1;
            }
            zch = 1;
        }
    }
    if (!go_adj_z && debounce > 0)
        debounce--;
    
    in_x = i2c_read(PERIP_X);
    in_y = i2c_read(PERIP_Y);
    if (in_x != 128 && in_x != I2C_ERROR) {
        adj_x = ((F32)RESTRICT(in_x - 128, -16, 16))/64.0;
        //pc.printf("input: %0.5f\n", adj_x);
        next.x += adj_x;
    }
    if (in_y != 128 && in_y != I2C_ERROR) {
        adj_y = ((F32)RESTRICT(in_y - 128, -16, 16))/64.0;
        //pc.printf("input: %0.5f\n", adj_y);
        next.y += adj_y;
    }
    if ((adj_x != 0 ||adj_y != 0 || zch) && get_num_in_buffer(planner) < 1) {
        //pc.printf("added to buffer");
        add_point_to_buffer(planner, next);
    }
}
