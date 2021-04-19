/*
 * system_control.h
 *
 *  Created on: 2015-5-27
 *      Author: Isaac
 */
#ifndef __SYSTEM_CONTROL_H__
#define __SYSTEM_CONTROL_H__
//----条件编译选择发射/接收----
#define  LAUNCH
//#define  RECEIVE
extern void Wireless_IO_Init(void);
extern void Wireless_Key_Read(void);
extern void Wireless_Packet_Data(void);
//extern void TimerA_For_Key_Init(void);
#endif /* SYSTEM_CONTROL_H_ */
