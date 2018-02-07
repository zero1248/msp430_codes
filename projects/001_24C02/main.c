/*
 * main.c
 *
 *  Created on: 2015-5-12
 *      Author: Isaac
 */
#include<msp430.h>
#include"24C02.h"
#include"BCL.h"
#include"my_430.h"

unsigned char receive[10]="0000000000";
unsigned char flag_ = 0;
void delay(unsigned int x)
{
	unsigned int i=0,j=0;
  	for(i=x;i>0;i--)
  		for(j=110;j>0;j--);
}

void main(void)
{

	WDT_OUT;
	Port_Init();
	BCS_Init();
	EEPROM_Init();
	//EEPROM_Clear();
//	P1OUT &= ~BIT0;
	if(EEPROM_WriteData(0x44,0x41))
		flag_ = 1;
	_NOP();
	receive[0] = EEPROM_ReadSpecChar(0x44);
	if(receive[0]==0x41)
		flag_ = 2;
	_NOP();

	while(1);
}



