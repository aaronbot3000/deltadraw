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
char get_wheel_reading() {
	short ret_val = 0x7F;
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
			if (diff > 32)
				diff -= 64;
			else if (diff < -32)
				diff += 64;
			
			no_wrap += diff;
			last_pos = reading;
			
			pos_filt = (3 * pos_filt) >> 2;
			pos_filt += no_wrap >> 2;
		}	
		
		// Indicate a touch recognized
		P1OUT |= MASK6 | MASK7;

		if (last_pos_filt == ILLEGAL_SLIDER_WHEEL_POSITION) {
			ret_val = pos_filt + 2;
			ret_val &= 0xFFFC;
			if (ret_val > 31)
				ret_val -= 32;
		}
		else {
			diff = pos_filt - last_pos_filt;
			// Handle wrap-around
			if (diff > 16) {
				diff -= 32;
			}
			else if (diff < -16) {
				diff += 32;
			}
			if (diff > HYSTERESIS) {
				ret_val = pos_filt;
				ret_val &= 0xFFFC;
				if (ret_val > 31)
					ret_val -= 32;
			}
			else if (diff < -HYSTERESIS) {
				ret_val = pos_filt + 3;
				ret_val &= 0xFFFC;
				if (ret_val < 0)
					ret_val += 32;
			}
			else
				ret_val = 0;
		}
		//ret_val >>= 1;
		ret_val = pos_filt;
	} 
	// no wheel position was detected 
	else {
		P1OUT &= ~(MASK6 | MASK7);
		
		need_init = 1;
	} 
	return ret_val;
}
