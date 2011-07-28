#include "wheel.h"

static short pos_filt, last_pos_filt, last_pos, need_init;
static int no_wrap;

void wheel_setup() {
	P1OUT = BIT0;

	TI_CAPT_Init_Baseline(&wheel);
	TI_CAPT_Update_Baseline(&wheel,2);
	need_init = 1;
}

// Read the wheel position, filter it, and add hyteresis before returning
// for super smooth etch-a-sketch operation
// Returns a delta position, not absolute position
char get_wheel_reading() {
	short ret_val = 0;
	short diff, reading, i;

	reading = TI_CAPT_Wheel(&wheel);


	if (reading != ILLEGAL_SLIDER_WHEEL_POSITION) {
		if (need_init) {
			no_wrap = reading >> 2;
			for (i = 0; i < 3; i++) {
				no_wrap += TI_CAPT_Wheel(&wheel) >> 2;
			}
			last_pos = no_wrap;
			pos_filt = no_wrap;
			last_pos_filt = ILLEGAL_SLIDER_WHEEL_POSITION;
			need_init = 0;
		}
		else {
			diff = reading - last_pos;
			if (diff > 32 && reading > 48 && last_pos < 16)
				diff -= 64;
			else if (diff < -32 && reading < 48 && last_pos > 16)
				diff += 64;
			
			no_wrap += diff;
			last_pos = reading;
			
			last_pos_filt = pos_filt;
			pos_filt = (3 * pos_filt) >> 2;
			pos_filt += no_wrap >> 2;
			
			if (pos_filt >= 255) {
				pos_filt -= 127;
				last_pos_filt -= 127;
				no_wrap -= 127;
			}
			if (pos_filt <= 0) {
				pos_filt += 128;
				last_pos_filt += 128;
				no_wrap += 128;
			}
		}
		
		// Indicate a touch recognized
		P1OUT |= MASK6 | MASK7;

		if (last_pos_filt != ILLEGAL_SLIDER_WHEEL_POSITION) {
			ret_val = pos_filt - last_pos_filt;
		}
		else
			ret_val = 0;
	} 
	// no wheel position was detected 
	else {
		P1OUT &= ~(MASK6 | MASK7);
		
		need_init = 1;
	} 
	// saturate ret_val into a signed char, just in case
	if (ret_val > 127)
		ret_val = 127;
	if (ret_val < -128)
		ret_val = -128;
	return ret_val;
}
