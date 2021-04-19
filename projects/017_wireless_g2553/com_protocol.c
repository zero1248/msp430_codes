/*
 * com_protocol.c
 *
 *  Created on: 2015-4-30
 *      Author: Isaac
 */
#include<msp430g2553.h>
#include"com_protocol.h"
#include"UART.h"
#include"18B20.h"
#include"photoresistance.h"
#include"OLED.h"
#include"codetab.h"
#include"electric_monitor.h"

unsigned char Tx_Data_Packet[6] = {'0','1','2','3','4','5'};
unsigned char Rx_Data_Packet[6] = {'0','1','2','3','4','5'};
char temperature_temp[5] = {0,0,0,0,0};

#ifdef MONITOR_TERMINAL

#define LOCAL_ADDRESS 0x40

/********************************************************
*名        称：Display_FixedData()
*功        能：显示固定汉字图片
*入口参数 ：无
*出口参数 ：五
*说        明：
********************************************************/
void Display_FixedData(void)
{
	OLED_Reset();//清屏
	OLED_Display_16x16(0,0,1);      /*"无"*/
	OLED_Display_16x16(16,0,2);     /*"线"*/
	OLED_Display_16x16(32,0,3);     /*"环"*/
	OLED_Display_16x16(48,0,4);		/*"境"*/
	OLED_Display_16x16(64,0,5);		/*"监"*/
	OLED_Display_16x16(80,0,6);		/*"测"*/
	OLED_Display_16x16(96,0,7);		/*"仪"*/
	OLED_Display_16x16(112,0,8);    /*电量*/
	OLED_Display_16x16(0,2,10);     /*"节"*/
	OLED_Display_16x16(16,2,11);    /*"点"*/
	OLED_Display_8x16 (32,2,"1 ");  /*"1 "*/
	OLED_Display_16x16(80,2,12);    /*"℃"*/
	OLED_Display_16x16(111,2,13);   /*有光*/
	OLED_Display_16x16(0,4,10);     /*"节"*/
	OLED_Display_16x16(16,4,11);    /*"点"*/
	OLED_Display_8x16 (32,4,"2 ");  /*"2 "*/
	OLED_Display_16x16(80,4,12);    /*"℃"*/
	OLED_Display_16x16(111,4,13);   /*有光*/
	OLED_Display_8x16 (48,6,"XDU Lab423");
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
	unsigned char temp_low =0;
	unsigned char temp_high =0;
	unsigned int temperature_data =0;
	unsigned char LDR_data = 0;
	unsigned char node = 0;
	float float_temp = 1.0;
	unsigned char electric_data = 0;
	//-------------获取节点信息-------------
	node = Rx_Data_Packet[1];
	//-------------获取光照信息-------------
	LDR_data = Rx_Data_Packet[3] & 0x80;
	//-------------获取温度信息-------------
	temp_high = Rx_Data_Packet[3] & 0x07;
	temp_low  = Rx_Data_Packet[4];
	//-------------获取电量信息-------------
	electric_data = Get_Electric();
	//-------------转换温度-------------
	temperature_data = temp_high;
	temperature_data = (temperature_data << 8) | temp_low;
	float_temp = temperature_data * 0.0625;
	float_temp = float_temp * 10 + 0.5;
	temperature_data = (unsigned int)float_temp;
	temperature_temp[0] = '0'+ ( temperature_data /100);
	temperature_temp[1] = '0'+ ( (temperature_data % 100)/10);
	temperature_temp[2] = '.';
	temperature_temp[3] = '0'+ ( temperature_data %10 );
	temperature_temp[4] = '\0';
	//----------------显示数据--------------------
	if( electric_data ) //电量足
		OLED_Display_16x16(112,0,8);
	else
		OLED_Display_16x16(112,0,9);
	switch( node )
	{
		case NODE_1:
		{
			OLED_Display_8x16 (48,2,temperature_temp);
			if( LDR_data )   //有光照
				OLED_Display_16x16 (111,2,13);
			else             //无光照
				OLED_Display_16x16 (111,2,14);
		}
		break;
		case NODE_2:
		{
			OLED_Display_8x16 (48,4,temperature_temp);
			if( LDR_data )   //有光照
				OLED_Display_16x16 (111,4,13);
			else             //无光照
				OLED_Display_16x16 (111,4,14);
		}
		break;
		default: break;
	}
}

