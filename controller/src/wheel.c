#include "wheel.h"

static unsigned int wheel_position=ILLEGAL_SLIDER_WHEEL_POSITION, last_wheel_position=ILLEGAL_SLIDER_WHEEL_POSITION;
//static unsigned int deltaCnts[1];
//static unsigned int prox_raw_Cnts;

void wheel_setup() {
	P1OUT = BIT0;

	TI_CAPT_Init_Baseline(&wheel);
	TI_CAPT_Update_Baseline(&wheel,2);
}

/* ----------------GetGesture----------------------------------------------
 * Determine immediate gesture based on current & previous wheel position
 * ------------------------------------------------------------------------*/
unsigned char GetGesture(unsigned char wheel_position) {
	unsigned char gesture = INVALID_GESTURE, direction, ccw_check, cw_check; 
	// ******************************************************************************
	// gesturing
	// determine if a direction/swipe is occuring
	// the difference between the initial position and
	// the current wheel position should not exceed 8
	// 0-1-2-3-4-5-6-7-8-9-A-B-C-D-E-F-0...
	//
	// E-F-0-1-2:  cw, 4
	// 2-1-0-F-E: ccw, 4
	// A-B-C-D-E-F

	//if(initial_wheel_position == INVALID_WHEEL_POSITION)
	//{
	//gesture = 0;
	//initial_wheel_position = wheel_position;
	//}
	//else

	if(last_wheel_position != ILLEGAL_SLIDER_WHEEL_POSITION) {
		if(last_wheel_position  > wheel_position) {
			// E-D-C-B-A:  ccw, 4
			// counter clockwise: 0 < (init_wheel_position - wheel_position) < 8
			//                    gesture = init_wheel_position - wheel_position
			//
			// E-F-0-1-2:  cw, 4
			// clockwise:        0 < (init_wheel_position+wheel_position)-16 <8
			//                    
			ccw_check = last_wheel_position  - wheel_position;
			if(ccw_check < 8) {
				gesture = ccw_check;
				direction = COUNTER_CLOCKWISE;
			}
			else {
				// E-F-0-1-2:  cw, 4
				// 16 - 14 + 2 = 4
				cw_check = 16 - last_wheel_position  + wheel_position ;
				if(cw_check < 8)
				{
					gesture = cw_check;
					direction = CLOCKWISE;
				}
			}
		}
		else {
			// initial_wheel_position <= wheel_position
			//
			// 2-1-0-F-E: ccw, 4
			// counter clockwise: 
			//                    0 < (init_wheel_position+wheel_position)-16 <8
			//                    gesture = init_wheel_position - wheel_position
			//
			// 0-1-2-3-4:  cw, 4
			// clockwise:        0 < (wheel_position - init_wheel_position) < 8
			//    
			cw_check = wheel_position - last_wheel_position ;
			if(cw_check < 8) {
				gesture = cw_check;
				direction = CLOCKWISE;
			}
			else {
				// 2-1-0-F-E: ccw, 4
				// 16 + 2 - 14 = 4
				ccw_check = 16 + last_wheel_position  - wheel_position;
				if(ccw_check < 8) {
					gesture = ccw_check;
					direction = COUNTER_CLOCKWISE;
				}
			}
		}
	} 
	if (gesture == INVALID_GESTURE)
		return gesture;
	if (direction == COUNTER_CLOCKWISE)
		return (gesture + 16);
	else
		return gesture;
}

/* ----------------CapTouchActiveMode----------------------------------------------
 * Determine immediate gesture based on current & previous wheel position
 * -------------------------------------------------------------------------------*/
char CapTouchActiveMode() {
	unsigned char idleCounter, activeCounter;
	unsigned char gesture, gestureDetected; 
	char ret_val = 0;

	gesture = INVALID_GESTURE;            // Wipes out gesture history

	idleCounter = 0;      
	activeCounter = 0;
	gestureDetected = 0;

	while (idleCounter++ < MAX_IDLE_TIME) {  
		// Set DCO to 8MHz
		// SMCLK = 8MHz/8 = 1MHz
	
	   //BCSCTL1 = CALBC1_8MHZ;     
	   //DCOCTL = CALDCO_8MHZ;
	   //BCSCTL2 |= DIVS_3;
		     
		TACCTL0 &= ~CCIE;   

		wheel_position = ILLEGAL_SLIDER_WHEEL_POSITION;       
		wheel_position = TI_CAPT_Wheel(&wheel);

		// Process wheel touch/position/gesture  if a wheel touch is registered
		// Wheel processing has higher priority than center button

		if(wheel_position != ILLEGAL_SLIDER_WHEEL_POSITION) {
			// Indicate a touch recognized
			P1OUT |= BIT0;

			// Adjust wheel position based: rotate CCW by 2 positions
			if (wheel_position < 0x08) {
				wheel_position += 0x40 - 0x08;
			}
			else {
				wheel_position -= 0x08;
				// Adjust wheel position based: rotate CCW by 2 positions
			}

			wheel_position = wheel_position >>2;  // divide by four

			gesture = GetGesture(wheel_position);            

			/* Add hysteresis to reduce toggling between wheel positions if no gesture 
			 * has been TRULY detected. */  

			if ( (gestureDetected==0) && ((gesture<=1) || (gesture==0x11) || (gesture==0x10))) {
				if (last_wheel_position != ILLEGAL_SLIDER_WHEEL_POSITION)
					wheel_position = last_wheel_position;
				gesture = 0;
			}

			// Turn on corresponding LED(s)


			if ((gesture != 0) && (gesture != 16) && (gesture != INVALID_GESTURE)) {
				// A gesture has been detected 
				if (gestureDetected ==0) {
					// Starting of a new gesture sequence
					gestureDetected = 1;
				} 

				ret_val = wheel_position - last_wheel_position;
			}      

			idleCounter = 0;                      // Reset idle counter
			activeCounter++;
			last_wheel_position = wheel_position;
		} 
		else {
			// no wheel position was detected 

			P1OUT &= ~BIT0;       

			// Reset all touch conditions, turn off LEDs, 
			last_wheel_position= ILLEGAL_SLIDER_WHEEL_POSITION;      
			gesture = INVALID_GESTURE;
			gestureDetected = 0;

		} 
	} 
	return ret_val;
}
