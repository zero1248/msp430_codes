/*
 * Breath_Lamp_Event.c
 *
 *  Created on: 2015-5-25
 *      Author: Isaac
 */
#include"msp430g2553.h"
#include"Breath_Lamp_Event.h"
#include"TimerA_PWM.h"

static unsigned int bright = 0;

/********************************************************
*名       称：WDT_Ontime()
*功       能：WDT定时中断事件处理函数
*入口参数：无
*出口参数：无
*说       明：增强代码的移植性和可读性
********************************************************/
void WDT_Ontime(void)
{
	static int bright_delta = 0;
	TA0_PWM_SetPermill( 1 , bright );
	if( bright >= 400)
		bright_delta = -5;
	if( bright <= 10 )
		bright_delta = 5;
	bright += bright_delta;
	_NOP();
}


