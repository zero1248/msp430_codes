/*
 * my_430.h
 *
 *  Created on: 2015-5-1
 *      Author: Isaac
 */

#ifndef __MY_430_H__
#define __MY_430_H__

#define MSP430_G2553
//#define MSP430_F149
//#define MSP430_F169

#define WDT_OUT WDTCTL = WDTPW + WDTHOLD
extern void Port_Init(void);
extern void Clk_Init(void);

#endif /* MY_430_H_ */
