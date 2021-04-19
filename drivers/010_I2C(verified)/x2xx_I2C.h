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
