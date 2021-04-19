#include"TimerA.h"

/****************************************************************************
* 名    称：TimerA_init()
* 功    能：初始化TimerA
* 入口参数：无
* 出口参数：无
****************************************************************************/
void TimerA_init(void){
    TACTL = MC_0 + TACLR;//暂停定时器 并清零计数器
	TACTL = TASSEL_2+TACLR+ID_3+MC_1; //设置计时器A使用子系统时钟
	//TA0CCTL0 = CCIE + CM_1 + SCS + CAP+CCIS_0;     //对应CCR1使能，上升沿同步捕获P1.1输入
	CCR0 = CCR0_VALUE;
	CCTL0 |= CCIE;//设置计时器中断使能    8分频 up模式
}

/****************************************************************************
* 名    称：TA_capture0_init()
* 功    能：初始化TimerA_Capture0
* 入口参数：无
* 出口参数：无
****************************************************************************/
void TA_capture0_init( void ){
    P1SEL |= BIT1;
    P1DIR &= ~BIT1;
    P1IN &= ~BIT1;
    TACTL |= MC_0 + TACLR;//定时器暂停，计数器清零
    TACTL |= TASSEL_2 + ID_0 + MC_1  ; //子系统时钟+不分频
                                                                     //上升计数（上升到CCR0）
    TACCTL0 |= CAP + SCS + CCIS_0 + CM_1; //捕获模式+同步捕获+
                                                                   //捕获输入选择P1.1+捕获边沿选择
    TACCTL0 |= CCIE; //捕获中断使能
}

/****************************************************************************
* 名    称：TA_capture1_init()
* 功    能：初始化TimerA_Capture1
* 入口参数：无
* 出口参数：无
****************************************************************************/
void TA_capture1_init( void ){
    P1SEL |= BIT2;
    P1DIR &= ~BIT2;
    P1IN &= ~BIT2;
    TACTL |= MC_0 + TACLR;//定时器暂停，计数器清零
    TACTL |= TASSEL_2 + ID_0 + MC_1  ; //子系统时钟+不分频
                                                                     //上升计数（上升到CCR0）
    TACCTL1 |= CAP + SCS + CCIS_0 + CM_1; //捕获模式+同步捕获+
                                                                   //捕获输入选择P1.2+捕获边沿选择
    TACCTL1 |= CCIE; //捕获中断使能
}

/****************************************************************************
* 名    称：TA_capture2_init()
* 功    能：初始化TimerA_Capture2
* 入口参数：无
* 出口参数：无
****************************************************************************/
void TA_capture2_init( void ){
    P1SEL |= BIT3;
    P1DIR &= ~BIT3;
    P1IN &= ~BIT3;
    TACTL |= MC_0 + TACLR;//定时器暂停，计数器清零
    TACTL |= TASSEL_2 + ID_0 + MC_1  ; //子系统时钟+不分频
                                                                     //上升计数（上升到CCR0）
    TACCTL2 |= CAP + SCS + CCIS_0 + CM_1; //捕获模式+同步捕获+
                                                                   //捕获输入选择P1.3+捕获边沿选择
    TACCTL2 |= CCIE; //捕获中断使能
}

/****************************************************************************
* 名    称：TA_PWM_mode1()
* 功    能：初始化PWM发生模式1
* 入口参数：无
* 出口参数：无
* 说明 : 低电平单脉冲
****************************************************************************/
void TA_PWM_mode1(void){
    P1SEL &=  BIT7 ;  //P1.6 1.7 分别是TA的OUT1和OUT2
    P1DIR |= BIT7;
    TACTL |= MC_2 + TASSEL_2 + ID_0 + TACLR; //定时器TA用ACLK，连续计数模式
    TACCTL2 &= ~OUT;     //TA2输出设为低电平
    TACCR2 = PWM_MODE1_CCR2;  //TA2低电平持续CCR2个ACLK周期后自动变高
    TACCTL2 = OUTMOD_1;  //设为模式1，延迟置位
}

