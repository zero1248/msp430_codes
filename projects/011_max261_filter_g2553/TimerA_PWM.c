/*
 * TimerA_PWM.c
 *
 *  Created on: 2015-5-21
 *      Author: Isaac
 */

#include<msp430g2553.h>
#include"TimerA_PWM.h"


//---------------TA0---------------------
/********************************************************
*名       称：TA0_PWM_Init()
*功       能：TA0定时器作为PWM发生器的初始化函数
*入口参数：clk-时钟源'S'=SMCLK;'A'=ACLK;'E'=TACLK;'e'='E'取反
          div-时钟分频系数:1/2/4/8
          mode1-通道1的输出模式，'F'设为超前PWM(模式7),'B'滞后PWM(模式3),
          'D'带死区增PWM(模式6),0=禁用
*出口参数：1-成功,2-失败
*说       明：调用PWM相关函数之前，需调用此函数设置TA的模式和时钟源
********************************************************/
char TA0_PWM_Init( char clk , char div , char mode1 )
{
	TA0CTL = 0;                                //清除以前设置
	switch( clk )                              //选择定时器时钟源
	{
	case'A':case'a': TA0CTL |= TASSEL_1;break; //ACLK
	case'S':case's': TA0CTL |= TASSEL_2;break; //SMCLK
	case'E':         TA0CTL |= TASSEL_0;break; //外部输入(TACLK)
	case'e':         TA0CTL |= TASSEL_3;break; //外部输入(TACLK取反)
	default: return(0);
	}
	switch( div )                              //选择分频系数
	{
		case 1: TA0CTL |= ID_0;break;          //1
		case 2: TA0CTL |= ID_1;break;          //2
		case 4: TA0CTL |= ID_2;break;          //4
		case 8: TA0CTL |= ID_3;break;          //8
		default: return(0);
	}
	switch( mode1 )                             //选择计数模式
	{
		case'F':case'f':case'B':case'b': TA0CTL|=MC_1;break; //普通PWM，增计数
		case'D':case'd':                 TA0CTL|=MC_3;break; //死区PWM，增减计数
		default:return(0);
	}
	switch( mode1 )                             //设置PWM通道1的输出模式
	{
		case'F':case'f': TA0CCTL1 = OUTMOD_7;TA01_SET;break;
		case'B':case'b': TA0CCTL1 = OUTMOD_3;TA01_SET;break;
		case'D':case'd': TA0CCTL1 = OUTMOD_6;TA01_SET;break;
		case'0':case 0 : TA01_OFF;break;       //如果设置为禁用TA0.1恢复为普通IO
		default: return(0);
	}
	return (1);
}

/********************************************************
*名       称：TA0_PWM_SetPeriod()
*功       能：设置PWM发生器的周期
*入口参数：period-(0~65535)时钟个数
*出口参数：1-成功,2-失败
*说       明：普通PWM与带死区PWM周期相差一倍
********************************************************/
char TA0_PWM_SetPeriod(unsigned int period)
{
	if(period > 65535) return(0);
	TA0CCR0 = period;
	return(1);
}

/********************************************************
*名       称：TA0_PWM_SetPermill()
*功       能：设置PWM输出的占空比(占空比)
*入口参数：channel-当前设置的通道号,1/2
		   duty-PWM高电平有效时间的千分比(0~1000)
*出口参数：1-成功,0-失败
*说       明：1000=100%，500=50.0%。死区模式时，两channel同时设定
********************************************************/
char TA0_PWM_SetPermill(char channel,unsigned int duty)
{
	unsigned char mod =0;
	unsigned int deadpermill = 0;
	unsigned long int percent = 0;
	percent = duty;
	deadpermill = ( (DEADTIME*1000)/TACCR0 );
	switch( channel )
	{
		case 1 :
			mod = ( TA0CCTL1 & 0x00e0)>>5;break;
		case 2 :
			mod = ( TA0CCTL2 & 0x00e0)>>5;break;
		default : return(0);
	}
	switch( mod )
	{
		case 2:case 6:
		{
			if( (1000-2*percent)<=deadpermill)
				percent = (1000-deadpermill)/2;
			TA0CCR1 = percent * TA0CCR0/1000;
			TA0CCR2 = TA0CCR0 - TA0CCR1;
			break;
		}
		case 7:
		{
			if( percent > 1000 ) percent = 1000;
			if( channel == 1 )TA0CCR1 = percent * TA0CCR0/1000;
			if( channel == 2 )TA0CCR2 = percent * TA0CCR0/1000;
			break;
		}
		case 3:
		{
			if(percent > 1000) percent = 1000;
			if(channel==1) TA0CCR1 = TA0CCR0-percent*TA0CCR0/1000;
			if(channel==2) TA0CCR2 = TA0CCR0-percent*TA0CCR0/1000;
			break;
		}
		default: return(0);
	}
	return(1);
}

