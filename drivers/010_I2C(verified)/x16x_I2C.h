/*
 * I2C169.h
 *        Port: P1.6-SCL P1.7-SDA
 *  Created on: 2015-6-26
 *      Author: Isac
 */

#ifndef __I2C_H__
#define __I2C_H__

#define I2C_I2C_MCLK_FREQ 8000000  //实际时钟频率
//-----屏蔽硬件差异，对IO进行宏定义-----
#define I2C_CLK_HIGH  P1DIR &= ~BIT6
#define I2C_CLK_LOW   P1DIR |= BIT6;P1OUT &= ~BIT6
#define I2C_DATA_HIGH P1DIR &= ~BIT7
#define I2C_DATA_LOW  P1DIR |= BIT7;P1OUT &= ~BIT7
#define I2C_DATA_IN   P1IN & BIT7
#define I2C_START I2C_Start()
#define I2C_STOP  I2C_Stop()
//-----从机地址宏及读写操作位的宏定义-----
#define SLAVE_ADDR  0x20  //实际从机地址
#define SLAVE_ADDR_W  SLAVE_ADDR<<1  //实际从机地址+写标志。不用修改
#define SLAVE_ADDR_R  (SLAVE_ADDR<<1)+1//实际从机地址+读标志。不用修改

extern void I2C_Init();
extern void I2C_Tx_Init();
extern void I2C_Rx_Init();
extern unsigned char I2C_TxFrame(unsigned char *p_Tx , unsigned char num);
extern unsigned char I2C_RxFrame(unsigned char *p_Rx , unsigned char num);
extern void I2C_Send_Char(unsigned char data);
extern void I2C_Master_Ask(void);
extern unsigned char I2C_Get_Char(void);
extern void I2C_Start(void);
extern void I2C_Stop(void);

#endif /* I2C_H_ */
