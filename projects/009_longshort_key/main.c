#include <msp430g2553.h>

#define IDLE              0
#define SHORT             1
#define LONG              2
#define COUNTER_THRESHOLD 50
unsigned char WDT_counter = 0;
void GPIO_Init();
void WDT_Init();
void Key_Sm();
unsigned char LongClick_Dect();
void P13_OnShortRelease();
void P13_OnLongClick();

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	GPIO_Init();
	WDT_Init();
	_EINT();
	LPM3;
}

/********************************************************
*名       称：GPIO_Init()
*功       能：设定按键和LED控制IO的方向，启用按键IO的上拉电阻
*入口参数：无
*出口参数：无
*说       明：
********************************************************/
void GPIO_Init(void)
{
	P1DIR |= BIT0 + BIT6;
	P1OUT |= BIT0;
	P1OUT &=~BIT6;
	//---配合机械按键，启用内部上拉电阻---
	P1REN |= BIT3;
	P1OUT |= BIT3;
}

/********************************************************
*名       称：WDT_Init()
*功       能：设定WDT定时中断为16ms，开启WDT定时中断使能
*入口参数：无
*出口参数：无
*说       明：WDT定时中断的时钟源选择ACLK，可以用LPM3休眠
********************************************************/
void WDT_Init(void)
{
	WDTCTL = WDT_ADLY_16;
	IE1 |= WDTIE;//WDT中断使能
}

/********************************************************
*名       称：WDT_ISR()
*功       能：相应WDT定时中断服务
*入口参数：无
*出口参数：无
*说       明：不能直接判断事件，需启用状态机
********************************************************/
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void)
{
	Key_Sm();//启用按键状态机
}

/********************************************************
*名       称：Key_Sm()
*功       能：判断出长短键
*入口参数：无
*出口参数：无
*说       明：Mealy型状态机，在switch(state)中需要判断事件
********************************************************/
void Key_Sm(void)
{
	static unsigned char state = 0;   //状态机状态变量
	static unsigned char key_now = 0; //记录按键当前电平
	unsigned char key_past = 0;       //记录按键前一次电平
	unsigned char key_dect = 0;       //按键状态值
	key_past = key_now;
	//---查询IO的输入寄存器---
	if(P1IN & BIT3) key_now = 1;
	else            key_now = 0;
	//---电平前高后低表明按下---
	if(( key_past==1 )&&( key_now==0 ))
		key_dect = 1;
	//---电平前低后高表明弹起---
	if(( key_past==0 )&&( key_now==1 ))
		key_dect = 2;
	switch(state)  //该状态机靠扫描的按键值key_dect跳转状态
	{
		case IDLE : WDT_counter = 0;//空闲状态对计数清零
			if( key_dect==1 ) state = SHORT;  //路径1
			break;
		case SHORT: if( key_dect ==2 )        //路径2
					{
						state = IDLE;
						P13_OnShortRelease(); //短按事件处理函数
					}
					if( LongClick_Dect() )    //路径3
					{
						state = LONG;
						P13_OnLongClick();    //长按事件处理函数
					}break;
		case LONG : WDT_counter = 0;          //长按状态对计数清零
			if( key_dect ==2 )
				state = IDLE;break;           //路径4
		default   : state = IDLE; break;
	}
}

unsigned char LongClick_Dect()
{
	WDT_counter++;
	if( WDT_counter == COUNTER_THRESHOLD )
	{
		WDT_counter = 0;
		return(1);
	}
	else return (0);
}

void P13_OnShortRelease(void)
{
	P1OUT ^= BIT0;
}

void P13_OnLongClick(void)
{
	P1OUT ^= BIT6;
}






