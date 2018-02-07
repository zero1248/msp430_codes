/*
 * photoresistance.c
 *
 *  Created on: 2015-5-1
 *      Author: Isaac
 */

#include<msp430g2553.h>
#include"photoresistance.h"

/********************************************************
*名        称：LDR_Init()
*功        能：初始化光敏电阻对应IO口
*入口参数：无
*出口参数：无
*说        明：将对应IO设为输入
********************************************************/
void LDR_Init(void)
{
	LCD_PORT_INIT;
}

/********************************************************
*名        称：Get_LDR()
*功        能：读取光敏电阻受光状态
*入口参数：无
*出口参数：res_value-反映光敏电阻受光状态
*说        明：有光-高电平（0x80），无光-低电平（0）
********************************************************/
unsigned char Get_LDR(void)
{
	unsigned char res_value = 0;
	if(LCD_READ) res_value = 0x80;
	else res_value = 0;
	return (res_value);
}



