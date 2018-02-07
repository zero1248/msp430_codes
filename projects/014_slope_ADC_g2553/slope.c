
/*
 * slope.c
 *
 *  Created on: 2015-5-21
 *      Author: Isaac
 */
#include<msp430g2553.h>
#include"slope.h"
#include"Timer_A3.h"
#include"ComparatorA.h"

unsigned char slope_measure_flag = 0; //用于识别当前所测电阻
unsigned int r_REF = 0;               //参考电阻相对电阻值
unsigned int r_SENS = 0;              //传感器电阻相对电阻值

/***********************************************************
*名       称：Slope_Port_Charge()
*功       能：开启充电电源，一直将积分电容上的电充满
*入口参数：无
*出口参数：无
*说       明：充满电后等待放电信号
***********************************************************/
void Slope_Port_Charge(void)
{
	PORT_SENS_HIGH;
	PORT_REF_HIGH;
}

/***********************************************************
*名       称：Slope_Measure_Init()
*功       能：初始化各硬件模块，给电容充满电等待第一次放电测量
*入口参数：无
*出口参数：无
*说       明：
***********************************************************/
void Slope_Measure_Init(void)
{
	Comparator_Aplus_Init();
	Timer0_A3_Init();
	Slope_Port_Charge();
}

/***********************************************************
*名       称：Slope_Measure_REF()
*功       能：启动参考电阻测量
*入口参数：无
*出口参数：无
*说       明：
***********************************************************/
void Slope_Measure_REF(void)
{
	slope_measure_flag = 0;//表明此次测量参考电阻
	PORT_SENS_HZ;          //传感器电阻高阻态，防止干扰
	TAR_CLEAR;             //TA主时钟清零
	PORT_REF_LOW;          //开启参考电阻放电通道，开始计数测量放电时间
}

/***********************************************************
*名       称：Slope_Measure_SENS()
*功       能：启动传感器电阻测量
*入口参数：无
*出口参数：无
*说       明：
***********************************************************/
void Slope_Measure_SENS(void)
{
	slope_measure_flag = 1;//表明此次测量传感器电阻
	PORT_REF_HZ;          //参考电阻高阻态，防止干扰
	TAR_CLEAR;             //TA主时钟清零
	PORT_SENS_LOW;          //开启传感器电阻放电通道，开始计数测量放电时间
}

/***********************************************************
*名       称：Slope_TA_CCI1B()
*功       能：针对Slope应用的捕获中断事件处理函数，测得电阻值
*入口参数：无
*出口参数：无
*说       明：根据标识位，判断本次测量的是参考电阻还是传感器电阻
***********************************************************/
void Slope_TA_CCI1B(void)
{
	switch( slope_measure_flag)
	{
		case 0 : r_REF = TA0CCR1; break;
		case 1 : r_SENS= TA0CCR1; break;
		default: break;
	}
}
