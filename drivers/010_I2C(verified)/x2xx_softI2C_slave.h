/*
 * softI2C.h
 *
 *  Created on: 2016Äê5ÔÂ12ÈÕ
 *      Author: Isaac
 */

#ifndef SOFTI2C_H_
#define SOFTI2C_H_

#include<stdbool.h>

#define I2C_SDA_INDIR    P1DIR &=~BIT2
#define I2C_SDA_OUTDIR   P1DIR |= BIT2
#define I2C_SCL_INDIR    P1DIR &=~BIT3
#define I2C_SCL_OUTDIR   P1DIR |= BIT3

#define I2C_SDA_HIGH     P1DIR &=~BIT2
#define I2C_SDA_LOW      P1DIR |= BIT2;P1OUT &=~BIT2
#define I2C_SCL_HIGH     P1DIR &=~BIT3
#define I2C_SCL_LOW      P1DIR |= BIT3;P1OUT &=~BIT3
#define I2C_SDA_IN       (P1IN  & BIT2)
#define I2C_SCL_IN       (P1IN  & BIT3)

void Slave_I2C_Init(void);
bool Slave_Get_Start(void);
bool Slave_Get_Stop(void);
bool Slave_Ack(void);
bool Slave_Wait_Ack(void);
unsigned char Slave_Get_Byte(void);
void Slave_Send_Byte(unsigned char i2c_data);
void Slave_Send_Word(unsigned int i2c_data);

#endif /* SOFTI2C_H_ */
