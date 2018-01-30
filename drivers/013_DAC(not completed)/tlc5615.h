/*
 * DAC10.h
 *
 *  Created on: 2015-5-14
 *      Author: Isaac
 */
/*这里采用TLC5615
 *4bit虚拟位（级联时需要），10bit数据位，2bit填充位
 * */
#ifndef __DAC10_H__
#define __DAC10_H__
#define DAC_IO_INIT   P1DIR |= BIT0+BIT1
#define DAC_SCLK_INIT P2DIR |= BIT2
#define CS_HIGH     P1OUT |= BIT0  //片选
#define CS_LOW      P1OUT &=~BIT0
#define SCLK_HIGH     P2OUT |= BIT2 //时钟
#define SCLK_LOW      P2OUT &=~BIT2
#define DIN_HIGH      P1OUT |= BIT1 //数据
#define DIN_LOW      P1OUT &=~BIT1

extern void DAC10_Init(void);
extern void DAC10_WriteData(unsigned int data);

#endif /* DAC10_H_ */
