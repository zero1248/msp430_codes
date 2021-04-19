/*
 * ADC12.c
 *
 *  Created on: 2015-11-4
 *      Author: Isaac
 */
#include "ADC12.h"
#include<msp430x16x.h>
//#define   Num_of_Results   32
//unsigned int results[Num_of_Results];    //保存ADC转换结果的数组
//static unsigned int index = 0;
unsigned int ADC12_value0 = 0;
float Voltage = 0.0;
float angle = 0.0;

/********************************************
函数名称：Init_ADC
功       能：初始化ADC，A0单通道（P6.0）
入口参数：无
出口参数：无
说       明：已经使能总中断，使用内部时钟八分频，1024周期采样（最慢）
********************************************/
void Init_ADC(void)
{
    P6SEL |= 0x01;                            // 使能ADC通道P6.0
    ADC12CTL0 = ADC12ON + SHT0_12 + MSC + REF2_5V + REFON;      // 打开ADC+设置采样周期1024(最长)
	                                          // +只需一次SHI信号触发
    ADC12CTL1 = SHP + SHS_0 + CONSEQ_2 + ADC12DIV_7;// 采样定时器作为控制信号源+取样保持源选为ADC12SC
                                              //+重复单通道+8分频
	ADC12IE = 0x01;                           // 使能ADC中断
	ADC12MCTL0 = INCH_0 + SREF_1;             //通道0+使用内部参考源
	_EINT();
    ADC12CTL0 |= ENC;                         // 使能转换
    ADC12CTL0 |= ADC12SC;                     // 开始转换
}

/***********************************************************
*名       称：Start_ADC12()
*功       能：ADC12开始转换
*入口参数：无
*出口参数：无
*说       明：使能ADC并发出开始信号
***********************************************************/
void Start_ADC12(void){
	ADC12CTL0 |= ENC;                         // 使能转换
    ADC12CTL0 |= ADC12SC;                     // 开始转换
}

/***********************************************************
*名       称：ADC12_Function()
*功       能：ADC12功能函数，在ADC12中断中调用
*入口参数：无
*出口参数：无
*说       明：
***********************************************************/
static ADC12_Function(void)
{
	ADC12_value0 = ADC12MEM0;
	Voltage = (2.5 / 4096) * ADC12_value0;
	angle = Voltage * 20;

}

/*******************************************
函数名称：ADC12ISR
功       能：ADC中断服务函数，在这里用多次平均的
          计算P6.0口的模拟电压数值
入口参数：无
出口参数：无
说      明 ：
********************************************/
#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
{
	ADC12_Function();
}




