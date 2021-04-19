/*
 * Filter_Control.c
 *
 *  Created on: 2015-6-10
 *      Author: Isaac
 */
#include<msp430g2553.h>
#include"Filter_Control.h"
#include"TimerA_PWM.h"


/********************************************************
*名       称：Filter_IO_Init()
*功       能：初始化程控滤波器所需IO口
*入口参数：无
*出口参数：无
*说       明：P1.0~1.1---D0~D1    P1.2~1.5-----A0~A3
          P2.0---------PWM(MAX261clock)
          P2.1---------MAX261_WR低电平有效
          P2.2---------74HC573(LE)高电平有效
********************************************************/
void Filter_IO_Init(void)
{
	P1DIR = 0xff;
	P1OUT = 0xff;
	P2DIR = 0xff;
	P2OUT = 0xff;
	LOAD_74HC573;
}

/********************************************************
*名       称：FilterA_Mode_Set()
*功       能：设置程控滤波器A的工作模式
*入口参数：mode-取值为1/2/3/4/5
*出口参数：无
*说       明：Mode1(00)  低通、带通、带阻(一般用作带通、低通)
		  Mode2(01)  低通、带通、带阻()
          Mode3(10)  低通、带通、高通(椭圆带通、低通、高通)
          Mode3A(10) 低通、带通、高通、带阻(与Mode3配置相同，但是使用片上的OP—AMP)
          Mode4(11)  低通、高通、全通(全通)
********************************************************/
void FilterA_Mode_Set(unsigned char mode)
{
	switch(mode)
	{
		case 1 : P1OUT = FILTER_MODE1; break;
		case 2 : P1OUT = FILTER_MODE2; break;
		case 3 : P1OUT = FILTER_MODE3; break;
		case 4 : P1OUT = FILTER_MODE4; break;
		case 5 : P1OUT = FILTER_MODE3A;break;
		default: break;
	}
	UNLOAD_FILTER;
	__delay_cycles(10);
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
}

/********************************************************
*名       称：FilterA_F0_Set()
*功       能：设置程控滤波器A的中心频率
*入口参数：f_data-中心频率对应的六位数据
*出口参数：无
*说       明：低两位表示数据，BIT2~BIT5表示地址
********************************************************/
void FilterA_F0_Set(unsigned int f_data)
{
	unsigned char frequency = 0;
	unsigned char f_temp = 0;
	frequency = f_data;
	//----载入f0、f1----
	f_temp = (frequency & 0x03) + FILTER_F01;
	P1OUT = f_temp;
	UNLOAD_FILTER;
	__delay_cycles(10);
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
	//----载入f2、f3----
	f_temp = frequency>>2;
	f_temp = (f_temp & 0x03) + FILTER_F23;
	P1OUT = f_temp;
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
	//----载入f4、f5----
	f_temp = frequency>>4;
	f_temp = (f_temp & 0x03) + FILTER_F45;
	P1OUT = f_temp;
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
}

/********************************************************
*名       称：FilterA_Qvalue_Set()
*功       能：设置程控滤波器A的Q值
*入口参数：Q_value-Q值对应的七位数据
*出口参数：无
*说       明：低两位表示数据，BIT2~BIT5表示地址
********************************************************/
void FilterA_Qvalue_Set(unsigned char Q_value)
{
	unsigned char Q_data = 0;
	unsigned char Q_temp = 0;
	Q_data = Q_value;
	//----载入Q0、Q1----
	Q_temp = (Q_data & 0x03) + FILTER_Q01;
	P1OUT = Q_temp;
	UNLOAD_FILTER;
	__delay_cycles(10);
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
	//----载入Q2、Q3----
	Q_temp = Q_data>>2;
	Q_temp = (Q_temp & 0x03) + FILTER_Q23;
	P1OUT = Q_temp;
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
	//----载入Q4、Q5----
	Q_temp = Q_data>>4;
	Q_temp = (Q_temp & 0x03) + FILTER_Q45;
	P1OUT = Q_temp;
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
	//----载入Q6----
	Q_temp = Q_data>>6;
	Q_temp = (Q_temp & 0x01) + FILTER_Q6;
	P1OUT = Q_temp;
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
}

