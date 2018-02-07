/*
 * electric_monitor.c
 *
 *  Created on: 2015-5-1
 *      Author: Isaac
 */

#include<msp430g2553.h>
#include"electric_monitor.h"


/***********************************************************
*名       称：Electric_Monitor_Init()
*功       能：电源监测端口初始化
*入口参数：无
*出口参数：无
*说       明：将P1.3方向设为输入
***********************************************************/
void Electric_Monitor_Init(void)
{
	ELECTRIC_PORT_INIT;
}

/***********************************************************
*名       称：Get_Electric()
*功       能：得到电源监测数据
*入口参数：无
*出口参数：1-高电平 0-低电平
*说       明：比较器输出高时表示电源电量足
***********************************************************/
unsigned char Get_Electric(void)
{
	unsigned char electric_data = 0;
	electric_data = ELECTRIC_MONITOR;
	return ( electric_data );
}
