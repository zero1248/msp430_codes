/*
 * 24C02.h
 *
 *  Created on: 2015-5-12
 *      Author: Isaac
 */

#ifndef __24C02_H__
#define __24C02_H__
//----------------读写定义------------------
//默认芯片上A0A1A2（地址位第321位，第0位是读写控制）全部接地
#define READ_24C02  0xa1     //读操作
#define WRITE_24C02 0xa0     //写操作

extern void EEPROM_delay_ms(void);
extern void EEPROM_Init(void);
unsigned char EEPROM_WriteData(unsigned char,unsigned char);
unsigned char EEPROM_ReadCurrChar(void);
unsigned char EEPROM_ReadCurrData(unsigned char*,unsigned char);
unsigned char EEPROM_ReadSpecChar(unsigned char);
unsigned char EEPROM_ReadSpecData(unsigned char,unsigned char*,unsigned char);
unsigned char EEPROM_Clear(void);
#endif /* __24C02_H__ */
