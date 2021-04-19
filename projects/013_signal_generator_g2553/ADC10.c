/*
 * ADC10.c
 *
 *  Created on: 2015-5-14
 *      Author: Isaac
 */
#include<msp430.h>
#include"ADC10.h"
#include"OLED.h"
unsigned int ADC_value = 0;
unsigned int ADC_precise_value = 0;
unsigned char sample_times = 0;
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
    P1DIR &= ~BIT0; //A0通道(P1.0)
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
	unsigned int data;
	char current[4]={0,0,0,0};
	char power[4]={0,0,0,0};
	ADC_value = ADC10MEM;
	if(ADC_value >327)P1OUT |= BIT4;
	ADC_precise_value += ADC_value;
	if(sample_times == 20)
	{
		sample_times =0;
		ADC_precise_value /= 20;
		data = ( ADC_precise_value )*2500/1024;
		current[0] = (unsigned char)(data / 10) + '0';
		current[1] = (unsigned char)(data % 10) + '0';
		current[2] = '0';
		current[3] = '\0';
		OLED_Display_8x16 (32,4,current);
		data *= 5;
		power[0] = (unsigned char)(data / 100) + '0';
		power[1] = '.';
		power[2] = (unsigned char)((data % 100)/10) + '0';
		power[3] = '\0';
		OLED_Display_8x16 (32,6,power);
		ADC_value = 0;
		ADC_precise_value = 0;
	}
}


#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR_HOOK(void)
{
	Stop_ADC10();
	sample_times++;
	ADC10_Function();


//	if(sample_times == 3)
//	{
//		sample_times =0;
//		ADC_precise_value /= 3;
//		if(ADC_precise_value >327)
//			P1OUT |= BIT4;
//		ADC_value = 0;
//		ADC_precise_value = 0;

}
