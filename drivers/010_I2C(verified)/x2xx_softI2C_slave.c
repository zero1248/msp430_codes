/*
 * softI2C.c
 *
 *  Created on: 2016年5月12日
 *      Author: Isaac
 */

#include<msp430.h>
#include"softI2C.h"

void Slave_I2C_Init(void){
	I2C_SDA_INDIR;
	I2C_SCL_INDIR;
}

bool Slave_Get_Start(void){
	I2C_SDA_HIGH;
	while(I2C_SDA_IN); //wait for SDA turn to low level
	if(I2C_SCL_IN){
		while(I2C_SCL_IN);
			return false;
	}
	return true;
}

bool Slave_Get_Stop(void){//if语句调用，要返回true，while语句调用，要返回false
	//unsigned int time_out = 100;
	I2C_SDA_HIGH;
	while(!I2C_SCL_IN){//wait for SDA turn to high level

	}
	if(!I2C_SDA_IN){
		while(!I2C_SDA_IN);
			return false;
	}
	return true;
}

bool Slave_Ack(void){
	I2C_SDA_HIGH;
	I2C_SDA_LOW;
	while(!I2C_SCL_IN);// wait for a valid clock
	while(I2C_SCL_IN); // wait to the end of a clock
	I2C_SDA_HIGH;
	return true;
}

bool Slave_Wait_Ack(void){
	I2C_SDA_HIGH;
	while(!I2C_SCL_IN);// wait for a valid clock
	if(!I2C_SDA_IN)
		return true;
	else
		return false;
}

unsigned char Slave_Get_Byte(void){
	unsigned char i2c_data = 0;
	unsigned char i = 0;
	for(i= 0;i<8;i++){
		while(!I2C_SCL_IN);// wait for a valid clock

		if(I2C_SDA_IN){
			i2c_data |=0x01;
		}
		if(i!=7)
			i2c_data <<= 1;

		while(I2C_SCL_IN); // wait to the end of a clock
	}
	return i2c_data;
}

void Slave_Send_Byte(unsigned char i2c_data){
	unsigned char i = 0;
	I2C_SDA_HIGH;
	for(i=0;i<8;i++){
		if(i2c_data & 0x80){
			I2C_SDA_HIGH;
		}
		else{
			I2C_SDA_LOW;
		}
		i2c_data <<= 1;
		while(!I2C_SCL_IN);// wait for a valid clock
		while(I2C_SCL_IN); // wait to the end of a clock
		__delay_cycles(2);
	}
	I2C_SDA_HIGH;
}

void Slave_Send_Word(unsigned int i2c_data){
	unsigned char i = 0;
	I2C_SDA_HIGH;
	for(i=0;i<16;i++){
		if(i2c_data & 0x8000){
			I2C_SDA_HIGH;
		}
		else{
			I2C_SDA_LOW;
		}
		i2c_data <<= 1;
		while(!I2C_SCL_IN);// wait for a valid clock
		while(I2C_SCL_IN); // wait to the end of a clock
		__delay_cycles(2);
	}
	I2C_SDA_HIGH;
}