/********************************************************
*名       称：FilterB_Mode_Set()
*功       能：设置程控滤波器B的工作模式
*入口参数：mode-取值为1/2/3/4/5
*出口参数：无
*说       明：Mode1(00)  低通、带通、带阻(一般用作带通、低通)
		  Mode2(01)  低通、带通、带阻()
          Mode3(10)  低通、带通、高通(椭圆带通、低通、高通)
          Mode3A(10) 低通、带通、高通、带阻(与Mode3配置相同，但是使用片上的OP—AMP)
          Mode4(11)  低通、高通、全通(全通)
                  地址位比滤波器A对应多0x20
********************************************************/
void FilterB_Mode_Set(unsigned char mode)
{
	switch(mode)
	{
		case 1 : P1OUT = FILTER_MODE1 + BIT5; break;
		case 2 : P1OUT = FILTER_MODE2 + BIT5; break;
		case 3 : P1OUT = FILTER_MODE3 + BIT5; break;
		case 4 : P1OUT = FILTER_MODE4 + BIT5; break;
		case 5 : P1OUT = FILTER_MODE3A +BIT5;break;
		default: break;
	}
	UNLOAD_FILTER;
	__delay_cycles(10);
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
}

/********************************************************
*名       称：FilterB_F0_Set()
*功       能：设置程控滤波器B的中心频率
*入口参数：f_data-中心频率对应的六位数据
*出口参数：无
*说       明：低两位表示数据，BIT2~BIT5表示地址
		     地址位比滤波器A对应多0x20
********************************************************/
void FilterB_F0_Set(unsigned int f_data)
{
	unsigned char frequency = 0;
	unsigned char f_temp = 0;
	frequency = f_data;
	//----载入f0、f1----
	f_temp = (frequency & 0x03) + FILTER_F01;
	P1OUT = f_temp + BIT5;
	UNLOAD_FILTER;
	__delay_cycles(10);
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
	//----载入f2、f3----
	f_temp = frequency>>2;
	f_temp = (f_temp & 0x03) + FILTER_F23;
	P1OUT = f_temp + BIT5;
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
	//----载入f4、f5----
	f_temp = frequency>>4;
	f_temp = (f_temp & 0x03) + FILTER_F45;
	P1OUT = f_temp + BIT5;
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
}

/********************************************************
*名       称：FilterB_Qvalue_Set()
*功       能：设置程控滤波器B的Q值
*入口参数：Q_value-Q值对应的七位数据
*出口参数：无
*说       明：低两位表示数据，BIT2~BIT5表示地址
		    地址位比滤波器A对应多0x20
********************************************************/
void FilterB_Qvalue_Set(unsigned char Q_value)
{
	unsigned char Q_data = 0;
	unsigned char Q_temp = 0;
	Q_data = Q_value;
	//----载入Q0、Q1----
	Q_temp = (Q_data & 0x03) + FILTER_Q01;
	P1OUT = Q_temp + BIT5;
	UNLOAD_FILTER;
	__delay_cycles(10);
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
	//----载入Q2、Q3----
	Q_temp = Q_data>>2;
	Q_temp = (Q_temp & 0x03) + FILTER_Q23;
	P1OUT = Q_temp + BIT5;
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
	//----载入Q4、Q5----
	Q_temp = Q_data>>4;
	Q_temp = (Q_temp & 0x03) + FILTER_Q45;
	P1OUT = Q_temp + BIT5;
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
	//----载入Q6----
	Q_temp = Q_data>>6;
	Q_temp = (Q_temp & 0x01) + FILTER_Q6;
	P1OUT = Q_temp + BIT5;
	LOAD_FILTER;
	__delay_cycles(80);
	UNLOAD_FILTER;
	__delay_cycles(80);
}

/********************************************************
*名       称：FilterB_Clock_Set()
*功       能：设置程控滤波器的时钟
*入口参数：clock_data-时钟值
*出口参数：无
*说       明：使用P2.0的TA1.0输出PWM波，占空比50%
********************************************************/
void Filter_Clock_Set(void)
{
	TA1_PWM_Init( 'S' , 8 , 'F' ,0 );
	TA1_PWM_SetPeriod(6);
	TA1_PWM_SetPermill(1 , 500);
}





