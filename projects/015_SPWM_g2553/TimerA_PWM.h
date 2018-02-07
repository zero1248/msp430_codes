/*
 * TimerA_PWM.h
 *
 *  Created on: 2015-5-21
 *      Author: Isaac
 */

#ifndef __TIMERA_PWM_H__
#define __TIMERA_PWM_H__

#define DEADTIME 20  //ËÀÇøÊ±¼ä
#define TA01_SET P1SEL|=BIT6;P1DIR|=BIT6 //P1.6
#define TA11_SET P2SEL|=BIT2;P2DIR|=BIT2 //P2.2
#define TA12_SET P2SEL|=BIT4;P2DIR|=BIT4 //P2.4
#define TA01_OFF P1SEL&=~BIT6 //P1.6
#define TA11_OFF P2SEL&=~BIT2 //P2.2
#define TA12_OFF P2SEL&=~BIT4 //P2.4

extern char TA0_PWM_Init(char clk,char div,char mode1);
extern char TA0_PWM_SetPeriod(unsigned int period);
extern char TA0_PWM_SetPermill(char channel,unsigned int duty);
extern char TA1_PWM_Init(char clk,char div,char mode1,char mode2);
extern char TA1_PWM_SetPeriod(unsigned int period);
extern char TA1_PWM_SetPermill(char channel,unsigned int duty);

#endif /* TIMERA_PWM_H_ */
