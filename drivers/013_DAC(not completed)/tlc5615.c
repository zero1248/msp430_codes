/*
 * DAC10.c
 *
 *  Created on: 2015-5-14
 *      Author: Isaac
 */
#include<msp430.h>
#include"DAC10.h"
/***********************************************************
*名       称：DAC10_Init()
*功       能：DAC10初始化
*入口参数：无
*出口参数：无
*说       明：
***********************************************************/
void DAC10_Init(void)
{
	//-------设置IO为输出-------------
	DAC_IO_INIT;
	DAC_SCLK_INIT;
	//-------设置IO初始状态为高-------
	SCLK_HIGH;
	DIN_HIGH;
}

/***********************************************************
*名       称：DAC10_WriteData()
*功       能：DAC10写入数据
*入口参数：data-int型，BITF = MSB
*出口参数：无
*说       明：CS的上升和下降都必须发生在 SCLK 为低电平期间
		  SCLK高电平期间写入数据
***********************************************************/
void DAC10_WriteData(unsigned int data)
{
	unsigned int temp = 0;
	unsigned char i = 0;
	temp = data;
	SCLK_LOW;
	CS_LOW;
	//-------------10位数据位-----------
	for( i=10 ; i>0 ; i-- )
	{
		SCLK_HIGH;
		if( temp & BITF ) DIN_HIGH;
		else               DIN_LOW;
		SCLK_LOW;
		temp <<= 1;
	}
	//----------两位填充位-----------
	SCLK_HIGH;
	DIN_LOW;
	SCLK_LOW;
	SCLK_HIGH;
	DIN_LOW;
	SCLK_LOW;
	//----------拉高片选----------
	CS_HIGH;
}


