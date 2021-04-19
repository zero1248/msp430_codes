/*
 * system_control.c
 *
 *  Created on: 2015-5-27
 *      Author: Isaac
 */
#include<msp430g2553.h>
#include"system_control.h"
#include"TimerA_PWM.h"

#ifdef LAUNCH
//----按键均加上拉电阻,不按下时为高电平----
#define KEY_1       (P1IN & BIT3)==0
#define KEY_2       (P1IN & BIT4)==0
#define KEY_3       (P1IN & BIT5)==0
#define KEY_4       (P2IN & BIT0)==0
#define KEY_CONTROL (P2IN & BIT1)
#define LED1_ON      P2OUT |= BIT2
#define LED2_ON      P2OUT |= BIT3
#define LED3_ON      P2OUT |= BIT4
#define LED4_ON      P2OUT |= BIT5
#define LED_DEGREE1   P2OUT = BIT2
#define LED_DEGREE2   P2OUT = BIT3
#define LED_DEGREE3   P2OUT = BIT4
#define LED_DEGREE4   P2OUT = BIT5
#define LED1_OFF       P2OUT &= ~BIT2
#define LED2_OFF       P2OUT &= ~BIT3
#define LED3_OFF       P2OUT &= ~BIT4
#define LED4_OFF       P2OUT &= ~BIT5
#define DATA_INIT      P2OUT =   0x00
#define PT2262_TE_ON     P1OUT &=~BIT6
#define PT2262_TE_OFF    P1OUT |= BIT6
//void TimerA_For_Key_Init(void){}

/********************************************************
*名       称：Wireless_IO_Init()
*功       能：初始化本无线控制系统的IO口
*入口参数：无
*出口参数：无
*说       明：P1.3/1.4/1.5/2.0为拨码开关输入,P2.1为自锁开关输入
		  P2.2/2.3/2.4/2.5为输出 p1.6输出TE
********************************************************/
void Wireless_IO_Init(void)
{
	P1DIR = 0xc7;
	P2DIR = 0xfc;
	DATA_INIT;
	PT2262_TE_OFF;
}
/********************************************************
*名       称：Wireless_Packet_Data()
*功       能：读取键值从而控制输出信号
*入口参数：无
*出口参数：无
*说       明：自锁开关断开时，控制4个LED亮灭
		   自锁开关闭合时，控制一个LED亮度
********************************************************/
void Wireless_Packet_Data(void)
{
	unsigned char LED_data = 0;
	unsigned char data_temp = 0;
	if(KEY_CONTROL)//控制四个灯
	{
		LED_data |= BIT7;//最高位表示后发的四位数据
		LED_data &=~BIT3;//第三位置低表示先发的四位数据
		LED_data |= (BIT4+BIT0);//模式选择
		if( KEY_1 )  LED_data |= BIT6;
		if( KEY_2 )  LED_data |= BIT5;
		if( KEY_3 )  LED_data |= BIT2;
		if( KEY_4 )  LED_data |= BIT1;
		data_temp = LED_data&0x0f;
		data_temp <<= 2;
		P2OUT = data_temp;
		PT2262_TE_ON;   //发射脉冲
		PT2262_TE_OFF;
		__delay_cycles(800000);
		data_temp = LED_data>>4;
		data_temp <<= 2;
		P2OUT = data_temp;
		PT2262_TE_ON;   //发射脉冲
		PT2262_TE_OFF;
		__delay_cycles(800000);
	}
	LED_data = 0;
	data_temp = 0;
	if(KEY_CONTROL==0x00)    //控制一个灯的亮度
	{
		LED_data |= BIT7;//最高位表示先发的四位数据
		LED_data &=~BIT3;//第三位置低表示先发的四位数据
		LED_data &=~(BIT4+BIT0);//模式选择
		if( KEY_1 )  LED_data |= BIT6;
		if( KEY_2 )  LED_data |= BIT5;
		if( KEY_3 )  LED_data |= BIT2;
		if( KEY_4 )  LED_data |= BIT1;
		data_temp = LED_data&0x0f;
		data_temp <<= 2;
		P2OUT = data_temp;
		PT2262_TE_ON;   //发射脉冲
		PT2262_TE_OFF;
		__delay_cycles(800000);
		data_temp = LED_data>>4;
		data_temp <<= 2;
		P2OUT = data_temp;
		PT2262_TE_ON;   //发射脉冲
		PT2262_TE_OFF;
		__delay_cycles(800000);
	}
}

#endif

