/******************************************************************************
Copyright :  2014-2020
File name:    12864.h
Author:        Isaac
Version:       1.0.0.2
Date:            2015-03-24
Description: LCD12864的各种驱动函数
Others:        无
******************************************************************************/
#ifndef __12864_H__
#define __12864_H__


#define uchar unsigned char
#define uint unsigned int

void delayms(uint);   //延时xms
void write_cmd(uchar);   ////写指令到12864
void write_dat(uchar);	   //写数据到12864
void lcd_pos(uchar , uchar );	//显示地址
//void lcd_printf(uchar , uchar , uchar *); //向（x，y）处打印数据
void lcd_init(void);		 //LCD初始化
#endif
