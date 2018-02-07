/*
 * I2C.h
 *        Port: P1.6-SCL P1.7-SDA
 *  Created on: 2015-4-27
 *      Author: Isac
 */

#ifndef __I2C_H__
#define __I2C_H__
#include<msp430g2553.h>

//define HARD_I2C      //选择硬件或软件I2C
#define SOFT_I2C

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
