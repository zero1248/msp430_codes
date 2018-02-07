/*
 * 18B20.c
 *
 *  Created on: 2015-4-29
 *      Author: Isaac
 */
#include<msp430.h>
#include<msp430g2553.h>
#include"18B20.h"

unsigned int DS18B20_temperature=0;

/*******************************************
函数名称：DS18B20_TADelay_us
功    能：实现N个微秒的延时
参    数：n--延时长度
返回值  ：无
说明    ：定时器A的计数时钟是1MHz，CPU主频8MHz
          所以通过定时器延时能够得到极为精确的
          us级延时
********************************************/
void DS18B20_TADelay_us(unsigned int n)
{
    CCR0 = n;
    TACTL |= MC_1;           //增计数到CCR0
    while(!(TACTL & BIT0));   //等待
    TACTL &= ~MC_1;          //停止计数
    TACTL &= ~BIT0;          //清除中断标志
}

/***********************************************************
*名        称：DS18B20_delay_us()
*功        能：us精确延时
*入口参数：无
*出口参数：无
*说        明：实际延时值与CPU时钟频率有关，所以用MCLK_FREQ来自动调整
***********************************************************/
/*static void DS18B20_delay_us()//默认取空函数
{
	//__delay_cycles(DS18B20_MCLK_FREQ / 1000);    //1000us
	//__delay_cycles(DS18B20_MCLK_FREQ / 10000);   //100us
	//__delay_cycles(DS18B20_MCLK_FREQ / 100000);  //10us
	__delay_cycles(DS18B20_MCLK_FREQ / 1000000); //1us
}*/

/***********************************************************
*名        称：DS18B20_Init()
*功        能：DS18B20初始化
*入口参数：无
*出口参数：无
*说        明：总线上的一个上升沿并保持高电平使18B20初始化
***********************************************************/
void DS18B20_Init(void)
{

	DS18B20_WRITE_SET;
	_NOP();
	DS18B20_WRITE_HIGH;
	DS18B20_TADelay_us(10);
	DS18B20_WRITE_LOW;
	DS18B20_TADelay_us(500);
	DS18B20_WRITE_HIGH;
	DS18B20_TADelay_us(55);
	DS18B20_WRITE_HIGH;
	DS18B20_TADelay_us(400);
}

/***********************************************************
*名        称：DS18B20_WriteChar()
*功        能：向18B20写一个字节数据
*入口参数：command-待写入字节
*出口参数：无
*说        明：低60~120高>1--写0  低1~15高>45--写1
***********************************************************/
void DS18B20_WriteChar(unsigned char command)
{
	unsigned char i = 0;
	unsigned char temp = 0; //中间变量
	_DINT();
	for( i=8 ; i>0 ; i-- )
	{
		DS18B20_WRITE_LOW;
		DS18B20_TADelay_us(6);
		temp = command & COMMAND_LSB; //取最低位
		if( temp )  DS18B20_WRITE_HIGH;
		else        DS18B20_WRITE_LOW;
		command >>= 1;
		DS18B20_TADelay_us(50);
		//DS18B20_TADelay_us(2);
		DS18B20_WRITE_HIGH;
		DS18B20_TADelay_us(10);
	}
	_EINT();
}

/***********************************************************
*名        称：DS18B20_ReadChar()
*功        能：18B20读取一字节数据
*入口参数：无
*出口参数：temp
*说        明：
***********************************************************/
unsigned char DS18B20_ReadChar(void)
{
	unsigned char i = 0;
	unsigned char temp = 0;
	_DINT();
	DS18B20_WRITE_SET;
	for( i=8 ; i>0 ; i--)
	{
        temp >>= 1;
        DS18B20_WRITE_LOW;
        DS18B20_TADelay_us(6);
        DS18B20_WRITE_HIGH;
        DS18B20_TADelay_us(8);
        DS18B20_READ_SET;
        _NOP();
        if( DS18B20_READ_DATA ) temp |= COMMAND_MSB;
        DS18B20_TADelay_us(45);
        DS18B20_WRITE_SET;
        DS18B20_WRITE_HIGH;
        DS18B20_TADelay_us(10);
	}
	_EINT();
	return ( temp );
}

/***********************************************************
*名        称：DS18B20_StartChange()
*功        能：18B20开始转换数据
*入口参数：无
*出口参数：无
*说        明：每次都要先初始化总线
***********************************************************/
void DS18B20_StartChange(void)
{
	DS18B20_Init();
	DS18B20_WriteChar(0xcc);// 跳过ROM，直接向18B20发温度变换命令，
	DS18B20_WriteChar(0x44);// 开始温度转换
}

/***********************************************************
*名        称：DS18B20_Read_SP()
*功        能：18B20读取ScratchPad
*入口参数：无
*出口参数：无
*说        明：每次都要先初始化总线
***********************************************************/
void DS18B20_Read_SP(void)
{
	DS18B20_Init();
	DS18B20_WriteChar(0xcc);// 跳过ROM，直接向18B20发温度变换命令，
	DS18B20_WriteChar(0xbe);// 读取ScratchPad
}

/***********************************************************
*名        称：DS18B20_ReadTemp()
*功        能：18B20完成温度转换
*入口参数：无
*出口参数：DS18B20_temperature-转换后得到的温度
*说        明：此时的温度实际上扩大了10倍，显示时要进行相应操作得到真实值
***********************************************************/
unsigned int DS18B20_ReadTemp(void)
{
	float float_temp = 1.0;
	unsigned char low_temperature  = 0;
	unsigned char high_temperature = 0;
	DS18B20_StartChange();
	DS18B20_TADelay_us(60000);
	DS18B20_Read_SP();
	low_temperature  = DS18B20_ReadChar();
	high_temperature = DS18B20_ReadChar();
	DS18B20_temperature = high_temperature;
	DS18B20_temperature <<= 8;
	DS18B20_temperature |= low_temperature;
	DS18B20_temperature &= 0x7ff;
	float_temp = DS18B20_temperature * 0.0625;
	float_temp = float_temp * 10 + 0.5;
	//temp*0.0625就是实际温度值，出厂时默认配置为12位，分辨率为0.0625，
	//DS18B20_temperature = (unsigned int)(float_temp * 10 + 0.5);
	return (float_temp );
}

/***********************************************************
*名        称：DS18B20_ReadTempData()
*功        能：18B20完成温度读取,但不计算 只得到初始数据
*入口参数：无
*出口参数：DS18B20_data-得到的温度数据
*说        明：
***********************************************************/
unsigned int DS18B20_ReadTempData(void)
{
	unsigned char low_temperature  = 0;
	unsigned char high_temperature = 0;
	unsigned int DS18B20_data;
	DS18B20_StartChange();
	DS18B20_TADelay_us(60000);
	DS18B20_Read_SP();
	low_temperature  = DS18B20_ReadChar();
	high_temperature = DS18B20_ReadChar();
	DS18B20_data = high_temperature;
	DS18B20_data <<= 8;
	DS18B20_data |= low_temperature;
	DS18B20_data &= 0x7ff;
	return ( DS18B20_data );
}



