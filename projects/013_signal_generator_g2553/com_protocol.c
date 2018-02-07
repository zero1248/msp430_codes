/*
 * com_protocol.c
 *
 *  Created on: 2015-4-30
 *      Author: Isaac
 */
#include<msp430g2553.h>
#include"com_protocol.h"
#include"UART.h"
#include"OLED.h"
#include"codetab.h"
unsigned int frequency_temp = 0;
unsigned char Tx_Data_Packet[6] = {'0','1','2','3','4','5'};
unsigned char Rx_Data_Packet[5] = {'0','1','2','3','4'};
char Frequency[6] = {0,0,0,0,0,0};
char PPvalue[5] = {0,0,0,0,0};

/********************************************************
*名        称：Display_FixedData()
*功        能：显示固定汉字图片
*入口参数 ：无
*出口参数 ：无
*说        明：
********************************************************/
void Display_FixedData(void)
{
	OLED_Reset();//清屏
	OLED_Display_16x16(0,0,1);      /*"信"*/
	OLED_Display_16x16(16,0,2);     /*"号"*/
	OLED_Display_16x16(32,0,3);     /*"发"*/
	OLED_Display_16x16(48,0,4);		/*"生"*/
	OLED_Display_16x16(64,0,5);		/*"器"*/
	OLED_Display_16x16(112,0,20);    /*电量*/

	OLED_Display_16x16(2,2,21);     /*无波形*/
	OLED_Display_8x16(24,2,"00000");   /*"Hz"*/
	OLED_Display_8x16(64,2,"Hz");   /*"Hz"*/
	OLED_Display_8x16(88,2,"0000");   /*"Vpp"*/
	OLED_Display_8x16(120,2,"V");   /*"Vpp"*/

	OLED_Display_16x16(0,4,16);    /*电*/
	OLED_Display_16x16(16,4,17);    /*流*/
	OLED_Display_8x16(56,4,"mA");
	OLED_Display_16x16(0,6,18);    /*功*/
	OLED_Display_16x16(16,6,19);    /*耗*/
	OLED_Display_8x16(56,6,"W");

	//OLED_Display_8x16 (48,6,"XDU Lab423");
}

static void Process(unsigned int value,char *Frequency){     //处理函数
	if(value >= 10000){
    	Frequency[0] = (unsigned char)( value / 10000) + '0';
    	Frequency[1] = (unsigned char)( (value % 10000) / 1000) + '0';
  		Frequency[2] = (unsigned char)( (value % 1000) / 100) + '0';
  		Frequency[3] = '0';
		Frequency[4] = '0';
		Frequency[5] = '\0';
    }
    else if( (value < 10000) && ( value >= 1000) ){
    	Frequency[0] = '0';
    	Frequency[1] = (unsigned char)( value / 1000 ) + '0';
    	Frequency[2] = (unsigned char)( (value % 1000) / 100 ) + '0';
  		Frequency[3] = '0';
		Frequency[4] = '0';
		Frequency[5] = '\0';
    }
	else if( (value < 1000) && (value >= 100) ){
		Frequency[0] = '0';
		Frequency[1] = '0';
    	Frequency[2] = (unsigned char)( value / 100 ) + '0';
    	Frequency[3] = '0';
    	Frequency[4] = '0';
    	Frequency[5] = '\0';
    }
}

/********************************************************
*名       称：Display_Data()
*功       能：主机显示所有数据
*入口参数：无
*出口参数：无
*说       明：调用各个模块
********************************************************/
void Display_Data(void)
{

	unsigned int PPvalue_temp = 0;
	unsigned char waveform_temp = 0;
	frequency_temp = Rx_Data_Packet[1];
	frequency_temp <<= 8;
	frequency_temp |= Rx_Data_Packet[2];

	waveform_temp = Rx_Data_Packet[3]+21;

	PPvalue_temp = Rx_Data_Packet[4];
	Process(frequency_temp , Frequency);
	PPvalue[0] = (unsigned char)(PPvalue_temp / 100) + '0';
	PPvalue[1] = (unsigned char)((PPvalue_temp % 100)/10) + '0';
	PPvalue[2] = '.';
	PPvalue[3] = (unsigned char)(PPvalue_temp % 10) + '0';
	PPvalue[4] = '\0';
	OLED_Display_16x16(2,2,waveform_temp);/*波形*/
	OLED_Display_8x16(24,2,Frequency);   /*频率*/
	OLED_Display_8x16(88,2,PPvalue);   /*峰峰值*/
	__delay_cycles(8000);
}


/********************************************************
*名        称：Extract_Data()
*功        能：提取接收到的数据帧
*入口参数：无
*出口参数：1-成功，0-失败
*说        明：
********************************************************/
char Extract_Data(void)
{
	unsigned char temp = 0;
	temp = Rx_FIFO_ReadChar();
	if( temp == MATCHING_CODE )
	{
			Rx_Data_Packet[0] = temp;
			Rx_Data_Packet[1] = Rx_FIFO_ReadChar(); //频率
			Rx_Data_Packet[2] = Rx_FIFO_ReadChar(); //频率
			Rx_Data_Packet[3] = Rx_FIFO_ReadChar(); //波形
			Rx_Data_Packet[4] = Rx_FIFO_ReadChar(); //幅值
			return 1;
	}
	else
	{
		return 0;
	}
}

/********************************************************
*名        称：Analysis_Data()
*功        能：解析接收到的数据帧，判断数据来源及去向
*入口参数：无
*出口参数：1-成功，0-失败
*说        明：
********************************************************/
void Analysis_Data(void)
{
	unsigned int frequency_temp = 0;

	frequency_temp = Rx_Data_Packet[1];
	frequency_temp <<= 8;
	frequency_temp |= Rx_Data_Packet[2];
	//if(frequency_temp<15000)
	//{
		Display_Data();
		__delay_cycles(8000);
	//}
}




