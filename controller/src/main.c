#include <msp430g2452.h>

#include "i2c.h"
#include "wheel.h"

char I2C_data;

void setup() {
	// Stop watchdog
	WDTCTL = WDTPW + WDTHOLD;
	if (CALBC1_8MHZ ==0xFF || CALDCO_8MHZ == 0xFF) {  
		// If calibration constants erased
		// do not load, trap CPU!!
		while(1) P1OUT ^= 0x01; 
	}

	BCSCTL1 |= DIVA_0;                    // ACLK/(0:1,1:2,2:4,3:8)
	BCSCTL3 |= LFXT1S_2;                  // LFXT1 = VLO  

	// Port init
	P1OUT &= ~(BIT3+BIT4+BIT5+BIT0);
	P1DIR |= BIT3+BIT4+BIT5+BIT0;
	P2SEL = 0x00;                         // No XTAL
	P2DIR |= (BIT0+BIT4+BIT2+BIT3+BIT1+BIT5);
	P2OUT &= ~(BIT0+BIT4+BIT2+BIT3+BIT1+BIT5);

	// Set DCO
	BCSCTL1 = CALBC1_8MHZ;                
  	DCOCTL = CALDCO_8MHZ;
  	BCSCTL2 |= DIVS_3;
}

void main(void) {
	setup();	
	wheel_setup();
	i2c_setup();

	while(1) {
		// CPU off, await USI interrupt
		// LPM0;
		I2C_data = CapTouchActiveMode();
	}
}
