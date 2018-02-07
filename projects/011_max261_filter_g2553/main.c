#include<msp430.h>
#include"BCL.h"
#include"TimerA_PWM.h"
#include"Filter_Control.h"

/*
 * main.c
 */
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    BCS_Init();
    Filter_IO_Init();
    Filter_Clock_Set();
    while(1)
    {
    	//----Filter A----
    	FilterA_Mode_Set(1);
    	__delay_cycles(1000);
    	FilterA_F0_Set(7);
    	__delay_cycles(1000);
    	FilterA_Qvalue_Set(121);
    	__delay_cycles(1000);
    	//----Filter B----
    	FilterB_Mode_Set(1);
    	__delay_cycles(1000);
    	FilterB_F0_Set(0);
    	__delay_cycles(1000);
    	FilterB_Qvalue_Set(121);
    	__delay_cycles(1000);
    }
}
