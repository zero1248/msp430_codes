#include <msp430.h> 

/*
 * main.c
 */
#include<msp430g2553.h>
#include"SPWM.h"
#include"TimerA_PWM.h"

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	BCSCTL1 = CALBC1_12MHZ;     //Set range
	DCOCTL = CALDCO_12MHZ;      //Set DCO step+modulation
	DCOCTL = DCOCTL&0xE0;       //关闭混频器，得到纯净频率
	P1DIR |= BIT6;              //P1.6将来作为示波器的同步信号
	//-----初始化TA1的PWM输出-----
	TA1_PWM_Init('S',1,'F',0);  //TA时钟设为SMCLK，通道1超前PWM输出，通道2不作TA用
	TA1_PWM_SetPeriod(128);     //PWM的周期为128个时钟
	//--------ADC10初始化--------
	ADC10CTL0 = SREF_1+ADC10SHT_3+REF2_5V+REFON+REFOUT+ADC10ON;
	ADC10CTL1 = INCH_0+CONSEQ_0;
	//--------定时器初始化---------
	Timer0_A_Init();
	_EINT();
	while(1)
	{
		ADC10_WaveSample();
		_NOP();
	}
}

