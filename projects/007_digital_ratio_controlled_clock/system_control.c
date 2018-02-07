/*
 * system_control.c
 *
 *  Created on: 2015-7-2
 *      Author: Isaac
 */
#include"system_control.h"
#include<msp430g2553.h>
#include"OLED.h"
#include"I2C.h"
#include"codetab.h"
#include"data_transform.h"
#include"key.h"

//----转换后数据（只声明）----
//unsigned int  Simu_Data[7];  //年月日时分秒
//unsigned char Trans_Data[20];//整理后的数据
//char Display_Data[11];   //存放年月日显示数据
//char Display_Data2[9];   //存放时分秒显示数据
//----接受数据----
unsigned char count_flag = 0;// 0表示未处在计时状态，1表示处在计时状态
unsigned char rec_foot = 0;
unsigned char intsec_flag = 0;//整0、20、40秒标识
unsigned char achieve_flag = 0;//接收完成标识
unsigned char Receive_Data[20] = {0}; //接收到时钟数据

/********************************************************
*名        称：TA_Capture0_Init()
*功        能：捕获模式初始化并使能捕获中断
*入口参数 ：无
*出口参数 ：无
*说        明：设置WDT为Timer模式，需要在main函数中打开总中断
********************************************************/
void TA_Capture0_Init(void)
{
    P1SEL |= BIT1;
    P1DIR &= ~BIT1;
    P1IN &= ~BIT1;
    TACTL |= MC_0 + TACLR;    //定时器暂停，计数器清零
    TACTL |= TASSEL_1 + ID_0 + MC_1  ; //ACLK+不分频+上升计数
    TACCTL0 |= CAP + SCS + CCIS_0 + CM_3; //捕获模式+同步捕获+
                                //捕获输入选择P1.1+捕获边沿选择两种边沿
    //TACCTL0 |= CCIE;          //捕获中断使能
}

/********************************************************
*名        称：Simu_Clock_Open()
*功        能：模拟时钟初始化
*入口参数 ：无
*出口参数 ：无
*说        明：设置WDT为Timer模式，需要在main函数中打开总中断
********************************************************/
void Simu_Clock_Open(void)
{
	WDTCTL = WDT_ADLY_1000;//WDT timer精确定时1000ms
	IE1 = WDTIE;//开中断
}

/********************************************************
*名        称：Simu_Clock_Close()
*功        能：模拟时钟关闭
*入口参数 ：无
*出口参数 ：无
*说        明：设置WDT为Timer模式，需要在main函数中打开总中断
********************************************************/
void Simu_Clock_Close(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	IE1 &= ~WDTIE;//关中断
}

/********************************************************************
* 名       称：Display_Normal()
* 功       能：显示变化部分
* 入口参数：无
* 出口参数：无
* 说      明 : 无
********************************************************************/
void Display_Normal(void)
{
	//TACCTL0 &= ~CCIE; //捕获中断关闭,已在中断中关闭
	//Simu_Clock_Open();
	Display_TransData();
	OLED_Display_8x16(0,0,Display_Data);   //显示年月日
	OLED_Display_8x16(0,2,Display_Data);   //显示时分秒
}

/********************************************************
*名        称：Get_Data()
*功        能：模拟时钟关闭
*入口参数 ：无
*出口参数 ：无
*说        明：设置WDT为Timer模式，需要在main函数中打开总中断
********************************************************/
void Get_Data(void)
{
	Simu_Clock_Close();
	OLED_Reset();               //清屏
	OLED_Display_16x16(4,0,8);       //校准中
	OLED_Display_16x16(5,0,9);
	OLED_Display_16x16(6,0,10);
	TACCTL0 |= CCIE;         //开始获取信号
	_EINT();
	while(!(achieve_flag));
	achieve_flag = 0;
	Transform_Data();
	Display_Normal();
}

