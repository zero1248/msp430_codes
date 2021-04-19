/*
                       带有缓冲区的键盘通用程序库
        说明：该程序库包含了读取键盘的相关函数，并且包含了一个键盘缓冲区，当
        主程序执行较慢，而遇到连续快速的键盘输入来不及及时读取的情况下，
        新的按键信息将会自动存入键盘缓冲队列内，每次调用读键盘函数时会
        依顺序读取,保证键盘操作不会丢失。该键盘程序可以作为各种程序的底
        层驱动使用。
            要使用该库函数，需要将本文件(Key.c)添加进工程，并在需要调用
        键盘函数的文件开头处包含"Key.h";还需要开启一个定时器，

        在定时中断内调用Key_ScanIO()函数。

        设置定时中断的频率在16~128次/秒之间。*/
#ifndef __KEY_H__
#define __KEY_H__
 void Key_Init(void);       //在主函数中调用即可
 void Key_InBuff(unsigned char Key);
 unsigned char Key_GetKey(void);    //在主函数中调用即可
 unsigned char Key_WaitKey(void);
 void Key_ScanIO(void);
 void TimerA_For_Key_Init(void);    //在主函数中调用即可
 void Delay_For_Key(void);     //在主函数while(1)中调用即可

#define KEY1_IN  ( P1IN & BIT5 ) //KEY1输入IO的定义(P1.5)
#define KEY2_IN  ( P2IN & BIT0 ) //KEY2输入IO的定义(P2.0)
#define KEY3_IN  ( P2IN & BIT1 ) //KEY3输入IO的定义(P2.1)
#define KEY4_IN  ( P2IN & BIT2 ) //KEY3输入IO的定义(P2.2)

#define PUSH_KEY1 0x01
#define PUSH_KEY2 0x02         /*键值宏定义*/
#define PUSH_KEY3 0x04
#define PUSH_KEY4 0x08

#define TIMERA_CCR0 10000
#define KEYBUFF_SIZE   64    /*键盘缓冲区大小,根据程序需要自行调整*/
#define KEY_INIT  P1DIR &= ~(BIT5);P2DIR &= ~(BIT0+BIT1+BIT2)

#endif
