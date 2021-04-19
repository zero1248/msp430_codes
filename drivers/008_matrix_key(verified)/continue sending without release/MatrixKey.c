#include"MatrixKey.h"
#include<msp430x16x.h>
#include"my_430.h"

const unsigned char Mat_LineScan[4]={
    0xEF , 0xDF , 0xBF , 0x7F };//列线扫描电压，分为第1，2，3，4根列线
char MatKey_Buff[ MAT_KEYBUFF_SIZE ]; //定义键盘缓冲队列数组(FIFO)
char MatKey_IndexW = 0;          //键盘缓冲队列写入指针(头指针)
char MatKey_IndexR = 0;          //键盘缓冲队列读取指针(尾指针)
char MatKey_Count = 0;           //键盘缓冲队列内记录的按键次数

/****************************************************************************
* 名    称：Mat_Scan_Delay()
* 功    能：为键盘的扫描进行一定的（非精确）延时
* 入口参数：xms 大概毫秒数
* 入口参数：无
****************************************************************************/
void Mat_Scan_Delay(unsigned int xms){
	unsigned int i,j;
	for(i=xms;i>0;i--)
		for(j=880;j>0;j--);
}

/****************************************************************************
* 名    称：P1_For_Scan_Init()
* 功    能：键盘扫描I/O口初始化
* 入口参数：xms 大概毫秒数
* 入口参数：无
* 说明 : 低四位设为输入 高四位设为输出
****************************************************************************/
void P1_For_Scan_Init(void){
    MAT_SCAN_IO = 0xf0;
}

/****************************************************************************
* 名    称：TimerA_For_Scan_Init()
* 功    能：键盘扫描TimerA初始化
* 入口参数：无
* 入口参数：无
* 说明 : CCR0计时数太小不能消抖动，太大容易丢键
****************************************************************************/
void TimerA_For_Scan_Init(void){
	TACTL = MC_0 + TACLR;//暂停定时器 并太少容易丢清零计数器
	TACTL = TASSEL_2+TACLR+ID_3+MC_1; //设置计时器A使用子系统时钟
	CCR0 = 10000;
	CCTL0 |= CCIE;//设置计时器中断使能    8分频 up模式
}

/****************************************************************************
* 名    称：MatKey_InBuff()
* 功    能：将一次键值压入键盘缓冲队列
* 入口参数：Key:被压入缓冲队列的键值
* 出口参数：无
****************************************************************************/
void MatKey_InBuff(unsigned char Key){
    if( MatKey_Count >= MAT_KEYBUFF_SIZE ) return;//若缓冲区已满，放弃本次按键
    MatKey_Count++;                       //按键次数计数增加
    MatKey_Buff[ MatKey_IndexW ] = Key;         //从队列头部追加新的数据
    if ( ++MatKey_IndexW >=  MAT_KEYBUFF_SIZE ){//循环队列，如果队列头指针越界
        MatKey_IndexW = 0;                  //队列头指针回到数组起始位置
    }
}

/****************************************************************************
* 名    称：MatKey_ScanIO()
* 功    能：对键盘对应I/O口进行扫描
* 入口参数：无
* 出口参数：无
****************************************************************************/
void MatKey_ScanIO(void){
    unsigned char mat_temp = 0;		//扫描状态暂存。
    unsigned char mat_row = 0 , mat_line = 0;	//行号，列号。
    _DINT();
    for( mat_line = 0 ; mat_line < 4 ; mat_line++ ){     //列扫描
        MAT_SCAN_OUT = Mat_LineScan[ mat_line ];//输出扫描电位。
        mat_temp = MAT_SCAN_IN & 0x0f;		//输入扫描电位，并屏蔽高4位。
        if( mat_temp != 0x0f ){		//判断该列是否有按键按下。
            switch( mat_temp ){             //如果有，则判断行号。
                case 0x0e : mat_row = 0; break;
                case 0x0d : mat_row = 4; break;
                case 0x0b : mat_row = 8; break;
                case 0x07 : mat_row = 12; break;
                default:      break;
            }
            break;
        }
    }
    if( ( MAT_SCAN_IN & 0x0f ) != 0x0f )
    	MatKey_InBuff( mat_row + mat_line + 1 );
    MAT_SCAN_OUT = 0x0f;	  		//恢复P1口
    _EINT();
}

/****************************************************************************
* 名    称：MatKey_GetKey()
* 功    能：从键盘缓冲队列内读取一次键值
* 入口参数：无
* 出口参数：若无按键，返回0，否则返回一次按键键值。
* 说    明: 调用一次该函数，会自动删除缓冲队列里一次按键键值。
****************************************************************************/
unsigned char MatKey_GetKey(void){
    unsigned char key_key;
    if(MatKey_Count==0)  return(0);     //若无按键，返回0
    _DINT();
    MatKey_Count--;                     //按键次数计数减1
    key_key=MatKey_Buff[ MatKey_IndexR ];         //从缓冲区尾部读取一个按键值
    if ( ++MatKey_IndexR >= MAT_KEYBUFF_SIZE ){//循环队列，如果队列尾指针越界
        MatKey_IndexR = 0;             //队列尾指针回到数组起始位置
    }
    _EINT();
    return( key_key );
}

/*void main(void){
    unsigned char key_value ;
	WDT_OUT;
	IO_INIT;
	Clk_Init();
	P1_For_Scan_Init();
	TimerA_For_Scan_Init();
	_EINT();
    while(1){
        key_value = MatKey_GetKey();
		switch(key_value){
		case 1 : P3OUT = 0x01; break;
		case 2 : P3OUT = 0x02; break;
		case 3 : P3OUT = 0x04; break;
		case 4 : P3OUT = 0x08; break;
		case 5 : P3OUT = 0x10; break;
		case 6 : P3OUT = 0x20; break;
		case 7 : P3OUT = 0x40; break;
		case 8 : P3OUT = 0x80; break;
		default : P3OUT = 0x00;break;
		}
		Mat_Scan_Delay(1);
    }
}*/

/*******************************************
* 函数名称：MatKey_Timer_A()
* 功    能：按键计数及清除
* 参    数：无
* 返回值：无
* 说明：调用键盘扫描函数即可
********************************************/
#pragma vector = TIMERA0_VECTOR
__interrupt void MatKey_Timer_A(void){
   MatKey_ScanIO();
}
