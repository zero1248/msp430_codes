/*
 * 18B20.h
 *
 *  Created on: 2015-4-29
 *      Author: Isaac
 */

#ifndef __18B20_H__
#define __18B20_H__

#define DS18B20_WRITE_SET  P1DIR |= BIT4   //设置为写模式,每次操作完设置为写模式
#define DS18B20_WRITE_HIGH P1OUT |= BIT4   //向18B20输出高电平
#define DS18B20_WRITE_LOW  P1OUT &= ~BIT4//输出低电平
#define DS18B20_READ_SET   P1DIR &= ~BIT4  //设置为读模式
#define DS18B20_READ_DATA  P1IN & BIT4     //读取相应IO口数据

#define DS18B20_MCLK_FREQ  8000000   //主系统时钟
#define COMMAND_LSB 0x01
#define COMMAND_MSB 0x80

extern unsigned int DS18B20_temperature;

extern void DS18B20_Init(void);
extern void DS18B20_TADelay_us(unsigned int n);
extern void DS18B20_WriteChar(unsigned char command);
extern unsigned char DS18B20_ReadChar(void);
extern unsigned int DS18B20_ReadTemp(void);//得到温度数值
extern unsigned int DS18B20_ReadTempData(void);//得到温度数据


#endif /* 18B20_H_ */
