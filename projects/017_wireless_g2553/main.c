/*
 * main.c
 *
 *  Created on: 2015-4-27
 *      Author: Isaac
 */
#include<msp430.h>
#include"my_430.h"
#include"com_protocol.h"
#include"BCL.h"
#include"OLED.h"
#include"I2C.h"
#include"18B20.h"
#include"codetab.h"
#include"UART.h"
#include"electric_monitor.h"
#include"photoresistance.h"

#ifdef MONITOR_TERMINAL
void main(void)
{
	unsigned char j = 0;
	unsigned char i = 0;
	WDT_OUT;
	TACTL |= TASSEL_2 + ID_3;
	Port_Init();    		//引脚初始化
	BCS_Init();     		//时钟初始化
	OLED_I2C_Init();		//这里直接调用I2C_Init()
	OLED_Init();    		//OLED初始化
					//LDR_Init();     		//光敏电阻初始化
					//DS18B20_Init(); 		//18B20初始化
	Electric_Monitor_Init();//主机电量初始化
	UART_RX_OPEN();         //串口接收状态
	Display_FixedData();    //显示固定部分
	_EINT();
	while(1)
	{
		for( j=15 ; j>0 ; j-- )
			DS18B20_TADelay_us(5000);
		i = Extract_Data();
		if( i )         //提取数据
			Analysis_Data();        //解析数据
		DS18B20_TADelay_us(8000);
		DS18B20_TADelay_us(8000);
	}
}
#endif

#ifdef MONITOR_NODE1
void main(void)
{
	unsigned char i;
	unsigned char j;
	WDT_OUT;
	Port_Init();    		//引脚初始化
	BCS_Init();     		//时钟初始化
	             //OLED_I2C_Init();		//这里直接调用I2C_Init()
	             //OLED_Init();    		//OLED初始化
	TACTL |= TASSEL_2 + ID_3;
	_EINT();
	DS18B20_Init(); 		//18B20初始化
	LDR_Init();     		//光敏电阻初始化
	             //Electric_Monitor_Init();//主机电量初始化
	//UART_TXRX_OPEN();         //串口接收状态
	//UART_TX_OPEN();
	UART_RX_OPEN();
	P1SEL |= BIT0;
	while(1)
	{
		Packet_Data( NODE_1 , HOST_ADDRESS );//发送数据
		UART_TX_OPEN();
		UART_SendString(Tx_Data_Packet);
		UART_RX_OPEN();
		for( j=10 ; j>0 ; j-- )
			DS18B20_TADelay_us(5000);
		i = Extract_Data();
		if( i )         //提取数据
			Analysis_Data();        //解析数据
		DS18B20_TADelay_us(8000);
		DS18B20_TADelay_us(8000);
	}
}
#endif

#ifdef MONITOR_NODE2
void main(void)
{
	unsigned char i;
	unsigned char j;
	WDT_OUT;
	Port_Init();    		//引脚初始化
	BCS_Init();     		//时钟初始化
	             //OLED_I2C_Init();		//这里直接调用I2C_Init()
	             //OLED_Init();    		//OLED初始化
	TACTL |= TASSEL_2 + ID_3;
	_EINT();
	DS18B20_Init(); 		//18B20初始化
	LDR_Init();     		//光敏电阻初始化
	             //Electric_Monitor_Init();//主机电量初始化
	//UART_TXRX_OPEN();         //串口接收状态
	//UART_TX_OPEN();
	UART_RX_OPEN();
	P1SEL |= BIT0;
	while(1)
	{
		Packet_Data( NODE_2 , HOST_ADDRESS );//发送数据
		UART_TX_OPEN();
		UART_SendString(Tx_Data_Packet);
		UART_RX_OPEN();
		for( j=10 ; j>0 ; j-- )
		DS18B20_TADelay_us(5000);
		i = Extract_Data();
		if( i )         //提取数据
			Analysis_Data();        //解析数据
		DS18B20_TADelay_us(8000);
		DS18B20_TADelay_us(8000);
	}
}
#endif