/********************************************************
*名        称：Packet_Data()
*功        能：主机打包要发送的数据
*入口参数：source_address-源地址（即主机地址），target_address-目标地址
*出口参数：无
*说        明：
********************************************************/
void Packet_Data( unsigned char source_address,unsigned char target_address )
{
//	unsigned int temp_data = 0;
//	unsigned char temp_low = 0;
//	unsigned char temp_high = 0;
//	unsigned char LDR_data = 0;
//	LDR_data = Get_LDR();
//	temp_data = DS18B20_ReadTempData();
//	temp_low = (unsigned char)temp_data;
//	temp_high = (unsigned char)(temp_data >> 8);
//	temp_high |= LDR_data; //将光照信息与温度数据高字节合并
	Tx_Data_Packet[0] = MATCHING_CODE;   //匹配码
	Tx_Data_Packet[1] = source_address;   //数据源地址码，可以是本机地址，也可以是中继源地址
	Tx_Data_Packet[2] = target_address;  //目标地址码
	Tx_Data_Packet[3] = 0xaa;            //应答
	Tx_Data_Packet[4] = 0xaa;
	Tx_Data_Packet[5] = '\0';
}

/********************************************************
*名        称：Extract_Data()
*功        能：提取接收到的数据帧
*入口参数：无
*出口参数：1-成功，0-失败
*说        明：
********************************************************/
unsigned char Extract_Data(void)
{
	unsigned char temp = 0;
	unsigned char temp_1 = 0;
	temp = Rx_FIFO_ReadChar();
	temp_1 = Rx_FIFO_ReadChar();
	if( temp == MATCHING_CODE )
	{
		if(  temp_1 == 0x41 || temp_1 == 0x42 )
		{
			Rx_Data_Packet[0] = temp;
			Rx_Data_Packet[1] = temp_1; //来源
			Rx_Data_Packet[2] = Rx_FIFO_ReadChar(); //去向
			Rx_Data_Packet[3] = Rx_FIFO_ReadChar(); //光照+温度高
			Rx_Data_Packet[4] = Rx_FIFO_ReadChar(); //温度低
			Rx_Data_Packet[5] = Rx_FIFO_ReadChar(); // \0
			return 1;
		}
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
	unsigned char target_temp = 0;
	unsigned char source_temp = 0;
	unsigned char temp_data = 0;
	source_temp = Rx_Data_Packet[1];
	target_temp = Rx_Data_Packet[2];
	temp_data = Rx_Data_Packet[3] & 0x03;
	if( (target_temp == LOCAL_ADDRESS)&&(Rx_Data_Packet[4]!=MATCHING_CODE) )
	{	                              //是从机发给自己,而且末位未丢失
		if( (source_temp == NODE_1) || (source_temp == NODE_2) )
		{
			if( (temp_data==0x01) && (Rx_Data_Packet[4]>=0x40) )
			{
				Display_Data();//显示收到内容
			}
			else if( (temp_data==0x02) && (Rx_Data_Packet[4]<=0x50) )
			{
				Display_Data();//显示收到内容
			}
		}
	}
	else    //不是发给自己，不做处理
	{
		//UART_RX_OPEN();
	}
}

#endif


#ifdef MONITOR_NODE1
#define LOCAL_ADDRESS 0x41

void Display_Data(void){}      //为了兼容写空函数
void Display_FixedData(void){}
/********************************************************
*名        称：Packet_Data()
*功        能：从机打包要发送的数据
*入口参数：source_address-源地址
		  target_address-目标地址
*出口参数：无
*说        明：
********************************************************/
void Packet_Data( unsigned char source_address,unsigned char target_address )
{
	unsigned int temp_data = 0;
	unsigned int temp_temp = 0;
	unsigned char temp_low = 0;
	unsigned char temp_high = 0;
	unsigned char LDR_data = 0;
	LDR_data = Get_LDR();
	temp_data = DS18B20_ReadTempData();
	temp_temp = temp_data;
	temp_low = (unsigned char)temp_temp;
	temp_high =(unsigned char) (temp_data>>8);
	temp_high |= LDR_data; //将光照信息与温度数据高字节合并
	Tx_Data_Packet[0] = MATCHING_CODE;   //匹配码
	Tx_Data_Packet[1] = source_address;   //数据源地址码，可以是本机地址，也可以是中继源地址
	Tx_Data_Packet[2] = target_address;  //目标地址码
	Tx_Data_Packet[3] = temp_high;
	Tx_Data_Packet[4] = temp_low;
	Tx_Data_Packet[5] = '\0';
}

/********************************************************
*名        称：Extract_Data()
*功        能：提取接收到的数据帧
*入口参数：无
*出口参数：1-成功，0-失败
*说        明：
********************************************************/
unsigned char Extract_Data(void)
{
	unsigned char temp = 0;
	temp = Rx_FIFO_ReadChar();
	if( temp == MATCHING_CODE )
	{
		UART_TX_OPEN();
		Rx_Data_Packet[0] = temp;
		Rx_Data_Packet[1] = Rx_FIFO_ReadChar(); //来源
		Rx_Data_Packet[2] = Rx_FIFO_ReadChar(); //去向
		Rx_Data_Packet[3] = Rx_FIFO_ReadChar(); //光照+温度高
		Rx_Data_Packet[4] = Rx_FIFO_ReadChar(); //温度低
		Rx_Data_Packet[5] = '\0'; // \0
		return (1);
	}
	else return (0);
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
	unsigned char target_temp = 0;
	unsigned char temp_data = 0;
	temp_data = Rx_Data_Packet[3] & 0x03;
	target_temp = Rx_Data_Packet[2];
	if( target_temp == LOCAL_ADDRESS )
	{              //是主机发给自己（应答），不做处理
		//UART_RX_OPEN( );
	}
	else if(target_temp == NODE_2)
	{             //不是发给自己，继续发送给目标地址
		if( (temp_data==0x01) || (temp_data==0x02) )
		{
			UART_TX_OPEN();
			Tx_Data_Packet[0] = Rx_Data_Packet[0];
			Tx_Data_Packet[1] = Rx_Data_Packet[1];
			Tx_Data_Packet[2] = Rx_Data_Packet[2];
			Tx_Data_Packet[3] = Rx_Data_Packet[3];
			Tx_Data_Packet[4] = Rx_Data_Packet[4];
			Tx_Data_Packet[5] = Rx_Data_Packet[5];
			UART_SendString( Tx_Data_Packet );
			UART_RX_OPEN();
		}
	}
//	Tx_Data_Packet[0] = Rx_Data_Packet[0];
//	Tx_Data_Packet[1] = Rx_Data_Packet[1];
//	Tx_Data_Packet[2] = Rx_Data_Packet[2];
//	Tx_Data_Packet[3] = Rx_Data_Packet[3];
//	Tx_Data_Packet[4] = Rx_Data_Packet[4];
//	Tx_Data_Packet[5] = '\0';
//	UART_SendString( Tx_Data_Packet );
//	UART_RX_OPEN();
}
#endif

#ifdef MONITOR_NODE2
#define LOCAL_ADDRESS 0x42

void Display_Data(void){}      //为了兼容写空函数
void Display_FixedData(void){}
/********************************************************
*名        称：Packet_Data()
*功        能：从机打包要发送的数据
*入口参数：source_address-源地址
		  target_address-目标地址
*出口参数：无
*说        明：
********************************************************/
void Packet_Data( unsigned char source_address,unsigned char target_address )
{
	unsigned int temp_data = 0;
	unsigned int temp_temp = 0;
	unsigned char temp_low = 0;
	unsigned char temp_high = 0;
	unsigned char LDR_data = 0;
	LDR_data = Get_LDR();
	temp_data = DS18B20_ReadTempData();
	temp_temp = temp_data;
	temp_low = (unsigned char)temp_temp;
	temp_high =(unsigned char) (temp_data>>8);
	temp_high |= LDR_data; //将光照信息与温度数据高字节合并
	Tx_Data_Packet[0] = MATCHING_CODE;   //匹配码
	Tx_Data_Packet[1] = source_address;   //数据源地址码，可以是本机地址，也可以是中继源地址
	Tx_Data_Packet[2] = target_address;  //目标地址码
	Tx_Data_Packet[3] = temp_high;
	Tx_Data_Packet[4] = temp_low;
	Tx_Data_Packet[5] = '\0';
}

/********************************************************
*名        称：Extract_Data()
*功        能：提取接收到的数据帧
*入口参数：无
*出口参数：1-成功，0-失败
*说        明：
********************************************************/
unsigned char Extract_Data(void)
{
	unsigned char temp = 0;
	temp = Rx_FIFO_ReadChar();
	if( temp == MATCHING_CODE )
	{
		Rx_Data_Packet[0] = temp;
		Rx_Data_Packet[1] = Rx_FIFO_ReadChar(); //来源
		Rx_Data_Packet[2] = Rx_FIFO_ReadChar(); //去向
		Rx_Data_Packet[3] = Rx_FIFO_ReadChar(); //光照+温度高
		Rx_Data_Packet[4] = Rx_FIFO_ReadChar(); //温度低
		Rx_Data_Packet[5] = Rx_FIFO_ReadChar(); // \0
		return (1);
	}
	else return (0);
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
	unsigned char target_temp = 0;
	unsigned char temp_data = 0;
	temp_data = Rx_Data_Packet[3] & 0x03;
	target_temp = Rx_Data_Packet[2];
	if( target_temp == LOCAL_ADDRESS )
	{                //是主机发给自己（应答），不做处理
		//UART_RX_OPEN();
	}
	else if(target_temp == NODE_1)
	{               //不是发给自己，继续发送给目标地址
		if( (temp_data==0x01) || (temp_data==0x02) )
		{
			UART_TX_OPEN();
			Tx_Data_Packet[0] = Rx_Data_Packet[0];
			Tx_Data_Packet[1] = Rx_Data_Packet[1];
			Tx_Data_Packet[2] = Rx_Data_Packet[2];
			Tx_Data_Packet[3] = Rx_Data_Packet[3];
			Tx_Data_Packet[4] = Rx_Data_Packet[4];
			Tx_Data_Packet[5] = Rx_Data_Packet[5];
			UART_SendString( Tx_Data_Packet );
			UART_RX_OPEN();
		}
	}
}
#endif
