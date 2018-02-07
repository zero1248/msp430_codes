/*
 * photoresistance.h
 *
 *  Created on: 2015-5-1
 *      Author: Isaac
 */

#ifndef __PHOTORESISTANCE_H__
#define __PHOTORESISTANCE_H__

#define LCD_PORT_INIT P1DIR &= ~BIT5
#define LCD_READ P1IN & BIT5
extern void LDR_Init(void);
extern unsigned char Get_LDR(void);


#endif /* PHOTORESISTANCE_H_ */
