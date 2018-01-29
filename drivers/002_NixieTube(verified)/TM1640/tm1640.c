/*
 * tm1640.c
 *
 *  Created on: 2015-11-7
 *      Author: Isaac
 */
#include "tm1640.h"
#include "msp430x14x.h"
const unsigned char Segment_Tab[] = {
  a_ + b_ + c_ + d_ + e_ + f_,             // Displays "0"
  b_ + c_,                                 // Displays "1"
  a_ + b_ + d_ + e_ + g_,                  // Displays "2"
  a_ + b_ + c_ + d_ + g_,                  // Displays "3"
  b_ + c_ + f_ + g_,                       // Displays "4"
  a_ + c_ + d_ + f_ +g_,                   // Displays "5"
  a_ + c_ + d_ + e_ + f_ + g_,             // Displays "6"
  a_ + b_ + c_,                            // Displays "7"
  a_ + b_ + c_ + d_ + e_ + f_ + g_,        // Displays "8"
  a_ + b_ + c_ + d_ + f_ + g_,             // Displays "9"
  a_ + b_ + c_ + e_ + f_ + g_,             // Displays "A"
  c_ + d_ + e_ + f_ + g_,                  // Displays "B"
  a_ + d_ + e_ + f_,	                   // Displays "C"
  b_ + c_ + d_ + e_ + g_,                  // Displays "D"
  a_ + d_ + e_ + f_ + g_,            	   // Displays "E"
  a_ + e_ + f_ + g_,                       // Displays "F"
  g_,                                      // Displays "-"
  b_,                                      // Displays "'"
  0                                        // Displays " "
};
#undef a_             //释放预定义
#undef b_
#undef c_
#undef d_
#undef e_
#undef f_
#undef g_

static unsigned char Number_Value[]={0,0,0,0,0,0,0,0};//用来存储每位数码管应显示的数值

/********************************************************
*名        称：Start_Tm1640()
*功        能：TM1640芯片开始输入数据
*入口参数 ：无
*出口参数 ：无
*说        明：CLK为高电平时，DIN由高变低
********************************************************/
void Start_Tm1640(void){
	TM1640_SCLK_HIGH;
	TM1640_DIN_HIGH;
	TM1640_DIN_LOW;
	__delay_cycles(8);
	TM1640_SCLK_LOW;
}

/********************************************************
*名        称：Stop_Tm1640()
*功        能：TM1640芯片停止输入数据
*入口参数 ：无
*出口参数 ：无
*说        明：CLK为高电平时，DIN由低变高
********************************************************/
void Stop_Tm1640(void){
	TM1640_SCLK_LOW;
	TM1640_DIN_LOW;
	TM1640_SCLK_HIGH;
	__delay_cycles(8);
	TM1640_DIN_HIGH;
}

/********************************************************
*名        称：Tm1640_Write_Bit()
*功        能：TM1640芯片写高电平或低电平
*入口参数 ：无
*出口参数 ：无
*说        明：CLK为低电平时，DIN上的信号才能改变；
		   CLK为高电平时，DIN上的信号必须不变
********************************************************/
static void Tm1640_Write_Bit(unsigned char tm1640_bit){
	if(tm1640_bit){
		TM1640_DIN_HIGH;
		TM1640_SCLK_HIGH;
		__delay_cycles(8);
		TM1640_SCLK_LOW;
	}
	else{
		TM1640_DIN_LOW;
		TM1640_SCLK_HIGH;
		__delay_cycles(8);
		TM1640_SCLK_LOW;
	}
}

/********************************************************
*名        称：Tm1640_Input_Command()
*功        能：TM1640芯片输入字节
*入口参数 ：无
*出口参数 ：无
*说        明：调用Tm1640_Write_Bit()写入每位电平，可以使指令
		      也可以是数据
********************************************************/
static void Tm1640_Input_Command(unsigned char tm1640_command){
	unsigned char i = 0;
	for( i=0 ; i<8 ; i++ ){
		if(tm1640_command & BIT0)
			Tm1640_Write_Bit(1);
		else
			Tm1640_Write_Bit(0);
		tm1640_command >>= 1;
	}
}

/********************************************************
*名        称：Get_Length()
*功        能：获取输入int型数据的长度（包括符号）
*入口参数 ：data_int——int型数据
*出口参数 ：length——数据的长度
*说        明：该数据长度不会超过正负30000
********************************************************/
static unsigned char Get_Length(int data_int){
	unsigned char length = 0;//记录符号与数据的总位数
	if(data_int < 0){
		length++;
		data_int *= -1;
	}
	if(data_int >= 10000)length += 5;
	else if(data_int >= 1000 && data_int<10000)
		length += 4;
	else if(data_int >= 100 && data_int<1000)
		length += 3;
	else if(data_int >= 10 && data_int<100)
		length += 2;
	else if(data_int >= 1 && data_int<10)
		length += 1;
	return length;
}

/********************************************************
*名        称：Get_Num_Value()
*功        能：获取输入int型的每一位十进制位并存入数组Number_Value[]
*入口参数 ：data_int——int型数据
*出口参数 ：无
*说        明：需要判断正负号，并且在前面补' '
********************************************************/
static void Get_Num_Value( int data_int ){
	unsigned char length = 0;//记录符号与数据的总位数
	unsigned char i = 0 , j = 0;
	unsigned char minus_sign = 0;//负数标记，1表示负数
	unsigned char bit_dec = 0;   //十进制位
	if( data_int < 0 )minus_sign = 1;//判断正负
	length = Get_Length( data_int );//获取数据长度
	for( i=0 ; i<(8-length) ; i++ ){//填充空白位
		 Number_Value[i] = 18; //18是Segment_Tab[]中' '的脚标
	}
	if( minus_sign ){
		data_int *= -1;
		Number_Value[i++] = 18;
		for( j=8 ; j>=i ; j-- ){
			bit_dec = data_int % 10;
			data_int /= 10;
			Number_Value[j] = bit_dec;
		}
	}
	else{
		for( j=8 ; j>=i ; j-- ){
			bit_dec = data_int % 10;
			data_int /= 10;
			Number_Value[j] = bit_dec;
		}
	}
}

/********************************************************
*名        称：Tm1640_Input()
*功        能：向Tm1640内部寄存器写入信息
*入口参数 ：mode——写入模式，加一模式，或固定地址模式
		   address——地址
		   data——数据
*出口参数 ：无
*说        明：所需模式指令和地址指令在头文件中均有定义
********************************************************/
void Tm1640_Input(unsigned char mode , unsigned char address , float data){
	int data_int = 0;
	unsigned char i =0;
	unsigned char data_temp = 0; //临时显示数据
	data_int = (int)(data * 100);
	Get_Num_Value( data_int );
	Tm1640_Input_Command(mode);
	Tm1640_Input_Command(address);
	for( i=0 ; i<8 ; i++ ){
		data_temp = Number_Value[i];
		Tm1640_Input_Command(Segment_Tab[data_temp]);
	}
	Tm1640_Input_Command(TM1640_BRIGHTNESS_3);
}
