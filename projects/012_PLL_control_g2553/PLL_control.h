/*
 * PLL_control.h
 *
 *  Created on: 2015-7-8
 *      Author: Isaac
 */

#ifndef __PLL_CONTROL_H__
#define __PLL_CONTROL_H__

#define ENB_HIGH   P1OUT |= BIT3
#define ENB_LOW    P1OUT &=~BIT3
#define CLK_HIGH   P1OUT |= BIT0
#define CLK_LOW    P1OUT &=~BIT0
#define CLK_ACTION P1OUT |= BIT0;P1OUT &=~BIT0
#define ADin_HIGH  P1OUT |= BIT1
#define ADin_LOW   P1OUT &=~BIT1
#define Din_HIGH   P1OUT |= BIT2
#define Din_LOW    P1OUT &=~BIT2
//----控制字----
#define MSB_1            Din_HIGH//写控制字
#define MSB_0            Din_LOW //写参考频率
#define TEST_BIT         Din_LOW //非测试模式
#define AUX_DATA_SELECT  Din_HIGH//ADin输入16位发射数据 Din输入16位接收数据
#define REF_OUT          Din_LOW //REFout输出频率=OSCout/4
#define TxPD_ENABLE      Din_HIGH//关闭发送使能
#define RxPD_ENABLE      Din_LOW
#define REFPD_ENABLE     Din_LOW
//----参考频率----
#define AUX_REF_ENABLE   Din_LOW //关闭辅助参考频率计数器
#define Tx_0_SELECT      Din_HIGH
#define Rx_0_SELECT      Din_HIGH
#define REF_DATA_12      2400
#define F_R1_S1          Din_LOW
#define F_R1_S2          Din_HIGH
#define AUX_REF_DATA_14  0
//----Rx_Counter----
#define Rx_16            7000

extern void ConReg_Emit(void);
extern void RefFre_Emit(unsigned int fre);
extern void Tx_Counter_Emit(unsigned int Rx_data);

#endif /* PLL_CONTROL_H_ */
