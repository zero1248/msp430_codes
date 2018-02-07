/*
 * system_control.h
 *
 *  Created on: 2015-7-2
 *      Author: Isaac
 */

#ifndef __SYSTEM_CONTROL_H__
#define __SYSTEM_CONTROL_H__

extern unsigned char rec_foot;
extern unsigned char intsec_flag;//整0、20、40秒标识
extern unsigned char Receive_Data[20]; //接收到时钟数据

extern void TA_Capture0_Init(void);
extern void Simu_Clock_Open(void);
extern void Simu_Clock_Close(void);
extern void Display_Normal(void);
extern void Get_Data(void);
extern void Display_Fixed(void);

#endif /* SYSTEM_CONTROL_H_ */
