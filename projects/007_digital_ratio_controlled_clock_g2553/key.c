#include "key.h"
#include<msp430g2553.h>
#include"system_control.h"
static unsigned char LPM_flag = 0;
unsigned char getdata_flag = 0;

void Key_Init(void)
{
    P1IES |= BIT2+BIT3+BIT4+BIT5;               // P1.0选择下降沿中断
    P1IE  |= BIT2+BIT3+BIT4+BIT5;               // 打开P1.0中断使能
    P1IFG &= 0X00;
 }


static void P1Interrupt_ISR(void)
{

	if(P1IFG & BIT3)     //接收信号校准
	{
		getdata_flag = 1;
	}
	if(P1IFG & BIT4)
	{

	}
	if(P1IFG & BIT5)
	{

	}
}


/********************************************************
*名        称：P1_interrupt()
*功        能：P1口按键中断中断
*入口参数 ：无
*出口参数 ：无
*说        明：下降沿中断
********************************************************/
#pragma vector=PORT1_VECTOR
__interrupt void P1_interrupt(void)
{
	if(P1IFG & BIT2)   //进出低功耗模式
	{
		if(LPM_flag == 0)
		{
			LPM_flag = 1;
			LPM3;
		}
		if(LPM_flag == 1)
		{
			LPM_flag = 0;
			LPM3_EXIT;
		}
	}
	P1Interrupt_ISR();
	P1IFG &= 0x00;
}





