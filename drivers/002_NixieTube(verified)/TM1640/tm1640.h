/*
 * tm1640.h
 *
 *  Created on: 2015-11-7
 *      Author: Isaac
 */
//for tm1640(16 grid 8 segment nixie tube driver chip)
#ifndef __NIXIE_TUBE_H__
#define __NIXIE_TUBE_H__

//TM1640CLK、DIN输入引脚设置
#define TM1640_SCLK_INIT P2DIR |= BIT1;P2OUT |= BIT1
#define TM1640_DIN_INIT  P2DIR |= BIT2;P2OUT |= BIT2
#define TM1640_SCLK_HIGH P2OUT |= BIT1
#define TM1640_SCLK_LOW  P2OUT &=~BIT1
#define TM1640_DIN_HIGH  P2OUT |= BIT2
#define TM1640_DIN_LOW   P2OUT &=~BIT2



//TM1640数据命令
#define TM1640_ADD1_MODE    0x40
#define TM1640_FIXED_MODE   0x44
//TM1640地址命令
#define TM1640_ADDRESS_0    0xC0
#define TM1640_ADDRESS_1    0xC1
#define TM1640_ADDRESS_2    0xC2
#define TM1640_ADDRESS_3    0xC3
#define TM1640_ADDRESS_4    0xC4
#define TM1640_ADDRESS_5    0xC5
#define TM1640_ADDRESS_6    0xC6
#define TM1640_ADDRESS_7	0xC7
#define TM1640_ADDRESS_8	0xC8
#define TM1640_ADDRESS_9	0xC9
#define TM1640_ADDRESS_A	0xCA
#define TM1640_ADDRESS_B	0xCB
#define TM1640_ADDRESS_C	0xCC
#define TM1640_ADDRESS_D	0xCD
#define TM1640_ADDRESS_E	0xCE
#define TM1640_ADDRESS_F	0xCF
//TM1640控制显示命令
#define TM1640_OFF	        0x80
#define TM1640_BRIGHTNESS_0 0x88
#define TM1640_BRIGHTNESS_1 0x89
#define TM1640_BRIGHTNESS_2 0x8A
#define TM1640_BRIGHTNESS_3 0x8B
#define TM1640_BRIGHTNESS_4 0x8C
#define TM1640_BRIGHTNESS_5 0x8D
#define TM1640_BRIGHTNESS_6 0x8E
#define TM1640_BRIGHTNESS_7 0x8F

#define a_             0x01             // AAAAAAAA
#define b_             0x02             // F      B
#define c_             0x04             // F      B
#define d_             0x08             // GGGGGGGG
#define e_             0x10             // E      C
#define f_             0x20             // E      C  ...
#define g_             0x40             // DDDDDDDD  ...
#define DOTSEG         0x80
#define NEGSEG         0x40       //DOTSEG是小数点 NEGSEG是负号

extern void Start_Tm1640(void);
extern void Stop_Tm1640(void);
extern void Tm1640_Input(unsigned char , unsigned char , float );
#endif /* NIXIE_TUBE_H_ */
