/*
 * OLED.c
 *  SoftI2C
 *  Created on: 2015-4-29
 *      Author: Isaac
 */
#include <msp430.h>
#include "OLED.h"
#include "I2C.h"
#include "codetab.h" //存放字库

#define X_WIDTH 	128
#define Y_WIDTH 	64
#define	Brightness	0xcf

/********************************************************
*名       称：OLED_I2C_Init()
*功       能：OLED初始化
*入口参数：无
*出口参数：无
*说       明：直接调用I2C初始化
********************************************************/
void OLED_I2C_Init(void)
{
	I2C_Init();
}

/***********************************************************
*名       称：OLED_delay_us()
*功       能：us精确延时
*入口参数：无
*出口参数：无
*说       明：实际延时值与CPU时钟频率有关，所以用MCLK_FREQ来自动调整
***********************************************************/
static void OLED_delay_us()//默认取空函数
{
	//__delay_cycles(OLED_MCLK_FREQ / 1000);    //1000us
	//__delay_cycles(OLED_MCLK_FREQ / 10000);   //100us
	//__delay_cycles(OLED_MCLK_FREQ / 100000);  //10us
	__delay_cycles(OLED_MCLK_FREQ / 1000000); //1us
}

/********************************************************
*名       称：OLED_Write_Data()
*功       能：向OLED写入数据
*入口参数：data-待发送数据
*出口参数：无
*说       明：有完整的起始位和终止位
********************************************************/
unsigned char OLED_Write_Data(unsigned char OLED_data)
{
	_DINT();
	I2C_Start();
	I2C_Send_Char( 0x78 );
	I2C_Send_Char( 0x40 );
	I2C_Send_Char( OLED_data );
	I2C_Stop();
	_EINT();
	return 1;
}

/********************************************************
*名       称：OLED_Write_Command()
*功       能：向OLED写入指令
*入口参数：Command-待发送指令
*出口参数：无
*说       明：有完整的起始位和终止位
********************************************************/
unsigned char OLED_Write_Command(unsigned char OLED_command)
{
	_DINT();
	I2C_Start();
	I2C_Send_Char( 0x78 );
	I2C_Send_Char( 0x00 );
	I2C_Send_Char( OLED_command );
	I2C_Stop();
	_EINT();
	return 1;
}

/********************************************************
*名       称：OLED_Set_Position()
*功       能：向OLED设置写入坐标
*入口参数：x-横坐标，y-纵坐标
*出口参数：无
*说       明：
********************************************************/
void OLED_Set_Position(unsigned char x, unsigned char y)
{
	OLED_Write_Command( 0xb0 + y );
	OLED_Write_Command( ( (x & 0xf0) >> 4) | 0x10 );
	OLED_Write_Command( (x & 0x0f) | 0x01 );
}

/********************************************************
*名       称：OLED_Fill()
*功       能：向OLED全屏显示
*入口参数：bmp_dat-要显示的内容
*出口参数：无
*说       明：重复 bap_dat
********************************************************/
void OLED_Fill(unsigned char bmp_dat)
{
	unsigned char y,x;
	for( y=0 ; y<8; y++ )
	{
		OLED_Write_Command( 0xb0+y );
		OLED_Write_Command( 0x01 );
		OLED_Write_Command( 0x10 );
		for( x=X_WIDTH ; x>0 ; x-- )
			OLED_Write_Data( bmp_dat );
	}
}

/********************************************************
*名       称：OLED_Reset()
*功       能：OLED复位
*入口参数：无
*出口参数：无
*说       明：所有位置写入0
********************************************************/
void OLED_Reset(void)
{
	unsigned char y=0 , x=0;
	for( y=0 ; y<8 ; y++ )
	{
		OLED_Write_Command( 0xb0 + y );
		OLED_Write_Command( 0x01 );
		OLED_Write_Command( 0x10 );
		for( x=0 ; x<X_WIDTH ; x++ )
			OLED_Write_Data(0);
	}
}

