/*
 * uPD6450_control.h
 *
 *  Created on: 2015-8-13
 *      Author: Isaac
 */

#ifndef UPD6450_CONTROL_H_
#define UPD6450_CONTROL_H_

#define BUSY_INIT P1DIR &=~BIT0
#define CLK_INIT  P1DIR |= BIT1
#define STB_INIT  P1DIR |= BIT2
#define DATA_INIT P1DIR |= BIT3

#define CLK_0  P1OUT &=~BIT1
#define CLK_1  P1OUT |= BIT1
#define STB_0  P1OUT &=~BIT2
#define STB_1  P1OUT |= BIT2
#define DATA_0 P1OUT &=~BIT3
#define DATA_1 P1OUT |= BIT3

#define DATA__0 CLK_0;DATA_0;CLK_1
#define DATA__1 CLK_0;DATA_1;CLK_1
#define STB__1  STB_1;STB_0

#define BANK_0 0
#define BANK_1 1
#define CHARACTER_B 18
#define CHARACTER_C 19
#define BLINK_0 0x80
#define BLINK_1 0x88

extern void uPD6450_Port_Init(void);
extern void uPD6450_Send_Byte(unsigned char data);
extern void uPD6450_Format_Release(void);
extern void uPD6450_Display_Character(unsigned char cha);
extern void uPD6450_Display_Address(unsigned char line,unsigned char column);
extern void uPD6450_Blink_Set(unsigned char blink);
extern void uPD6450_Display_Position(unsigned char vertical,unsigned char horizontal);
extern void uPD6450_NP_EI_Osc(void);
extern void uPD6450_ON_BL_LCosc(void);

#endif /* UPD6450_CONTROL_H_ */
