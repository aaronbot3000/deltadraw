#include <msp430g2452.h>

#include "i2c.h"

void setup() {
	// Stop watchdog
	WDTCTL = WDTPW + WDTHOLD;
	if (CALBC1_12MHZ ==0xFF || CALDCO_12MHZ == 0xFF) {  
		// If calibration constants erased
		// do not load, trap CPU!!
		while(1) P1OUT ^= 0x01; 
	}
	
	// Set DCO
	BCSCTL1 = CALBC1_12MHZ;
	DCOCTL = CALDCO_12MHZ;

	// Unused pins as outputs
	P1DIR = 0xFF;
	P2OUT = 0;
	P2DIR = 0xFF;
}

void main(void) {
	setup();	

	i2c_setup();

	while(1) {
		// CPU off, await USI interrupt
		LPM0;
	}
}