/****************************************************************************
* 名    称：TA_PWM_mode2_6()
* 功    能：初始化PWM发生模式2/6
* 入口参数：无
* 出口参数：无
* 说明 : 带死区的双路PWM
****************************************************************************/
void TA_PWM_mode2_6(void){
    P1SEL &= BIT6 + BIT7 ;  //P1.6 1.7 分别是TA的OUT1和OUT2
    P1DIR |= BIT6 + BIT7;
    TACTL |= MC_3 + TASSEL_2 + ID_0 + TACLR; //定时器TA用ACLK，增减计数模式
    TACCTL1 = OUTMOD_6;  //TA1设为模式6(高电平死区PWM输出)
    TACCTL2 = OUTMOD_2;  //TA2设为模式2(低电平死区PWM输出)
    TACCR0 = PWM_MODE2_CCR0;  //PWM周期=CCR0个ACLK周期
    TACCR1 = PWM_MODE2_CCR1;  //第一路高电平持续CCR1个ACLK周期
    TACCR2 = PWM_MODE2_CCR2;  //第二路高电平持续CCR2个ACLK周期
                        //死区时间=20000个ACLK周期
}

/****************************************************************************
* 名    称：TA_PWM_mode3()
* 功    能：初始化PWM发生模式3
* 入口参数：无
* 出口参数：无
* 说明 : 低电平PWM
****************************************************************************/
void TA_PWM_mode3(void){
    P1SEL &= BIT7 ;  //P1.6 1.7 分别是TA的OUT1和OUT2
    P1DIR |= BIT7;
    TACTL |= MC_1 + TASSEL_2 + ID_0; //定时器TA设为增量计数模式,ACLK
    TACCTL2 = OUTMOD_3;  //模式3= 低电平PWM输出
    TACCR0 = PWM_MODE3_CCR0;   //PWM总周期 =CCR0个ACLK周期
    TACCR2 = PWM_MODE3_CCR2;   //低电平时间= CCR2个ACLK周期
}

/****************************************************************************
* 名    称：TA_PWM_mode4()
* 功    能：初始化PWM发生模式4
* 入口参数：无
* 出口参数：无
* 说明 : 可变频率输出
****************************************************************************/
void TA_PWM_mode4(void){
    P1SEL &= BIT7 ;  //P1.6 1.7 分别是TA的OUT1和OUT2
    P1DIR |= BIT7;
    TACTL |= MC_1 + TASSEL_2 + ID_0; //定时器TA设为增量计数模式,ACLK
    TACCTL2 = OUTMOD_4; //模式4= 可变频率输出
    TACCR0 = PWM_MODE4_CCR0;       //输出方波周期 =CCR0个ACLK周期
}

/****************************************************************************
* 名    称：TA_PWM_mode5()
* 功    能：初始化PWM发生模式5
* 入口参数：无
* 出口参数：无
* 说明 : 高电平单脉冲
****************************************************************************/
void TA_PWM_mode5(void){
    P1SEL &= BIT7 ;  //P1.6 1.7 分别是TA的OUT1和OUT2
    P1DIR |= BIT7;
    TACTL |= MC_2 + TASSEL_2 + ID_0 + TACLR; //定时器TA用ACLK，连续计数模式
    TACCTL2 |= OUT;      //TA2输出设为高电平
    TACCR2 = PWM_MODE5_CCR2;        //TA2高电平持续CCR2个ACLK周期后自动变低
    TACCTL2 = OUTMOD_5;  //设为模式5，延迟清零
}

/****************************************************************************
* 名    称：TA_PWM_mode7()
* 功    能：初始化PWM发生模式7
* 入口参数：无
* 出口参数：无
* 说明 : 高电平PWM
****************************************************************************/
void TA_PWM_mode7(void){
    P1SEL &= BIT6 + BIT7 ;  //P1.6 1.7 分别是TA的OUT1和OUT2
    P1DIR |= BIT6 +BIT7;
    TACTL |= MC_1 + TASSEL_2 + ID_0; //定时器TA设为增量计数模式,ACLK
    TACCTL2 = OUTMOD_7; //模式7= 高电平PWM输出
    TACCR0 = PWM_MODE7_CCR0;      //PWM总周期 =10000个ACLK周期
    TACCR2 = PWM_MODE7_CCR2;       //高电平时间= 1000个ACLK周期
}

// Timer A0 interrupt service routine
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A0 (void){
	//中断函数内容

	/*捕获模式
	两次读取CCRx并作差（第二次的要加上65535*溢出次数）；
	第二次后TACCTLx &= ~COV;
	*/
}

