/*
 * my_430.c
 *
 *  Created on: 2015-5-1
 *      Author: user
 */
#include"my_430.h"


#ifdef MSP430_G2553
#include<msp430g2553.h>
void Port_Init(void)
{
	P1DIR = 0xff;
	P1OUT = 0xff;
	P2DIR = 0xff;
	P2OUT = 0xff;
}
#endif

#ifdef MSP430_F149
#include<msp430x14x.h>
void Port_Init(void)
{
	P1DIR = 0xff;P1OUT = 0xff
	P2DIR = 0xff;P2OUT = 0xff
	P3DIR = 0xff;P3OUT = 0xff
	P4DIR = 0xff;P4OUT = 0xff
	P5DIR = 0xff;P5OUT = 0xff
	P6DIR = 0xff;P6OUT = 0xff
}
#endif

#ifdef MSP430_F169
#include<msp430x16x.h>
void Port_Init(void)
{
	P1DIR = 0xff;P1OUT = 0xff
	P2DIR = 0xff;P2OUT = 0xff
	P3DIR = 0xff;P3OUT = 0xff
	P4DIR = 0xff;P4OUT = 0xff
	P5DIR = 0xff;P5OUT = 0xff
	P6DIR = 0xff;P6OUT = 0xff
}
#endif
