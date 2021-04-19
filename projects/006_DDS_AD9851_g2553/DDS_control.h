/*
 * DDS_control.h
 *
 *  Created on: 2015-6-5
 *      Author: Isaac
 */

#ifndef __DDS_CONTROL_H__
#define __DDS_CONTROL_H__


#define AD9851_W_CLK_HIGH    P2OUT |= BIT2  //P2.2口接ad9850的w_clk脚/PIN7
#define AD9851_W_CLK_LOW    P2OUT &= ~BIT2
#define AD9851_FQ_UD_HIGH    P2OUT |= BIT1  //P2.1口接ad9850的fq_ud脚/PIN8
#define AD9851_FQ_UD_LOW    P2OUT &= ~BIT1
#define AD9851_RESET_HIGH    P2OUT |= BIT0  //P2.0口接ad9850的reset脚/PIN22
#define AD9851_RESET_LOW    P2OUT &= ~BIT0
#define AD9851_DATA    P1OUT  //P1口接ad9850的8个数据脚

extern void AD9851_Reset_Parrel(void);
extern void AD9851_Write_Parrel(unsigned char w0,double frequence);
#endif /* DDS_CONTROL_H_ */
