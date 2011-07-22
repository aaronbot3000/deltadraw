#ifndef WHEEL_H_
#define WHEEL_H_

#include "CTS_Layer.h"

#define INVALID_GESTURE         0xFD
#define GESTURE_START           0xFC
#define GESTURE_STOP            0xFB
#define COUNTER_CLOCKWISE       1
#define CLOCKWISE               2
#define GESTURE_POSITION_OFFSET 0x20
#define WHEEL_POSITION_OFFSET   0x30

#define WHEEL_TOUCH_DELAY		    12			    //Delay between re-sendings of touches
#define MAX_IDLE_TIME           200

/*----------------- LED definition---------------------------------------------
 * There are 8 LEDs to represent different positions around the wheel. They are 
 * controlled by 5 pins of Port 1 using a muxing scheme. The LEDs are divided 
 * vertically into two groups of 4, in which each LED is paired up [muxed] with
 * the LED mirrored on the other side of the imaginary center vertical line via 
 * the use of pin P1.3 and one specific port pin. 
 * Specifically, the pairs are LEDs [0,7], [1,6], [2,5], [3,4], as shown in the 
 * diagram below.
 *     LED                        Position (degrees, clockwise)
 * --RIGHT SIDE--
 *      0       BIT4,!BIT3                  45
 *      1       BIT5,!BIT3                  80
 *      2       BIT6,!BIT3                 100
 *      3       BIT7,!BIT3                 135 *
 * 
 * --LEFT SIDE--
 *      4       BIT3,(BIT4,5,6)             225
 *      5       BIT3,(BIT4,5,7)             260
 *      6       BIT3,(BIT4,6,7)             280
 *      7       BIT3,(BIT5,6,7)             315
 *----------------------------------------------------------------------------*/
#define MASK7                   BIT4
#define MASK6                   BIT5
#define MASK5                   BIT6
#define MASK4                   BIT7

#define MASK3                   (BIT3+BIT4+BIT5+BIT6)
#define MASK2                   (BIT3+BIT4+BIT5+BIT7)
#define MASK1                   (BIT3+BIT4+BIT6+BIT7)
#define MASK0                   (BIT3+BIT5+BIT6+BIT7)



void wheel_setup();
char CapTouchActiveMode();

#endif /*WHEEL_H_*/
