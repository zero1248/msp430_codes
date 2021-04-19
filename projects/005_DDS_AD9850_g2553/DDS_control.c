/*
 * DDS_control.c
 *
 *  Created on: 2015-6-5
 *      Author: Isaac
 */
#include"DDS_control.h"
#include"msp430g2553.h"

#define CRYSTAL_PHY 125 //晶振频率 单位MHz
/********************************************************
*名       称：AD9850_Reset()
*功       能：AD9850复位
*入口参数：无
*出口参数：无
*说       明：每次执行此函数后AD9850内部寄存器复位
********************************************************/
void AD9850_Reset_Parrel(void)
{
	AD9850_W_CLK_LOW;
	AD9850_FQ_UD_LOW;
//-----reset信号-----
	AD9850_RESET_LOW;
	AD9850_RESET_HIGH;
	AD9850_RESET_LOW;
}

/********************************************************
*名       称：AD9850_EnWrite()
*功       能：AD9850并行模式使能写入一个字节
*入口参数：无
*出口参数：无
*说       明：每次执行此函数后才会将数据口的数据写入AD9850内部寄存器
********************************************************/
static void AD9850_EnWrite(void)
{
	AD9850_W_CLK_HIGH;
	AD9850_W_CLK_LOW;
}

/********************************************************
*名       称：AD9850_Write_Parrel()
*功       能：AD9850并行模式发送数据
*入口参数：w0-高五位表示相位，bit2控制电源正常为0，bit0~1为工厂测试部分，设为00即可
		  frequence-按字节数从高到低顺序发送频率数据
*出口参数：无
*说       明：w0通常写0x00，frequence单位是Hz
********************************************************/
void AD9850_Write_Parrel(unsigned char w0,double frequence)
{
	unsigned char data_temp = 0;
	long int fre_data = 0;
	double fre_temp = 0;
	//----计算频率的HEX值----
	fre_temp = 4294.967295 / CRYSTAL_PHY;//适合100M晶振,如果时钟频率不为125MHZ，修改该处的频率值，单位MHz ！！！
	//frequence = frequence / 1000000;
	frequence = frequence * fre_temp;
	fre_data = frequence;
	//-----写w0数据-----
	data_temp = w0;
	AD9850_DATA = data_temp;
	AD9850_EnWrite();
	//-----写w1数据-----
	data_temp = (unsigned char)(fre_data >> 24);
	AD9850_DATA = data_temp;
	AD9850_EnWrite();
	//-----写w2数据-----
	data_temp = (unsigned char)(fre_data >> 16);
	AD9850_DATA = data_temp;
	AD9850_EnWrite();
	//-----写w3数据-----
	data_temp = (unsigned char)(fre_data >> 8);
	AD9850_DATA = data_temp;
	AD9850_EnWrite();
	//-----写w4数据-----
	data_temp = (unsigned char)(fre_data >> 0);
	AD9850_DATA = data_temp;
	AD9850_EnWrite();
	//----移入使能----
	AD9850_FQ_UD_HIGH;
	AD9850_FQ_UD_LOW;
}
