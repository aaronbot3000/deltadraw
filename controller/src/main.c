#include <msp430g2452.h>

#include "i2c.h"

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;            // Stop watchdog
  if (CALBC1_12MHZ ==0xFF || CALDCO_12MHZ == 0xFF)                                     
  {  
    while(1);                          // If calibration constants erased
                                       // do not load, trap CPU!!
  }   
  BCSCTL1 = CALBC1_12MHZ;               // Set DCO
  DCOCTL = CALDCO_12MHZ;

  //P1OUT = 0xC0;                        // P1.6 & P1.7 Pullups
  //P1REN |= 0xC0;                       // P1.6 & P1.7 Pullups
  P1DIR = 0xFF;                        // Unused pins as outputs
  P2OUT = 0;
  P2DIR = 0xFF;
  
  i2c_setup();

  while(1)
  {
    LPM0;                              // CPU off, await USI interrupt
    _NOP();                            // Used for IAR
  }
}
