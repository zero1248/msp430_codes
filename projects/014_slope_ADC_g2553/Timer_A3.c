/*
 * Timer_A3.c
 *
 *  Created on: 2015-5-21
 *      Author: Isaac
 */

#include<msp430.h>
#include"Timer_A3.h"
#include"slope.h"

void Timer0_A3_Init(void)
{
	//-----------捕获模式-----------
	TA0CTL = TASSEL_2 + ID_0 + MC_2; //连续计数开始
	TA0CCTL1 = CM_1 +CCIS_1 + SCS + CAP + OUTMOD_0 + CCIE;
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR_HOOK(void)
{
	switch( __even_in_range(TA0IV,TA0IV_TAIFG) )
	{
	case TA0IV_TACCR1 : Slope_TA_CCI1B(); break;
	case TA0IV_TACCR2 :                   break;
	case TA0IV_TAIFG :                    break;
	default : break;
	}
}


