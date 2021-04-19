/*
 * SPWM.c
 *
 *  Created on: 2015-5-24
 *      Author: Isaac
 */

#include<msp430g2553.h>
#include"SPWM.h"
#include"TimerA_PWM.h"

unsigned int Table_Value[64] = {0};

//-----------各种正弦表-------------
const unsigned int Sin_Table[SIN_NUM] = {
64,88,108,122,127,122,108,
88,64,39,19,5,0,5,19,39};

/********************************************************
*名       称：ADC10_WaveSample()
*功       能：完成一次采样
*入口参数：无
*出口参数：无
*说       明：共采样64个点
********************************************************/
void ADC10_WaveSample(void)
{
	int j = 0,i = 0;
	for(j=0 ; j<64 ; j++)
	{
		ADC10CTL0 |= ENC +ADC10SC;
		for(i=0 ; i<300 ; i++);    //延时
		Table_Value[j] = ADC10MEM;
	}
}

/********************************************************
*名       称：Timer0_A_Init()
*功       能：TA0初始化
*入口参数：无
*出口参数：无
*说       明：TA0用于改变占空比，其实就是改变整个SPWM的频率
********************************************************/
void Timer0_A_Init(void)
{
	TA0CCTL0 = CCIE;                   //允许比较/捕获模块0的中断
	TA0CCR0 = CSMCLK / SIN_F / SIN_NUM;//配置合适的查表定时时间
	TA0CTL = TASSEL_2 + MC_1;          //TA0设为增计数模式，时钟=SMCLK
	_EINT();
}

/********************************************************
*名       称：Change_Duty()
*功       能：TA0定时时间处理函数
*入口参数：无
*出口参数：无
*说       明：用正弦表定时改写TA1输出PWM的占空比
********************************************************/
void Change_Duty(void)
{
	static int i = 0;
	unsigned int cnt = 0;
	if( i >= SIN_NUM)
	{
		i = 0;
		P1OUT ^= BIT6; //作为示波器的同步信号输出，便于示波器观测SPWM信号
	}
	cnt = (unsigned int) Sin_Table[i++];
	TA1CCR1 = cnt;     //这里进行正弦波查表后，更改占空比
}

/********************************************************
*名       称：Timer_A0()
*功       能：TA0中断子函数
*入口参数：无
*出口参数：无
*说       明：定时中断内一般先关闭总中断
********************************************************/
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{
	_DINT();
	Change_Duty();
	_EINT();
}