#ifdef RECEIVE
#define LED1_ON     P1OUT &= ~BIT4
#define LED2_ON     P1OUT &= ~BIT5
#define LED3_ON     P2OUT &= ~BIT0
#define LED4_ON     P2OUT &= ~BIT1
#define LED1_OFF     P1OUT |=  BIT4
#define LED2_OFF     P1OUT |=  BIT5
#define LED3_OFF     P2OUT |=  BIT0
#define LED4_OFF     P2OUT |=  BIT1
#define DATA        P1IN & 0x0f
#define PT2272_VT   P2IN & 0x80
#define NIXIE_A    P2OUT |= BIT5
#define NIXIE_B    P2OUT |= BIT4
#define NIXIE_C    P2OUT |= BIT6
#define NIXIE_D    P2OUT |= BIT2
#define NIXIE_E    P2OUT |= BIT3
#define NIXIE_F    P1OUT |= BIT7
#define NIXIE_G    P1OUT |= BIT6
#define NIXIE_CLEAR P1OUT &= 0x3f;P2OUT &= 0x83
#define NIXIE_0    P1OUT|=BIT7;P2OUT|=(BIT5+BIT4+BIT6+BIT2+BIT3)//NIXIE_A;NIXIE_B;NIXIE_C;NIXIE_D;NIXIE_E;NIXIE_F
#define NIXIE_1    P2OUT|=(BIT6+BIT4)//NIXIE_B;NIXIE_C
#define NIXIE_2    P1OUT|=BIT6;P2OUT|=(BIT5+BIT4+BIT2+BIT3)//NIXIE_A;NIXIE_B;NIXIE_G;NIXIE_E;NIXIE_D
#define NIXIE_3    P1OUT|=BIT6;P2OUT|=(BIT5+BIT4+BIT6+BIT2)//NIXIE_A;NIXIE_B;NIXIE_C;NIXIE_D;NIXIE_G
#define NIXIE_4    P1OUT|=(BIT6+BIT7);P2OUT|=(BIT4+BIT6)//NIXIE_B;NIXIE_C;NIXIE_F;NIXIE_G
extern unsigned char data_temp =  0;
/********************************************************
*名       称：Wireless_IO_Init()
*功       能：初始化本无线控制系统的IO口
*入口参数：无
*出口参数：无
*说       明：P1.0/1.1/1.2/1.3为输入
		    除P2.7均是输出
********************************************************/
void Wireless_IO_Init(void)
{
	P2SEL &= ~(BIT6+BIT7);
	P2SEL2&= ~(BIT6+BIT7);
	P1DIR = 0xf0;
	P2DIR = 0x7f;
	P1OUT = 0x30;
	P2OUT = 0x03;
	P2IES = 0x00;                          //选择P2.7口为上升沿中断
	P2IFG = 0x00;                           //清零P2.7口的的中断标志
	P2IE  = 0x80;                            //使能P2.7口中断功能
	_EINT();
}


/********************************************************
*名       称：Wireless_Packet_Data()
*功       能：解析收到的数据
*入口参数：无
*出口参数：无
*说       明：
********************************************************/
void Wireless_Packet_Data(void)
{
	if((data_temp&BIT0)&&(data_temp&BIT4))
	{
		_DINT();
		if(data_temp & BIT6)LED1_ON;
		else               LED1_OFF;
		if(data_temp & BIT5)LED2_ON;
		else               LED2_OFF;
		if(data_temp & BIT2)LED3_ON;
		else               LED3_OFF;
		if(data_temp & BIT1)
		{
			P2SEL&=~BIT1;
			LED4_ON;
		}
		else               LED4_OFF;
		_EINT();
	}
	if(((data_temp&BIT0)==0)&&((data_temp&BIT4)==0))
	{
		_DINT();
		if(data_temp & BIT6)
		{
			NIXIE_CLEAR;
			NIXIE_1;
			TA1_PWM_Init( 'S', 8 , 'F' , 0 );
			TA1_PWM_SetPeriod(1000);
			TA1_PWM_SetPermill(1,900);
		}
		if(data_temp & BIT5)
		{
			NIXIE_CLEAR;
			NIXIE_2;
			TA1_PWM_Init( 'S', 8 , 'F' , 0 );
			TA1_PWM_SetPeriod(1000);
			TA1_PWM_SetPermill(1,750);
		}
		if(data_temp & BIT2)
		{
			NIXIE_CLEAR;
			NIXIE_3;
			TA1_PWM_Init( 'S', 8 , 'F' , 0 );
			TA1_PWM_SetPeriod(1000);
			TA1_PWM_SetPermill(1,500);
		}
		if(data_temp & BIT1)
		{
			NIXIE_CLEAR;
			NIXIE_4;
			TA1_PWM_Init( 'S', 8 , 'F' , 0 );
			TA1_PWM_SetPeriod(1000);
			TA1_PWM_SetPermill(1,250);
		}
		if((data_temp&0x66)==0)
		{
			NIXIE_CLEAR;
			NIXIE_0;
			P2SEL&=~BIT1;
			LED4_OFF;
		}
		_EINT();
	}
}

/********************************************************
*名       称：Port_2()
*功       能：P2.7每个上升沿读取数据，将其存入全局变量以便解析
*入口参数：无
*出口参数：无
*说       明：
********************************************************/
#pragma vector = PORT2_VECTOR      //P2口中断程序
__interrupt void Port_2(void){
	unsigned char temp;
	_DINT();
	temp = DATA;
	if((temp & BIT3)==0)
	{
		data_temp &= 0xf0;
		data_temp |= temp;
	}
	if((temp & BIT3)==0x08)
	{
		data_temp &= 0x0f;
		temp <<= 4;
		data_temp |= temp;
	}
	P2IFG = 0x00;
	_EINT();
}
#endif
