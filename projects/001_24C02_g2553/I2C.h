/*
 * I2C.h for EEPROM(more strict than others)
 *        Port: P1.6-SCL P1.7-SDA
 *  Created on: 2015-4-27
 *      Author: Isac
 */

#ifndef __I2C_H__
#define __I2C_H__
#include<msp430g2553.h>

//define HARD_I2C      //选择硬件或软件I2C
#define SOFT_I2C

#define I2C_MCLK_FREQ 8000000  //实际时钟频率
//-----屏蔽硬件差异，对IO进行宏定义-----
#define I2C_CLK_HIGH  P1DIR &= ~BIT6
#define I2C_CLK_LOW   P1DIR |= BIT6;P1OUT &= ~BIT6
#define I2C_DATA_HIGH P1DIR &= ~BIT7
#define I2C_DATA_LOW  P1DIR |= BIT7;P1OUT &= ~BIT7
#define I2C_DATA_IN   (P1IN & BIT7)
#define I2C_SDA_IN    P1DIR &= ~BIT7
#define I2C_SDA_OUT   P1DIR |= BIT7
#define I2C_START I2C_Start()
#define I2C_STOP  I2C_Stop()
//-----从机地址宏及读写操作位的宏定义-----
#define SLAVE_ADDR  0xa0  //实际从机地址(24C02)
#define SLAVE_ADDR_W  SLAVE_ADDR|0x00  //实际从机地址+写标志。不用修改
#define SLAVE_ADDR_R  SLAVE_ADDR|0x01//实际从机地址+读标志。不用修改
//--------------返回参数宏定义--------------
#define TRUE  1
#define FALSE 0
//-----------------延时宏定义----------------------
#define DELAY_2US  __delay_cycles(8);__delay_cycles(8)
//-----------------------初始化-----------------------
static void I2C_delay_us(void);
extern void I2C_Init(void);
extern void I2C_Tx_Init(void);
extern void I2C_Rx_Init(void);
//----------------------应答--------------------------
extern void I2C_Master_Ask(void);
extern void I2C_Master_NoAsk(void);
extern unsigned char I2C_Check_SlaveAsk(void);
//---------------------数据传输------------------------
void I2C_Write1(void);
void I2C_Write0(void);
void I2C_Send_Char(unsigned char);
unsigned char I2C_Get_Char(void);
extern unsigned char I2C_TxFrame(unsigned char *p_Tx , unsigned char);
extern unsigned char I2C_RxFrame(unsigned char *p_Rx , unsigned char);
extern void I2C_Start(void);
extern void I2C_Stop(void);

#endif /* I2C_H_ */