/********************************************************************
* 名    称：Display_Fixed()
* 功    能：显示固定部分
* 入口参数：无
* 出口参数：无
* 说明 : 无
********************************************************************/
void Display_Fixed(void)
{
	OLED_Reset();//清屏
//	OLED_Display_16x16(0,0,1);   //数字式电波时钟
//	OLED_Display_16x16(16,0,2);
//	OLED_Display_16x16(32,0,3);
//	OLED_Display_16x16(48,0,4);
//	OLED_Display_16x16(64,0,5);
//	OLED_Display_16x16(80,0,6);
//	OLED_Display_16x16(96,0,7);
}

/********************************************************
*名        称：CapInterrupt_ISR()
*功        能：TimerA捕获中断服务函数
*入口参数 ：无
*出口参数 ：无
*说        明：识别接收到的数据，出口参数直接写入数组
********************************************************/
static void CapInterrupt_ISR(void)
{
	unsigned int CCR0_count = 0;
	if( (CCI == 0) && (count_flag == 0) )
	{
		CCR0 = 0;
	    count_flag = 1;
	}
	else if( (CCI == 1) && (count_flag == 1) )
	{
		CCR0_count = CCR0;
		CCR0 = 0;
		count_flag = 2;
		if( (CCR0_count>3250) && (CCR0_count<3290) )
		{
			Receive_Data[rec_foot++] = 0;
		}
		else if( (CCR0_count>6530) && (CCR0_count<6580) )
		{
			Receive_Data[rec_foot++]= 1;
		}
		else if( (CCR0_count>9810) && (CCR0_count<9850) )
		{
			Receive_Data[rec_foot++] = 2;
		}
		else if( (CCR0_count>13080) && (CCR0_count<13130) )
		{
			Receive_Data[rec_foot++] = 3;
		}
		if( rec_foot == 21 )
		{
			TACCTL0 &= ~CCIE;
			count_flag = 0;
			rec_foot = 0;
			achieve_flag = 1;
		}
	}
	else if( (CCI == 0) && (count_flag == 2) )
	{
		CCR0_count = CCR0;
		CCR0 = 0;
		count_flag = 1;
		if( CCR0_count > 32768 )
		{
			intsec_flag = rec_foot;
			Receive_Data[rec_foot++] = 4;
			if( rec_foot == 21 )
			{
				TACCTL0 &= ~CCIE;
				count_flag = 0;
				rec_foot = 0;
				achieve_flag = 1;
			}
		}
	}
}

/********************************************************
*名        称：WDTInterrupt_ISR()
*功        能：看门狗定时器中断服务函数
*入口参数 ：无
*出口参数 ：无
*说        明：每一秒进一次中断，计时数组相应位置加一秒，并且保证
********************************************************/
static void WDTInterrupt_ISR(void)
{
	Simu_Data[5]++;
	if(Simu_Data[5] == 60)
	{
		Simu_Data[5] = 0;
		Simu_Data[4]++;
		if(Simu_Data[4] == 60)
		{
			Simu_Data[4] = 0;
			Simu_Data[3]++;
			if(Simu_Data[3] == 24)
			{
				Simu_Data[3] = 0;
				Simu_Data[2]++;
			}
		}
	}
}

/********************************************************
*名        称：WDT_Timer_interrupt()
*功        能：看门狗定时器中断
*入口参数 ：无
*出口参数 ：无
*说        明：1s进一次中断，中断中模拟时钟+1s
********************************************************/
#pragma vector=WDT_VECTOR
__interrupt void WDT_Timer_interrupt(void)
{
	//----无需手动清除标志位----
	WDTInterrupt_ISR();
	Display_Normal();
}

/********************************************************
*名        称：TimerA0_Capture_interrupt()
*功        能：TimerA捕获中断
*入口参数 ：无
*出口参数 ：无
*说        明：上升沿或下降沿都进入中断，调用中断服务函数
********************************************************/
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TimerA0_Capture_interrupt(void)
{
	CapInterrupt_ISR();
}
