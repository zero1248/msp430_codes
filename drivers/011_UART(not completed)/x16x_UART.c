/*
 * UART196.c
 *
 *  Created on: 2015-6-26
 *      Author: Isaac
 */
#include<msp430x16x.h>
#include"UART169.h"
typedef unsigned char uchar;
//unsigned char num[128]={0};

/*void Clk_Init(){       //初始化时钟
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

/*******************************************
函数名称：InitUART
功    能：初始化UART端口
参    数：无
返回值  ：无
********************************************/
void InitUART(void)
{
    P3SEL |= 0x30;                            // P3.4,5 = USART0 TXD/RXD
    ME1 |= URXE0 + UTXE0;                             // Enable USART0 T/RXD
    UCTL0 |= CHAR;                            // 8-bit character
    UTCTL0 |= SSEL0;                          // UCLK = ACLK
    UBR00 = 0x03;                             // 32k/9600 - 3.41
    UBR10 = 0x00;                             //
    UMCTL0 = 0x4A;                            // Modulation
    UCTL0 &= ~SWRST;                          // Initialize USART state machine
}
/*******************************************
函数名称：Send1Char
功    能：向PC机发送一个字符
参    数：sendchar--要发送的字符
返回值  ：无
********************************************/
void Send1Char(uchar sendchar)
{
      while (!(IFG1 & UTXIFG0));    //等待发送寄存器为空
      TXBUF0 = sendchar;

}
/*******************************************
函数名称：PutSting
功    能：向PC机发送字符串
参    数：ptr--指向发送字符串的指针
返回值  ：无
********************************************/
void PutString(uchar *ptr)
{
      while(*ptr != '\0')
      {
            Send1Char(*ptr++);                     // 发送数据
      }
      while (!(IFG1 & UTXIFG0));
      TXBUF0 = '\n';                              //发送换行指令
}
/*******************************************
函数名称：Get1Char
功    能：接收一个来自PC机的字符
参    数：无
返回值  ：接收到的字符
********************************************/
uchar Get1Char(void)
{
    while (!(IFG1 & URXIFG0));    //等待接收到字符
    return  RXBUF0;
}

/*void main(void)
{
    unsigned char i;
    //下面六行程序关闭所有的IO口
    P1DIR = 0XFF;P1OUT = 0XFF;
    P2DIR = 0XFF;P2OUT = 0XFF;
    P3DIR = 0XFF;P3OUT = 0XFF;
    P4DIR = 0XFF;P4OUT = 0XFF;
    P5DIR = 0XFF;P5OUT = 0XFF;
    P6DIR = 0XFF;P6OUT = 0XFF;
    WDTCTL = WDTPW + WDTHOLD;   //停止看门狗
    InitUART();                 //初始化UART端口
	Clk_Init();
	while(1)
	{
		for( i=0 ; i<128 ; i++ )
		{
		    num[i] = Get1Char();
			Send1Char(num[i]);
		}
	}
}*/


