/*
 *  electric_monitor.h
 *        Port: P1.3
 *  Created on: 2015-5-1
 *      Author: Isaac
 */

#ifndef __ELECTRIC_MONITOR_H__
#define __ELECTRIC_MONITOR_H__

#define ELECTRIC_PORT_INIT P1DIR &= ~BIT3
#define ELECTRIC_MONITOR P1IN &= BIT3

extern void Electric_Monitor_Init(void);
extern unsigned char Get_Electric(void);

#endif /* ELECTRIC_MONITOR_H_ */
