/*
 * PLL_control.c
 *
 *  Created on: 2015-7-8
 *      Author: Isaac
 */

#include<msp430g2553.h>
#include"PLL_control.h"

/********************************************************
*名        称：ConReg_Emit()
*功        能：控制寄存器写入
*入口参数 ：无
*出口参数 ：无
*说        明：ENB为高 MSB为1,默认 Test Bit=0; Aux Data Select=1;
	 	   REFout=0; TxPD Enable=0;RxPD Enable=1; RefPD Enable=1
********************************************************/
void ConReg_Emit(void)
{
	ENB_HIGH;
	MSB_1;
	CLK_ACTION;
	Din_LOW;    //此位置0
	CLK_ACTION;
	TEST_BIT;   //非测试模式
	CLK_ACTION;
	AUX_DATA_SELECT; //ADin和Din分别输入16位发射数据和16位接收数据
	CLK_ACTION;
	REF_OUT;
	CLK_ACTION;
	TxPD_ENABLE;//关闭发射寄存器、发射鉴相器及相关电路关机
	CLK_ACTION;
	RxPD_ENABLE;
	CLK_ACTION;
	REFPD_ENABLE;
	CLK_ACTION;
	ENB_LOW;
}

/********************************************************
*名        称：RefFre_Emit()
*功        能：参考频率编程写入
*入口参数 ：无
*出口参数 ：无
*说        明：ENB为高 MSB为0，共32位。AUX REF ENABLE=0,Tx-0 SELECT,
		   Rx-0 SELECT,12-BITS FREQ DATA,fR1S1,fR1S2,
		   14-BITS AUX REF FREQ DATA
********************************************************/
void RefFre_Emit(unsigned int fre)
{
	unsigned char i = 0;
	ENB_HIGH;
	MSB_0;
	CLK_ACTION;
	AUX_REF_ENABLE;
	CLK_ACTION;
	Tx_0_SELECT;
	CLK_ACTION;
	Rx_0_SELECT;
	CLK_ACTION;
	for(i=0; i<12; i++ )
	{
		if(fre&BIT0)
		{
			Din_HIGH;
			CLK_ACTION;
		}
		else
		{
			Din_LOW;
			CLK_ACTION;
		}
		fre >>= 1;
	}
	F_R1_S1;
	CLK_ACTION;
	F_R1_S2;
	CLK_ACTION;
	for(i=0; i<14; i++ )
	{
		Din_LOW;
		CLK_ACTION;
	}
	ENB_LOW;
}

void Tx_Counter_Emit(unsigned int Rx_data)
{
	unsigned char i = 0;
	ENB_LOW;
	for(i=0; i<16; i++ )
	{
		if(Rx_data & BIT0)
		{
			Din_HIGH;
			CLK_ACTION;
		}
		else
		{
			Din_LOW;
			CLK_ACTION;
		}
		Rx_data >>= 1;
	}
	ENB_HIGH;
	ENB_LOW;
}















