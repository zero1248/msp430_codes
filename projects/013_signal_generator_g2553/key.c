#include "key.h"
#include<msp430g2553.h>

static unsigned char KeyBuff[ KEYBUFF_SIZE ]; //定义键盘缓冲队列数组(FIFO)
unsigned char Key_IndexW = 0;          //键盘缓冲队列写入指针(头指针)
unsigned char Key_IndexR = 0;          //键盘缓冲队列读取指针(尾指针)
unsigned char Key_Count  = 0;           //键盘缓冲队列内记录的按键次数

unsigned char P_KEY1 = 255;            //存放KEY1前一次状态的变量
unsigned char N_KEY1 = 255;            //存放KEY1当前状态的变量
unsigned char P_KEY2 = 255;            //存放KEY2前一次状态的变量
unsigned char N_KEY2 = 255;            //存放KEY2当前状态的变量
unsigned char P_KEY3 = 255;            //存放KEY3前一次状态的变量
unsigned char N_KEY3 = 255;            //存放KEY3当前状态的变量
unsigned char P_KEY4 = 255;            //存放KEY4前一次状态的变量
unsigned char N_KEY4 = 255;            //存放KEY4当前状态的变量

void TimerA_For_Key_Init(void){
    TACTL = MC_0 + TACLR;//暂停定时器 并清零计数器
	TACTL = TASSEL_2+TACLR+ID_3+MC_1; //设置计时器A使用子系统时钟
	CCR0 = TIMERA_CCR0;
	CCTL0 |= CCIE;//设置计时器中断使能    8分频 up模式
}

/****************************************************************************
* 名    称：Key_Init()
* 功    能：初始化键盘所在的IO口
* 入口参数：无
* 出口参数：无
****************************************************************************/
void Key_Init(void){
    KEY_INIT;
}

/****************************************************************************
* 名    称：Key_ScanIO()
* 功    能：扫描键盘IO口并判断按键事件
* 入口参数：无
* 出口参数：无，键值压入缓冲队列
* 说    明: 该函数需要每隔1/16秒至1/128秒调用一次。最好放在定时中断内执行。
            如果中断间隔太长，可能丢键;间隔太短不能消除抖动。
****************************************************************************/
void Key_ScanIO(){
    P_KEY1 = N_KEY1;  //保存KEY1前一次的状态
    N_KEY1=KEY1_IN; //读取KEY1当前的状态

    P_KEY2 = N_KEY2;  //保存KEY2前一次的状态
    N_KEY2 = KEY2_IN; //读取KEY2当前的状态

    P_KEY3 = N_KEY3;  //保存KEY3前一次的状态
    N_KEY3 = KEY3_IN; //读取KEY3当前的状态

    P_KEY4 = N_KEY4;  //保存KEY4前一次的状态
    N_KEY4 = KEY4_IN; //读取KEY4当前的状态

    if( ( P_KEY1 == 0 ) && ( N_KEY1 == 0 ) ) Key_InBuff( PUSH_KEY1 );
    if( ( P_KEY2 == 0 ) && ( N_KEY2 == 0 ) ) Key_InBuff( PUSH_KEY2 );
    if( ( P_KEY3 == 0 ) && ( N_KEY3 == 0 ) ) Key_InBuff( PUSH_KEY3 );
    if( ( P_KEY4 == 0 ) && ( N_KEY4 == 0 ) ) Key_InBuff( PUSH_KEY4 );
}

/****************************************************************************
* 名    称：Key_InBuff()
* 功    能：将一次键值压入键盘缓冲队列
* 入口参数：Key:被压入缓冲队列的键值
* 出口参数：无
****************************************************************************/
void Key_InBuff(unsigned char Key){
    if( Key_Count >= KEYBUFF_SIZE ) return;//若缓冲区已满，放弃本次按键
    _DINT();
    Key_Count++;                       //按键次数计数增加
    KeyBuff[Key_IndexW] = Key;         //从队列头部追加新的数据
    if (++Key_IndexW >=  KEYBUFF_SIZE){//循环队列，如果队列头指针越界
        Key_IndexW = 0;                  //队列头指针回到数组起始位置
    }
    _EINT();
}
/****************************************************************************
* 名    称：Key_GetKey()
* 功    能：从键盘缓冲队列内读取一次键值
* 入口参数：无
* 出口参数：若无按键，返回0，否则返回一次按键键值。
* 说    明: 调用一次该函数，会自动删除缓冲队列里一次按键键值。
****************************************************************************/
unsigned char Key_GetKey(){
    unsigned char Key;
    if(Key_Count==0)  return(0);     //若无按键，返回0
    _DINT();
    Key_Count--;                     //按键次数计数减1
    Key=KeyBuff[Key_IndexR];         //从缓冲区尾部读取一个按键值
    if (++Key_IndexR >= KEYBUFF_SIZE){//循环队列，如果队列尾指针越界
        Key_IndexR = 0;             //队列尾指针回到数组起始位置
    }
    _EINT();
    return( Key );
}
/****************************************************************************
* 名    称：Key_WaitKey()
* 功    能：从键盘缓冲队列内读取一次键值
* 入口参数：无
* 出口参数：若有按键，返回键值，否则等待按键。
* 说    明: 该函数会阻塞CPU继续执行后面的程序，应用时需注意。
****************************************************************************/
unsigned char Key_WaitKey(){
    unsigned char Key;
    while(1){
        Key=Key_GetKey();    //从键盘缓冲队列读取一次键值
        if(Key==0)  LPM3;    //如果没按键，则停止CPU，等待被唤醒继续读按键
        else   return(Key);  //如果有按键，则返回键值
    }
}

/****************************************************************************
* 名    称：Delay_For_Key()
* 功    能：为while函数中的键值读取后的操作进行延时
* 入口参数：无
* 出口参数：无
* 说    明: 键盘扫描函数每隔1/16秒至1/128秒调用一次，因此若while函数中扫
描太快则按键几乎无效，因此while中需延时
****************************************************************************/
void Delay_For_Key(void){
	unsigned int delay_key;
	for( delay_key=20000 ; delay_key>0 ; delay_key-- );
}

/****************************************************************************
* 名    称：Key_Timer_A0 ()
* 功    能：TimerA为按键扫描计时
* 入口参数 : 无
* 出口参数：无
* 说    明: 该函数只调用键盘扫描函数
****************************************************************************/
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Key_Timer_A0 (void){
	Key_ScanIO();
}

