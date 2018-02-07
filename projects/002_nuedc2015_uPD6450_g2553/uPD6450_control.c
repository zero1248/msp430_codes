/*
 * uPD6450_control.c
 *
 *  Created on: 2015-8-13
 *      Author: Isaac
 */
#include"uPD6450_control.h"
#include<msp430g2553.h>

/********************************************************
*名       称：uPD6450_Port_Init()
*功       能：uPD6450通信引脚初始化
*入口参数：无
*出口参数：无
*说       明：初始化单片机输入输出端口，并正确配置输出电平
********************************************************/
void uPD6450_Port_Init(void)
{
	BUSY_INIT;
	CLK_INIT;
	STB_INIT;
	DATA_INIT;
	CLK_1;
	STB_0;
}

/********************************************************
*名       称：uPD6450_Inquire_Busy()
*功       能：uPD6450查忙
*入口参数：无
*出口参数：无
*说       明：等待BUSY引脚拉低
********************************************************/
static void uPD6450_Inquire_Busy(void)
{
	__delay_cycles(800);
}

/********************************************************
*名       称：uPD6450_Send_Byte()
*功       能：uPD6450发送一个字节数据
*入口参数：data--一个Byte数据
*出口参数：无
*说       明：先发送最高位，CLK上升沿锁入数据，
		    八位数据发送完STB送入写入脉冲，然后等待查忙
********************************************************/
void uPD6450_Send_Byte(unsigned char data)
{
    unsigned char i = 0;
    for(i=8 ; i>0 ; i--)
    {
    	if(data & BIT7 )
    		{DATA__1;}
    	else
    		{DATA__0;}
    	data <<= 1;
    }
    STB__1;
    uPD6450_Inquire_Busy();
}

/********************************************************
*名       称：uPD6450_Format_Release()
*功       能：每次上电后uPD6450发送复位数据
*入口参数：data——一个Byte数据
*出口参数：无
*说       明：Fr位（BIT0）从0变1，可以清除测试模式（测试模式会阻止接收正常数据）
********************************************************/
void uPD6450_Format_Release(void)
{
	uPD6450_Send_Byte(0xff);
}


/********************************************************
*名       称：uPD6450_Select_Banks()
*功       能：选择指令库
*入口参数：f0
*出口参数：无
*说       明：F0位（BIT1）控制选择指令库，高6位置1
********************************************************/
static void uPD6450_Select_Banks(unsigned char f0)
{
	unsigned char f0_data = 0;
	 f0_data = f0;
	if(f0_data)
		uPD6450_Send_Byte(0xff);
	else
		uPD6450_Send_Byte(0xfd);
}

/********************************************************
*名       称：uPD6450_Display_Character()
*功       能：写入待显示数据
*入口参数：cha
*出口参数：无
*说       明：先选择0指令库，再写入字符数据，B-12，C-13
********************************************************/
void uPD6450_Display_Character(unsigned char cha)
{
	unsigned char cha_data = 0;
	cha_data = cha;
	uPD6450_Select_Banks(BANK_0);
	uPD6450_Send_Byte(cha_data);
}

/********************************************************
*名       称：uPD6450_Display_Address()
*功       能：写入数据显示位置
*入口参数：line-行位置
		  column-列位置
*出口参数：无
*说       明：先选择0指令库，再写入位置数据。
		  line范围0——11，column范围0-23
********************************************************/
void uPD6450_Display_Address(unsigned char line,unsigned char column)
{
	unsigned char line_data = 0,column_data = 0;
	line_data = line + 0x90;
	column_data = column + 0xa0;
	uPD6450_Select_Banks(BANK_0);
	uPD6450_Send_Byte(line_data);
	uPD6450_Select_Banks(BANK_0);
	uPD6450_Send_Byte(column_data);
}

/********************************************************
*名       称：uPD6450_Blink_Set()
*功       能：写入数据闪烁设置
*入口参数：blink-闪烁参数
*出口参数：无
*说       明：先选择指令库0.然后选择是否闪烁，0-不闪烁，1-闪烁
********************************************************/
void uPD6450_Blink_Set(unsigned char blink)
{
	unsigned char blink_data = 0;
	blink_data = blink;
	if(blink_data)
	{
		uPD6450_Select_Banks(BANK_0);
		uPD6450_Send_Byte(BLINK_1);
	}
	else
	{
		uPD6450_Select_Banks(BANK_0);
		uPD6450_Send_Byte(BLINK_0);
	}
}

/********************************************************
*名       称：uPD6450_Display_Position()
*功       能：写入数据像素位置
*入口参数：vertical-水平像素位置
		  horizontal-垂直像素位置
*出口参数：无
*说       明：先选择指令库1，然后写数据，都是低五位
********************************************************/
void uPD6450_Display_Position(unsigned char vertical,unsigned char horizontal)
{
	unsigned char ver_data = 0,hor_data = 0;
	ver_data = vertical + 0x40;
	hor_data = horizontal + 0xc0;
	uPD6450_Select_Banks(BANK_1);
	uPD6450_Send_Byte(ver_data);
	uPD6450_Select_Banks(BANK_1);
	uPD6450_Send_Byte(hor_data);
}

/********************************************************
*名       称：uPD6450_NP_EI_Osc()
*功       能：选择NTSC/PAL、External/Internal、Xosc
*入口参数：无
*出口参数：无
*说       明：PAL/External/晶振起振
********************************************************/
void uPD6450_NP_EI_Osc(void)
{
	uPD6450_Select_Banks(BANK_0);
	uPD6450_Send_Byte(0xf5);
}

/********************************************************
*名       称：uPD6450_ON_BL_LCosc()
*功       能：选择ON/OFF、Blink、LCosc
*入口参数：无
*出口参数：无
*说       明：display显示/Blink关/LC起振
********************************************************/
void uPD6450_ON_BL_LCosc(void)
{
	uPD6450_Select_Banks(BANK_0);
	uPD6450_Send_Byte(0xe9);
}



