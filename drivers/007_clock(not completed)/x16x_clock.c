#include"clock.h"
#include<msp430x16x.h>

/****************************************************************************
* 名    称：Clk_Init()
* 功    能：初始化时钟，MCLK，SMCLK都为8Mhz
* 入口参数：无
* 出口参数：无
****************************************************************************/
/*void Clk_Init(){       //初始化时钟   在my_430中已定义
    unsigned int i;
    BCSCTL1 &=~XT2OFF;//Basic Clock System Control 1,Enable XT2CLK
                       //即打开XT2
    do {
        IFG1 &= ~OFIFG;        // IFG1:Interrupt Flag 1                                                // 清除振荡器失效标志
        for(i=0xFF;i>0;i--);// 延时，等待XT2起振
    }
    while((IFG1 & OFIFG) != 0);// 判断XT2是否起振,OFIFG=0则起振
    BCSCTL2 =SELM1+SELS;       //MCLK（SELM1）,SMCLK时钟为XT2,都为8MHZ
}*/
