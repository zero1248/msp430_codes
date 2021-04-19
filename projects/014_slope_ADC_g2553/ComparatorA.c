/*
 * CampatorA.c
 *
 *  Created on: 2015-5-21
 *      Author: Isaac
 */
#include<msp430.h>
#include"ComparatorA.h"

/***********************************************************
*名       称：Comparator_Aplus_Init()
*功       能：比较器初始化
*入口参数：无
*出口参数：无
*说       明：比较器的同相端链接P1.5，反相端使用内部0.25*VCC参考电压，
		   启用滤波器，输出直接连接到捕获模块Timer_A CCI1B上
***********************************************************/
void Comparator_Aplus_Init(void)
{
	CACTL1 = CAEX + CAREF_1 + CAON;
	CACTL2 = P2CA3 +P2CA1 + CAF;
	CAPD = CAPD5;
}



