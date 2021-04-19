#include <msp430.h>
#include "external_interrupt.h"

/****************************************************************************
* 名    称：P1_Interrupt_Init()
* 功    能：P1口外部中断初始化
* 入口参数：io_sel ，选择P1口作为外部中断触发IO的位
                   up_down ，选择触发方式：上升沿/下降沿
* 出口参数：无
* 说    明: 需开启总中断_EINT()才能最终使能外部中断
****************************************************************************/
void P1_Interrupt_Init(unsigned char io_sel , unsigned char up_down){
    P1DIR = ~( io_sel );
    P1SEL = ~( io_sel );
    if( up_down ) P1IES = ~( io_sel ); //上升沿中断
    else P1IES = io_sel; //下降沿中断
    P1IFG = ~( io_sel );   //中断标志复位
    P1IE  = io_sel; //开P1外部中断
}

/****************************************************************************
* 名    称：P2_Interrupt_Init()
* 功    能：P2口外部中断初始化
* 入口参数：io_sel ，选择P1口作为外部中断触发IO的位
                   up_down ，选择触发方式：上升沿/下降沿
* 出口参数：无
* 说    明: 需开启总中断_EINT()才能最终使能外部中断
****************************************************************************/
void P2_Interrupt_Init(unsigned char io_sel , unsigned char up_down){
    P2DIR = ~( io_sel );
    P2SEL = ~( io_sel );
    if( up_down ) P2IES = ~( io_sel ); //上升沿中断
    else P2IES = io_sel; //下降沿中断
    P2IFG = ~( io_sel );   //中断标志复位
    P2IE  = io_sel; //开P2外部中断
}

/****************************************************************************
* 名    称：P1IO_Detect()
* 功    能：P1口外部中断内的判断函数
* 入口参数：无
* 出口参数：无
* 说    明: 判断中断来源，进行相应处理函数的调用
****************************************************************************/
static void P1IO_Detect(void)
{
        unsigned int push_key = 0;
        push_key = P1IFG & (~P1DIR);//确保只记录一个IO
        __delay_cycles(10000);              //去抖
        if( (P1IN & push_key) == 0 )    //按键正常时P1IN为0，push_key为1,防止是按键松开的第一个抖动
        {
            switch( push_key )                //不看P1IN，防止多个键被按下
            {
                case BIT0 : P10_Onclick(); break;
                //case BIT1 : P11_Onclick(); break;
                //case BIT2 : P12_Onclick(); break;
                //case BIT3 : P13_Onclick(); break;
               // case BIT4 : P14_Onclick(); break;
               // case BIT5 : P15_Onclick(); break;
               // case BIT6 : P16_Onclick(); break;
                //case BIT7 : P17_Onclick(); break;
                default :                           break;
            }
        }
}

/****************************************************************************
* 名    称：P1IO_Detect()
* 功    能：P1口外部中断内的处理函数
* 入口参数：无
* 出口参数：无
* 说    明: 判断中断来源后，进行相应操作
****************************************************************************/
void P10_Onclick(void)
{

}

/****************************************************************************
* 名    称：P1_Intrpt()
* 功    能：P1口外部中断函数
* 入口参数：无
* 出口参数：无
* 说    明: 需复位P1IFG标志位
****************************************************************************/
 #pragma vector=PORT1_VECTOR   //外部中断计脉冲
__interrupt void P1_Intrpt(void)
{
        P1IO_Detect();
        P1IFG = 0;
}

/****************************************************************************
* 名    称：P2IO_Detect()
* 功    能：P2口外部中断内的判断函数
* 入口参数：无
* 出口参数：无
* 说    明: 判断中断来源，进行相应处理函数的调用
****************************************************************************/
static void P2IO_Detect(void)
{
        unsigned int push_key = 0;
        push_key = P2IFG & (~P2DIR);//确保只记录一个IO
        __delay_cycles(10000);              //去抖
        if( (P2IN & push_key) == 0 )    //按键正常时P1IN为0，push_key为1,防止是按键松开的第一个抖动
        {
            switch( push_key )                //不看P1IN，防止多个键被按下
            {
                case BIT0 : P20_Onclick(); break;
                //case BIT1 : P21_Onclick(); break;
                //case BIT2 : P22_Onclick(); break;
                //case BIT3 : P23_Onclick(); break;
               // case BIT4 : P24_Onclick(); break;
               // case BIT5 : P25_Onclick(); break;
               // case BIT6 : P26_Onclick(); break;
                //case BIT7 : P27_Onclick(); break;
                default :                           break;
            }
        }
}

/****************************************************************************
* 名    称：P2IO_Detect()
* 功    能：P2口外部中断内的处理函数
* 入口参数：无
* 出口参数：无
* 说    明: 判断中断来源后，进行相应操作
****************************************************************************/
void P20_Onclick(void)
{

}

/****************************************************************************
* 名    称：P2_Intrpt()
* 功    能：P2口外部中断函数
* 入口参数：无
* 出口参数：无
* 说    明: 需复位P2IFG标志位
****************************************************************************/
 #pragma vector=PORT2_VECTOR   //外部中断计脉冲
__interrupt void P2_Intrpt(void)
{
    P2IO_Detect();
    P2IFG = 0;
}
