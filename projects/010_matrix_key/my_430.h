/*
 *  my_430.h
 *  for msp430f149/169
 *  Created on: 2015-6-26
 *      Author: Isaac
 */

#ifndef __MY_430_H__
#define __MY_430_H__

#define    WDT_OUT  WDTCTL = WDTPW + WDTHOLD
#define    P1_INIT P1DIR = 0xff;P1OUT = 0xff
#define	   P2_INIT P2DIR = 0xff;P2OUT = 0xff
#define	   P3_INIT P3DIR = 0xff;P3OUT = 0xff
#define	   P4_INIT P4DIR = 0xff;P4OUT = 0xff
#define	   P5_INIT P5DIR = 0xff;P5OUT = 0xff
#define    P6_INIT P6DIR = 0xff;P6OUT = 0xff
#define    IO_INIT P1_INIT;P2_INIT;P3_INIT;P4_INIT;P5_INIT;P6_INIT

#define    uint unsigned int
#define    uchar unsigned char

void Clk_Init(void);

#endif
