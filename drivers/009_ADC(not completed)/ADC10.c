/*
 * ADC10.c
 *
 *  Created on: 2015-5-14
 *      Author: Isaac
 */
#include<msp430.h>
#include"ADC10.h"

unsigned int ADC_value;

/***********************************************************
*名       称：ADC10_Init()
*功       能：ADC10初始化
*入口参数：无
*出口参数：无
*说       明：开中断|开ADC|开内部参考|2.5V|4倍时钟采样|2.5V和GND作参考
		  单通道重复转换|SMCLK|不分频|开始信号ADC10SC|A0通道
		  需要在main函数中使能总中断
***********************************************************/
void ADC10_Init(void)
{
    ADC10CTL0 &= ~ENC;
    ADC10CTL0 = ADC10IE | ADC10ON | REFON | REF2_5V | ADC10SHT_0 | SREF_1;
    ADC10CTL1 = CONSEQ_2 | ADC10SSEL_3 | ADC10DIV_0 | SHS_0 | INCH_0;
    __delay_cycles(30000);//Software delay for REFON to settle
    ADC10CTL0 |= ENC;//enable ADC10
}

/***********************************************************
*名       称：Start_ADC10()
*功       能：ADC10开始转换
*入口参数：无
*出口参数：无
*说       明：使能ADC并发出开始信号
***********************************************************/
void Start_ADC10(void)
{
	ADC10CTL0 |= ENC;                         // 使能转换
    ADC10CTL0 |= ADC10SC;                     // 开始转换
}

/***********************************************************
*名       称：Sop_ADC10()
*功       能：ADC10开始转换
*入口参数：无
*出口参数：无
*说       明：复位使能信号
***********************************************************/
void Stop_ADC10(void)
{
	ADC10CTL0 &= ~ENC;
}

/***********************************************************
*名       称：ADC10_Function()
*功       能：ADC10功能函数，在ADC10中断中调用
*入口参数：无
*出口参数：无
*说       明：
***********************************************************/
static void ADC10_Function(void)
{
	ADC_value = ADC10MEM;
}

/*******************************************
函数名称：ADC10ISR
功    能：ADC中断服务函数，
参    数：无
返回值  ：无
说   明  ：
********************************************/
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR_HOOK(void)
{
	ADC10_Function();
}
