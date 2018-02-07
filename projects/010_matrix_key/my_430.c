/*
 *  my_430.c
 *  for msp430f149/169
 *  Created on: 2015-6-26
 *      Author: Isaac
 */

#include"my_430.h"
#include<msp430x14x.h>

void Clk_Init(){       //初始化时钟
    unsigned int i;
    BCSCTL1 &=~XT2OFF;//Basic Clock System Control 1,Enable XT2CLK
                       //即打开XT2
    do {
        IFG1 &= ~OFIFG;        // IFG1:Interrupt Flag 1                                                // 清除振荡器失效标志
        for(i=0xFF;i>0;i--);// 延时，等待XT2起振
    }
    while((IFG1 & OFIFG) != 0);// 判断XT2是否起振,OFIFG=0则起振
    BCSCTL2 =SELM1+SELS;       //MCLK（SELM1）,SMCLK时钟为XT2,都为8MHZ
}
