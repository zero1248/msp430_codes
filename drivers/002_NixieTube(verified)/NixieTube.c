/*
                     MSP430FX1XX系列数码管显示通用驱动程序
  说明：该驱动程序库包含了常用的LCD显示功能,如显示数字、字母等
        可以作为各种程序的底层驱动使用。
        要使用该库函数，需要将本文件(LCD_Display.c)添加进工程，并在
        需要调用显示函数的文件开头处包含"NixieTube.h"*/
#include "NixieTube.h"
#include <msp430x16x.h>

#define a             0x01             //  AAAAAA
#define b             0x02             // F      B
#define c             0x04             // F      B
#define d             0x08             // GGGGGGG
#define e             0x10             // E      C
#define f             0x20             // E      C  ......
#define g             0x40             // DDDDDDD  ......
#define DOTSEG        0x80
#define NEGSEG        0x40       //DOTSEG是小数点 NEGSEG是负号
unsigned char Segment_Tab[] = {
  a + b + c + d + e + f,                        // Displays "0"
  b + c,                                        // Displays "1"
  a + b + d + e + g,                            // Displays "2"
  a + b + c + d + g,                            // Displays "3"
  b + c + f + g,                                // Displays "4"
  a + c + d + f +g,                             // Displays "5"
  a + c + d + e + f + g,                        // Displays "6"
  a + b + c,                                    // Displays "7"
  a + b + c + d + e + f + g,                    // Displays "8"
  a + b + c + d + f + g,                        // Displays "9"
  a + b + c + e + f + g,                        // Displays "A"
  c + d + e + f + g,                            // Displays "B"
  a + d + e + f,	                            // Displays "C"
  b + c + d + e + g,               	            // Displays "D"
  a + d + e + f + g,            	            // Displays "E"
  a + e + f + g,                                // Displays "F"
  g,                                            // Displays "-"
  b,                                            // Displays "'"
  0                                             // Displays " "
};
#undef a              //释放预定义
#undef b
#undef c
#undef d
#undef e
#undef f
#undef g

/****************************************************************************
* 名    称：Nixie_Delay()
* 功    能：用于控制数码管扫描速度
* 入口参数：无
* 出口参数：无
* 说    明: delay越小刷新越快
****************************************************************************/
void Nixie_Delay(void){      //普通延时保证不闪
    unsigned int delay;
    for ( delay = 1200 ; delay > 0 ; delay -- );
}

/****************************************************************************
* 名    称：Nixie_Display()
* 功    能：让某一位数码管显示一个数字或字母
* 入口参数：bit  要显示数码管的位置
                   segment  要显示的数字或字母
* 出口参数：无
* 说    明: 参数直接写成十进制即可，数码管的顺序从左往右显示
****************************************************************************/
void Nixie_Display(unsigned char bit , unsigned char segment){
    switch( segment ){
        case 0 :  NIXIE_DISPLAY0; break;
        case 1 :  NIXIE_DISPLAY1; break;
        case 2 :  NIXIE_DISPLAY2; break;
        case 3 :  NIXIE_DISPLAY3; break;
        case 4 :  NIXIE_DISPLAY4; break;
        case 5 :  NIXIE_DISPLAY5; break;
        case 6 :  NIXIE_DISPLAY6; break;
        case 7 :  NIXIE_DISPLAY7; break;
        case 8 :  NIXIE_DISPLAY8; break;
        case 9 :  NIXIE_DISPLAY9; break;
        case 10 : NIXIE_DISPLAYA;break;
        case 11 : NIXIE_DISPLAYB;break;
        case 12 : NIXIE_DISPLAYC;break;
        case 13 : NIXIE_DISPLAYD;break;
        case 14 : NIXIE_DISPLAYE;break;
        case 15 : NIXIE_DISPLAYF;break;
        default :break;
    }
    switch( bit ){
        case 0 :  BIT_SEL0; break;
        case 1 :  BIT_SEL1; break;
        case 2 :  BIT_SEL2; break;
        case 3 :  BIT_SEL3; break;
        case 4 :  BIT_SEL4; break;
        case 5 :  BIT_SEL5; break;
        case 6 :  BIT_SEL6; break;
        case 7 :  BIT_SEL7; break;
        default :break;
    }
    Nixie_Delay();
}

/****************************************************************************
* 名    称：Nixie_All_Delay()
* 功    能：一次向数码管写八个固定的数字或字母
* 入口参数：无
* 出口参数：无
* 说    明: 需在头文件中修改要显示的参数，有待改进
               改进方案：可通过以字符串输入 在函数中将字符串进行判断然后显示
****************************************************************************/
void Nixie_All_Display(void){
    NIXIE_DISPLAY1;  BIT_SEL0;  Nixie_Delay();
    NIXIE_DISPLAY2;  BIT_SEL1;  Nixie_Delay();
    NIXIE_DISPLAY3;  BIT_SEL2;  Nixie_Delay();
    NIXIE_DISPLAY4;  BIT_SEL3;  Nixie_Delay();
    NIXIE_DISPLAY5;  BIT_SEL4;  Nixie_Delay();
    NIXIE_DISPLAY6;  BIT_SEL5;  Nixie_Delay();
    NIXIE_DISPLAY7;  BIT_SEL6;  Nixie_Delay();
    NIXIE_DISPLAY8;  BIT_SEL7;  Nixie_Delay();
}