/********************************************************
*名       称：OLED_Init()
*功       能：OLED初始化
*入口参数：无
*出口参数：无
*说       明：其实并不知道是在干什么。。。直接改的例程
********************************************************/
void OLED_Init(void)
{
	OLED_delay_us();         //初始化之前的延时很重要！
	OLED_Write_Command(0xae);//--turn off oled panel
	OLED_Write_Command(0x00);//---set low column address
	OLED_Write_Command(0x10);//---set high column address
	OLED_Write_Command(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_Write_Command(0x81);//--set contrast control register
	OLED_Write_Command(Brightness); // Set SEG Output Current Brightness
	OLED_Write_Command(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_Write_Command(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_Write_Command(0xa6);//--set normal display
	OLED_Write_Command(0xa8);//--set multiplex ratio(1 to 64)
	OLED_Write_Command(0x3f);//--1/64 duty
	OLED_Write_Command(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_Write_Command(0x00);//-not offset
	OLED_Write_Command(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_Write_Command(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_Write_Command(0xd9);//--set pre-charge period
	OLED_Write_Command(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_Write_Command(0xda);//--set com pins hardware configuration
	OLED_Write_Command(0x12);
	OLED_Write_Command(0xdb);//--set vcomh
	OLED_Write_Command(0x40);//Set VCOM Deselect Level
	OLED_Write_Command(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_Write_Command(0x02);//
	OLED_Write_Command(0x8d);//--set Charge Pump enable/disable
	OLED_Write_Command(0x14);//--set(0x10) disable
	OLED_Write_Command(0xa4);// Disable Entire Display On (0xa4/0xa5)
	OLED_Write_Command(0xa6);// Disable Inverse Display On (0xa6/a7)
	OLED_Write_Command(0xaf);//--turn on oled panel
	OLED_Fill(0x00); //初始清屏
	OLED_Set_Position(0,0);
}

/********************************************************
*名       称：OLED_Display_6x8()
*功       能：OLED显示6*8一组标准ASCII字符串
*入口参数：x，y-地址
		  ch[]-要显示的字符串
*出口参数：无
*说       明：F6x8中ASCII码从0x21开始，因此ch[j]要减去32
********************************************************/
void OLED_Display_6x8(unsigned char x, unsigned char y,char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>122)
		{
			x=0; y++;
		}
		OLED_Set_Position( x , y );
		for( i=0 ; i<6 ; i++ )
			OLED_Write_Data(F6x8[c][i]);  //显示字库内对应的字符
		x += 6;
		j++;
	}
}

/********************************************************
*名       称：OLED_Display_8x16()
*功       能：OLED显示8*16一组标准ASCII字符串
*入口参数：x，y-地址
		  ch[]-要显示的字符串
*出口参数：无
*说       明：F8x16中ASCII码从0x21开始，因此ch[j]要减去32
********************************************************/
void OLED_Display_8x16(unsigned char x,unsigned char y,char ch[])
{
	unsigned char c=0 , i=0 , j=0;
	while ( ch[j]!='\0' )
	{
		c =ch[j]-32;
		if( x > 120 )
		{
			x = 0;
			y++;
		}
		OLED_Set_Position(x,y);
		for( i=0 ; i<8 ; i++ )
			OLED_Write_Data( F8X16[ c*16 + i ] );
		OLED_Set_Position(x,y+1);
		for( i=0 ; i<8 ; i++ )
			OLED_Write_Data( F8X16[ c*16 + i + 8 ] );
		x+=8;
		j++;
	}
}
/********************************************************
*名       称：OLED_Display_16x16()
*功       能：OLED显示16*16一组汉字点阵
*入口参数：x，y-地址
		  num-要显示的点阵数组中对应的汉字序号
*出口参数：无
*说       明：F16x16是1阶数组，且每32个表示一个汉字，
		    因此每个地址应该乘以32
********************************************************/
void OLED_Display_16x16(unsigned char x ,unsigned char y ,char num )
{
	unsigned char i = 0;
	unsigned int address = num<<5;
	OLED_Set_Position(x , y);
	for( i=0 ; i<16 ; i++ )
	{
		OLED_Write_Data( F16x16[address] );
		address += 1;
	}
	OLED_Set_Position( x , y+1 );
	for( i=0 ; i<16 ; i++ )
	{
		OLED_Write_Data( F16x16[address] );
		address += 1;
	}
}
/********************************************************
*名       称：OLED_Draw_BMP()
*功       能：OLED显示点阵位图
*入口参数：x0，y0，x1，y1-起止地址
		  BMP[]-要显示的点阵位图数组
*出口参数：无
*说       明：
********************************************************/
void OLED_Draw_BMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1,const unsigned char BMP[])
{
	unsigned int j = 0;
	unsigned char x = 0 , y = 0;

  if( y1%8 == 0)  y = y1/8;
  else y = y1/8 + 1;
	for( y=y0 ; y<y1 ; y++ )
	{
		OLED_Set_Position( x0 , y );
		for( x=x0 ; x<x1 ; x++ )
	    {
			OLED_Write_Data( BMP[j++] );
	    }
	}
}










