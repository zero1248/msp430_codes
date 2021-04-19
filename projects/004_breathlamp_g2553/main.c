#include <msp430.h> 
#include"Breath_Lamp_Event.h"
#include"TimerA_PWM.h"

#define PWM_PERIOD 100

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	TA0_PWM_Init( 'A',1,'F');
	TA0_PWM_SetPeriod(PWM_PERIOD);
	WDTCTL = WDT_ADLY_16;
	IE1 |= WDTIE;
	_EINT();
	LPM3;
}

/********************************************************
*名       称：WDT_ISR()
*功       能：相应WDT定时中断服务
*入口参数：无
*出口参数：无
*说       明：WDT定时中断独占中断向量，无需判断，无需人工清除标志位
********************************************************/
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
	WDT_Ontime();
}
