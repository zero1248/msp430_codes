/*
 * 24C02.c
 *
 *  Created on: 2015-5-12
 *      Author: Isaac
 */
#include<msp430.h>
#include"24C02.h"
#include"I2C.h"

/***********************************************************
*名       称：EEPROM_delay_ms()
*功       能：10ms精确延时
*入口参数：无
*出口参数：无
*说       明：实际延时值与CPU时钟频率有关，所以用SI2C_MCLK_FREQ来自动调整
	             不同的I2C设备速度不一样，延时参数视情况而定
***********************************************************/
void EEPROM_delay_ms(void){//默认取空函数
	__delay_cycles(80000);
	//__delay_cycles(I2C_MCLK_FREQ / 10000);   //100us
	//__delay_cycles(I2C_MCLK_FREQ / 100000);  //10us
	//__delay_cycles(I2C_MCLK_FREQ / 1000000); //1us
}

/********************************************************
*名       称：EEPROM_Init()
*功       能：EEPROM的初始化函数
*入口参数：无
*出口参数：无
*说       明：直接调用I2C_Init()函数
********************************************************/
void EEPROM_Init(void){
	I2C_Init();
}

/********************************************************
*名       称：EEPROM_WriteData()
*功       能：向EEPROM写入数据(8bit)
*入口参数：address （0x00-0xff）， data
*出口参数：1
*说       明：主机发送后等待从机应答即可，这里用主动查询应答代替等待
********************************************************/
unsigned char EEPROM_WriteData(unsigned char address , unsigned char data){
	I2C_Start();
	I2C_Send_Char( WRITE_24C02 );//写指令
	if( I2C_Check_SlaveAsk() )
		I2C_Send_Char( address );
	else
		return 0;
	if( I2C_Check_SlaveAsk() )
		I2C_Send_Char( data );
	else
		return 0;
	if( I2C_Check_SlaveAsk() )
		I2C_Stop();
	else
		return 0;
	EEPROM_delay_ms();//等待完成内部写入
	return 1;
}

/********************************************************
*名       称：EEPROM_ReadCurrChar()
*功       能：从EEPROM读出当前地址字节
*入口参数：无
*出口参数：data
*说       明：主机发送后等待从机应答即可，这里用主动查询应答代替等待
		  需要改变传送方向时，需将其实信号和从机地址各重复产生一遍
********************************************************/
unsigned char EEPROM_ReadCurrChar( void ){
	unsigned char data;
	I2C_Start();
	I2C_Send_Char( WRITE_24C02 );//写指令
	if( I2C_Check_SlaveAsk() )
		data = I2C_Get_Char();
	else
		return 0;
	I2C_Master_NoAsk();
	I2C_Stop();
	return (data);
}

/********************************************************
*名       称：EEPROM_ReadCurrData()
*功       能：从EEPROM读出当前地址N个字节
*入口参数：*Tx_-存放的数组
		  n-数据长度
*出口参数：data
*说       明：主机发送后等待从机应答即可，这里用主动查询应答代替等待
		  需要改变传送方向时，需将其实信号和从机地址各重复产生一遍
********************************************************/
unsigned char EEPROM_ReadCurrData( unsigned char *Tx_ , unsigned char n ){
	I2C_Start();
	I2C_Send_Char( WRITE_24C02 );//写指令
	if( I2C_Check_SlaveAsk() )
		I2C_RxFrame( Tx_ , n );
	else
		return 0;
	return 1;
}

/********************************************************
*名       称：EEPROM_ReadSpecChar()
*功       能：从EEPROM读出指定地址字节
*入口参数：address （0x00-0xff）
*出口参数：data
*说       明：主机发送后等待从机应答即可，这里用主动查询应答代替等待
		  需要改变传送方向时，需将其实信号和从机地址各重复产生一遍
********************************************************/
unsigned char EEPROM_ReadSpecChar( unsigned char address ){
	unsigned char data;
	I2C_Start();
	I2C_Send_Char( WRITE_24C02 );//写指令
	if( I2C_Check_SlaveAsk() )
	{
		I2C_Send_Char( address );
	}
	else
		return 0;
	if( I2C_Check_SlaveAsk() )
	{
		I2C_Start();
		I2C_Send_Char( READ_24C02 );//读指令
	}
	else
		return 0;
	if( I2C_Check_SlaveAsk() )
	{
		data = I2C_Get_Char();
	}
	else
		return 0;
	I2C_Master_NoAsk();
	I2C_Stop();
	return (data);
}

/********************************************************
*名       称：EEPROM_ReadSpecData()
*功       能：从EEPROM读出指定地址N个字节
*入口参数：address （0x00-0xff）
*         *Tx_-存放数据的数组
*         n-数据长度
*入口参数：address
*出口参数：data
*说       明：主机发送后等待从机应答即可，这里用主动查询应答代替等待
		  需要改变传送方向时，需将其实信号和从机地址各重复产生一遍
********************************************************/
unsigned char EEPROM_ReadSpecData(unsigned char address,unsigned char *Tx_,unsigned char n  ){
	I2C_Start();
	I2C_Send_Char( WRITE_24C02 );//写指令
	if( I2C_Check_SlaveAsk() )
	{
		I2C_Send_Char( address );
	}
	else
		return 0;
	if( I2C_Check_SlaveAsk() )
	{
		I2C_Start();//重新开始
		I2C_Send_Char( READ_24C02 );//读指令
	}
	else
		return 0;
	if( I2C_Check_SlaveAsk() )
	{
		I2C_RxFrame( Tx_ , n );
	}
	else
		return 0;
	return 1;
}

/********************************************************
*名       称：EEPROM_Clear()
*功       能：EEPROM清空数据
*入口参数：无
*出口参数：无
*说       明：通过写0实现（页写入，防止“上卷”），模拟查询应答
********************************************************/
unsigned char EEPROM_Clear(void){
	unsigned char address = 0;
	unsigned char i = 0 , j = 0;
	I2C_Start();
	I2C_Send_Char( WRITE_24C02 );//写指令
	for( i=0 ;i<32 ; i++)
	{
		I2C_Start();
		I2C_Send_Char( WRITE_24C02 );//写指令
		address = address + i*8;
		I2C_Send_Char( address );
		for( j=0 ; j<8 ; j++ )
		{
			I2C_Send_Char(0x00);
			I2C_CLK_HIGH; //模拟接收从机应答
			EEPROM_delay_ms();
			I2C_CLK_LOW;
			EEPROM_delay_ms();
		}
		I2C_Stop();
	}
	return 1;
}