//---------------TA1---------------------
/********************************************************
*名       称：TA1_PWM_Init()
*功       能：TA1定时器作为PWM发生器的初始化函数
*入口参数：clk-时钟源'S'=SMCLK;'A'=ACLK;'E'=TACLK;'e'='E'取反
          div-时钟分频系数:1/2/4/8
          mode1-通道1的输出模式，'F'设为超前PWM(模式7),'B'滞后PWM(模式3),
          'D'带死区增PWM(模式6),0=禁用
*出口参数：1-成功,2-失败
*说       明：调用PWM相关函数之前，需调用此函数设置TA的模式和时钟源
********************************************************/
char TA1_PWM_Init( char clk , char div , char mode1 , char mode2 )
{
	TA1CTL = 0;                                //清除以前设置
	switch( clk )                              //选择定时器时钟源
	{
	case'A':case'a': TA1CTL |= TASSEL_1;break; //ACLK
	case'S':case's': TA1CTL |= TASSEL_2;break; //SMCLK
	case'E':         TA1CTL |= TASSEL_0;break; //外部输入(TACLK)
	case'e':         TA1CTL |= TASSEL_3;break; //外部输入(TACLK取反)
	default: return(0);
	}
	switch( div )                              //选择分频系数
	{
		case 1: TA1CTL |= ID_0;break;          //1
		case 2: TA1CTL |= ID_1;break;          //2
		case 4: TA1CTL |= ID_2;break;          //4
		case 8: TA1CTL |= ID_3;break;          //8
		default: return(0);
	}
	switch( mode1 )                             //选择计数模式
	{
		case'F':case'f':case'B':case'b': TA1CTL|=MC_1;break; //普通PWM，增计数
		case'D':case'd':                 TA1CTL|=MC_3;break; //死区PWM，增减计数
		default:return(0);
	}
	switch( mode1 )                             //设置PWM通道1的输出模式
	{
		case'F':case'f': TA1CCTL1 = OUTMOD_7;TA11_SET;break;
		case'B':case'b': TA1CCTL1 = OUTMOD_3;TA11_SET;break;
		case'D':case'd': TA1CCTL1 = OUTMOD_6;TA11_SET;break;
		case'0':case 0 : TA11_OFF;break;       //如果设置为禁用TA0.1恢复为普通IO
		default: return(0);
	}
	switch( mode2 )                             //设置PWM通道2的输出模式
	{
		case'F':case'f': TA1CCTL2 = OUTMOD_7;TA12_SET;break;
		case'B':case'b': TA1CCTL2 = OUTMOD_3;TA12_SET;break;
		case'D':case'd': TA1CCTL2 = OUTMOD_2;TA12_SET;break;
		case'0':case 0 : TA12_OFF;break;       //如果设置为禁用TA0.1恢复为普通IO
		default: return(0);
	}
	return (1);
}

/********************************************************
*名       称：TA1_PWM_SetPeriod()
*功       能：设置PWM发生器的周期
*入口参数：period-(0~65535)时钟个数
*出口参数：1-成功,2-失败
*说       明：普通PWM与带死区PWM周期相差一倍
********************************************************/
char TA1_PWM_SetPeriod(unsigned int period)
{
	if(period > 65535) return(0);
	TA1CCR0 = period;
	return(1);
}

/********************************************************
*名       称：TA1_PWM_SetPermill()
*功       能：设置PWM输出的占空比(占空比)
*入口参数：channel-当前设置的通道号,1/2
		   duty-PWM高电平有效时间的千分比(0~1000)
*出口参数：1-成功,0-失败
*说       明：1000=100%，500=50.0%。死区模式时，两channel同时设定
********************************************************/
char TA1_PWM_SetPermill(char channel,unsigned int duty)
{
	unsigned char mod =0;
	unsigned int deadpermill = 0;
	unsigned long int percent = 0;
	percent = duty;
	deadpermill = ( (DEADTIME*1000)/TACCR0 );
	switch( channel )
	{
		case 1 :
			mod = ( TA1CCTL1 & 0x00e0)>>5;break;
		case 2 :
			mod = ( TA1CCTL2 & 0x00e0)>>5;break;
		default : return(0);
	}
	switch( mod )
	{
		case 2:case 6:
		{
			if( (1000-2*percent)<=deadpermill)
				percent = (1000-deadpermill)/2;
			TA1CCR1 = percent * TA1CCR0/1000;
			TA1CCR2 = TA1CCR0 - TA1CCR1;
			break;
		}
		case 7:
		{
			if( percent > 1000 ) percent = 1000;
			if( channel == 1 )TA1CCR1 = percent * TA1CCR0/1000;
			if( channel == 2 )TA1CCR2 = percent * TA1CCR0/1000;
			break;
		}
		case 3:
		{
			if(percent > 1000) percent = 1000;
			if(channel==1) TA1CCR1 = TA1CCR0-percent*TA1CCR0/1000;
			if(channel==2) TA1CCR2 = TA1CCR0-percent*TA1CCR0/1000;
			break;
		}
		default: return(0);
	}
	return(1);
}




