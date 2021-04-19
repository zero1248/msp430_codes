/*
 * I2C.c
 *
 *  Created on: 2015-4-27
 *      Author: Isaac
 */
#include"I2C.h"
#include<msp430g2553.h>

#ifdef HARD_I2C

#define TX_STATE  0             /*I2C发送状态*/
#define TX_STATE  1             /*I2C接收状态*/
//------对SMCLK分频产生I2C通信波特率------
#define I2C_BAUDRATE_DIV 14     /*I2C波特率控制*/
#define SLAVE_ADDR  0x20        /*从机地址*/
static unsigned char TxByteCnt = 0;   //剩余发送数据
static unsigned char RxByteCnt = 0;   //剩余接收数据
static unsigned char *pTxData;        //待发送Tx数据的指针
static unsigned char *pRxData;        //Rx接收存放数据的指针
extern unsigned char I2C_State = 0;          //收发状态指示变量

void I2C_Send_Char(unsigned char data){}//兼容软件IC
void I2C_Master_Ask(){}//兼容软件IC
unsigned char I2C_Get_Char(){}//兼容软件IC


/********************************************************
*名       称：USCI_B0_Init()
*功       能：初始化USCI_B0模块为I2C模式
*入口参数：无
*出口参数：无
*说       明：与下个函数功能相同
********************************************************/
void USCI_B0_Init()
{
	/*************UCB0CTL0******************
	 UCA10 (BIT7)      本机地址10位，默认7位
	 UCSLA10(BIT6)     从机地址10位 ，默认7位
	 UCMM(BIT5)        多主机通信，默认单主机
	 Unused(BIT4)
	 UCMST(BIT3)       主机模式，默认从机，配置为从机时只有这一位不一样
	 UCMODEx(BIT2\BIT1) 00-3-pin SPI\01、10-4-pin SPI\11-I2C
	 UCSYNC(BIT0)       同步通信，默认异步
	 **************************************/
	UCB0CTL1 |= UCSWRST;  //关闭USCI_B0模块以便进行初始化
	UCB0CTL0 = UCMST + UCMODE_3 +UCSYNC;
	/*****************UCB0CTL1*************
	UCSSELx(BIT7\BIT6) 00-UCLKI\01-ACLK\10-SMCLK\11-SMCLK
	Unused(BIT5)
	UCTR(BIT4)         通信时写模式，默认读模式
	UCTXNACK(BIT3)     发送“无应答”，默认不发送
	UCTXSTP(BIT2)      主机读取模式下在发送“无应答”后触发停止位，默认不触发停止位
	UCTXSTT(BIT1)      主机读取模式下在发送“无应答”后重复出发起始位，默认不触发起始位
	UCSWRST(BIT0)      USCI复位（关闭），默认释放（打开）
	**************************************/
	UCB0CTL1 = UCSSEL_2 + UCSWRST;
	UCB0I2COA = 10;      //主机地址
	UCB0I2CSA = 0;       //从机地址
	UCB0BR0 = 10;        //设置通信速率
	/****************USB0I2CIE*********************
	Reserved(BIT7-4)
	UCNACKIE(BIT3)    //开启无应答中断
	UCSTPIE(BIT2)     //开启起始位中断
	UCSTTIE(BIT1)     //开启停止位中断
	UCALIE(BIT0)      //开启仲裁失败中断
	*****************************************/
	UCB0I2CIE = UCNACKIE + UCSTPIE + UCSTTIE + UCALIE;
	UCB0CTL1 &= ~UCSWRST;//打开USCI模块
}

/********************************************************
*名       称：I2C_Init()
*功       能：初始化USCI_B0模块为I2C模式
*入口参数：无
*出口参数：无
*说       明：I2C设为3线制主机状态，暂不使能Tx和Rx中断
********************************************************/
void I2C_Init()
{
	_EINT();
	P1SEL |= BIT6 + BIT7;      //GPIO配置为USCI_B0功能
	P1SEL2 |= BIT6 + BIT7;     //GPIO配置为USCI_B0功能
	UCB0CTL1 |= UCSWRST;       //软件复位状态
	UCB0CTL0 = UCMST + UCMODE_3 +UCSYNC;//同步通信I2C主机状态
	UCB0CTL1 = UCSSEL_2 + UCSWRST;      //使用SMCLK，软件复位状态
	UCB0BR0 = I2C_BAUDRATE_DIV;   //除了分频系数，实际波特率还与SMCLK有关
	UCB0BR1 = 0;                  //这一级分频一般不用
	UCB0I2CSA = SLAVE_ADDR;       //从机地址
	UCB0CTL1 &= ~UCSWRST;         //开I2C
	_EINT();
}

/********************************************************
*名       称：I2C_Tx_Init()
*功       能：仅使能I2C的Tx中断，关闭Rx中断
*入口参数：无
*出口参数：无
*说       明：I2C只能半双工，只使能一个中断，工作可靠
********************************************************/
void I2C_Tx_Init()
{
	_DINT();
	while( (UCB0STAT & UCBUSY) | (UCB0CTL1 & UCTXSTP) );//确保总线空闲
	IE2 &= ~UCB0RXIE;                    //关Rx中断
	I2C_State = TX_STATE;
	IE2 |= UCB0TXIE;                     //开Tx中断
	_EINT();
}

/********************************************************
*名       称：I2C_Rx_Init()
*功       能：仅使能I2C的Rx中断，关闭Tx中断
*入口参数：无
*出口参数：无
*说       明：I2C只能半双工，只使能一个中断，工作可靠
********************************************************/
void I2C_Rx_Init()
{
	_DINT();
	while( (UCB0STAT & UCBUSY) | (UCB0CTL1 & UCTXSTP) );//确保总线空闲
	IE2 &= ~UCB0TXIE;                    //关Rx中断
	I2C_State = RX_STATE;
	IE2 |= UCB0RXIE;                     //开Tx中断
	_EINT();
}

/********************************************************
*名       称：I2C_TxFrame()
*功       能：新发送一帧数据
*入口参数：*p_Tx-待发送数据的首地址
		  num-待发送数据的个数
*出口参数：0-失败 1-成功
*说       明：只有不BUSY而且STOP已经复位的情况下才允许发送新的帧
********************************************************/
unsigned char I2C_TxFrame( unsigned char *p_Tx , unsigned char num)
{
	if( (UCB0STAT & UCBUSY) | (UCB0CTL1 & UCTXSTP) ) return (0);
	pTxData = (unsigned char*)p_Tx;  //更新数据指针
	TxByteCnt = num;                 //更新剩余接收数据的个数
	UCB0CTL1 |= UCTR + UCTXSTT;      //I2C Tx位，软件start condition
	_EINT();
	return(1);
}

/********************************************************
*名       称：I2C_RxFrame()
*功       能：新接收一帧数据
*入口参数：*p_Rx-待存放数据的首地址
		  num-待接收数据的个数
*出口参数：0-失败 1-成功
*说       明：只有不BUSY而且STOP已经复位的情况下才允许接收新的帧
********************************************************/
unsigned char I2C_RxFrame( unsigned char *p_Rx , unsigned char num)
{
	if( (UCB0STAT & UCBUSY) | (UCB0CTL1 & UCTXSTP) ) return (0);
	pRxData = (unsigned char*)p_Rx;   //更新数据指针
	RxByteCnt = num;                  //更新剩余接收数据个数
	UCB0CTL1 &= ~UCTR;                //I2C Rx位
	UCB0CTL1 |= UCTXSTT;              //软件start condition
	_EINT();
	return (1);
}

/********************************************************
*名       称：I2C_TxFrame_ISR()
*功       能：I2C的Tx事件处理函数，发送缓存数组中的数据
*入口参数：无
*出口参数：无
*说       明：类似FIFO操作，但是指针无需循环
********************************************************/
void I2C_TxFrame_ISR(void)
{
	if( TxByteCnt )   //检查数据是否发完
	{
		UCB0TXBUF = *pTxData++;   //装填待发送的数据，并更新数据大小
		TxByteCnt--;
	}
	else
	{
		UCB0CTL1 |= UCTXSTP;      //置停止位
		IFG2 &= ~UCB0TXIFG;       //人工清除标志位
	}
}

/********************************************************
*名       称：I2C_RxFrame_ISR()
*功       能：I2C的Rx事件处理函数，读取UCB0RXBUF写入指定缓存数组中
*入口参数：无
*出口参数：无
*说       明：类似FIFO操作，但是指针无需循环
********************************************************/
void I2C_RxFrame_ISR(void)
{
	if( RxByteCnt == 1)       //只剩1字节没接收时（实际已经在RxBuff里）
		UCB0CTL1 |= UCTXSTP;  //软件产生停止位，更新剩余数据大小
	RxByteCnt--;
	*pRxData++ = UCB0RXBUF;   //存储已接收的数据，更新接收数据大小
}

/********************************************************
*名       称：USCIAB0TX_ISR()
*功       能：I2C中断服务函数
*入口参数：无
*出口参数：无
*说       明：中断内的数据处理函数位于I2C.c
********************************************************/
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
	_DINT();
	if( I2C_State == TX_STATE )  //判断是收状态还是发状态
		I2C_TxFrame_ISR();       //发送帧
	else
		I2C_RxFrame_ISP();       //接收帧
	_EINT();
}


#endif

//------------------------分割线------------------------------

#ifdef SOFT_I2C
#define I2C_I2C_MCLK_FREQ 8000000  //实际时钟频率
//-----屏蔽硬件差异，对IO进行宏定义-----
#define I2C_CLK_HIGH  P1DIR &= ~BIT6
#define I2C_CLK_LOW   P1DIR |= BIT6;P1OUT &= ~BIT6
#define I2C_DATA_HIGH P1DIR &= ~BIT7
#define I2C_DATA_LOW  P1DIR |= BIT7;P1OUT &= ~BIT7
#define I2C_DATA_IN   P1IN & BIT7
#define I2C_START I2C_Start()
#define I2C_STOP  I2C_Stop()
//-----从机地址宏及读写操作位的宏定义-----
#define SLAVE_ADDR  0x20  //实际从机地址
#define SLAVE_ADDR_W  SLAVE_ADDR<<1  //实际从机地址+写标志。不用修改
#define SLAVE_ADDR_R  (SLAVE_ADDR<<1)+1//实际从机地址+读标志。不用修改
//-------为兼容硬件I2C编写的空函数-------
void I2C_Tx_Init(){}
void I2C_Rx_Init(){}

/***********************************************************
*名       称：I2C_delay_us()
*功       能：us精确延时
*入口参数：无
*出口参数：无
*说       明：实际延时值与CPU时钟频率有关，所以用SI2C_MCLK_FREQ来自动调整
	             不同的I2C设备速度不一样，延时参数视情况而定
***********************************************************/
static void I2C_delay_us()//默认取空函数
{
	//__delay_cycles(I2C_MCLK_FREQ / 1000);    //1000us
	//__delay_cycles(I2C_MCLK_FREQ / 10000);   //100us
	//__delay_cycles(I2C_MCLK_FREQ / 100000);  //10us
	//__delay_cycles(I2C_MCLK_FREQ / 1000000); //1us
}

/********************************************************
*名       称：I2C_Start()
*功       能：模拟I2C起始位
*入口参数：无
*出口参数：无
*说       明：在数据总线保持高电平时，数据总线产生下降沿
                      总线空闲时总是高电平
********************************************************/
void I2C_Start(void)
{
	I2C_DATA_HIGH;
	I2C_delay_us();
	I2C_CLK_HIGH;
	I2C_delay_us();
	I2C_DATA_LOW;
	I2C_delay_us();
	I2C_CLK_LOW;
	I2C_delay_us();
}

/********************************************************
*名       称：I2C_Stop()
*功       能：模拟I2C停止位
*入口参数：无
*出口参数：无
*说       明：在数据总线保持高电平时，数据总线产生上升沿
                      总线空闲时总是高电平
********************************************************/
void I2C_Stop(void)
{
	I2C_CLK_LOW;
	I2C_delay_us();
	I2C_DATA_LOW;
	I2C_delay_us();
	I2C_CLK_HIGH;
	I2C_delay_us();
	I2C_DATA_HIGH;
	I2C_delay_us();
}

/********************************************************
*名       称：I2C_Init()
*功       能：兼容硬件I2C的格式的初始化函数
*入口参数：无
*出口参数：无
*说       明：I2C的初始状态就是总线释放状态
********************************************************/
void I2C_Init(void)
{
	P1DIR |= BIT6;    //SCL输出
	P1DIR &= ~BIT7;   //SDA输入
	I2C_CLK_LOW;
	I2C_STOP;
}

/********************************************************
*名       称：I2C_Send_Char()
*功       能：主机向从机发送1个字节数据
*入口参数：data-待发送数据
*出口参数：无
*说       明：发完8位从机有一个应答位，不接收，但提供一个CLK始终
********************************************************/
void I2C_Send_Char(unsigned char data)
{
	unsigned char i=0;
	for( i=0 ; i<8 ; i++ )
	{
		if( (data<<i) & BIT7 )//先发高位
			I2C_DATA_HIGH;
		else
			I2C_DATA_LOW;
		I2C_CLK_HIGH;
		I2C_delay_us();
		I2C_CLK_LOW;
		I2C_delay_us();
	}
	I2C_CLK_HIGH; //模拟接收从机应答
	I2C_delay_us();
	I2C_CLK_LOW;
	I2C_delay_us();
}

/********************************************************
*名       称：I2C_Master_Ask()
*功       能：主机接受应答
*入口参数：无
*出口参数：无
*说       明：主机接受8个bit数据后，要控制数据线发出0应答信号，
		     然后释放总线
********************************************************/
void I2C_Master_Ask(void)
{
	I2C_DATA_LOW;
	I2C_delay_us();
	I2C_CLK_HIGH;   //发出0应答位
	I2C_delay_us();
	//----释放总线----
	I2C_CLK_LOW;
	I2C_delay_us();
	I2C_DATA_HIGH;
}

/********************************************************
*名       称：I2C_Get_Char()
*功       能：主机从从机接收1个字节数据
*入口参数：无
*出口参数：temp-待发送数据
*说       明：接收完8位数据，主机要给出应答位
********************************************************/
unsigned char I2C_Get_Char(void)
{
	unsigned char i = 0;
	unsigned char temp = 0;
	I2C_DATA_HIGH;
	for( i=8 ; i>0 ; i-- )
	{
		I2C_CLK_HIGH;
		I2C_delay_us();
		temp = temp <<1;   //若放在后面则导致最高位丢失
		if( I2C_DATA_IN == BIT7 )
			temp |= BIT0;
		I2C_delay_us();
		I2C_CLK_LOW;
		I2C_delay_us();
	}
	//----应答位----
	I2C_Master_Ask();
	return (temp);
}

/********************************************************
*名       称：I2C_TxFrame()
*功       能：主机发送若干字节的一帧数据
*入口参数：*p_TX-指向待发送数组
		  num-待发送数据个数
*出口参数：1-发送完毕
*说       明：无
********************************************************/
unsigned char I2C_TxFrame(unsigned char *p_Tx , unsigned char num)
{
	_DINT();
	unsigned char i=0;
	I2C_START;
	I2C_Send_Char( SLAVE_ADDR_W ); //发送地址和写标志
	for( i=num ; i>0 ; i-- )
		I2C_Send_Char( *p_Tx++ );
	I2C_STOP;
	_EINT();
	return 1;
}

/********************************************************
*名       称：I2C_RxFrame()
*功       能：主机接收若干字节的一帧数据
*入口参数：*p_Rx-指向存放接收数据数组
		  num-待接收数据个数
*出口参数：1-接收完毕
*说       明：无
********************************************************/
unsigned char I2C_RxFrame(unsigned char *p_Rx , unsigned char num )
{
	unsigned char i = 0;
	_DINT();
	I2C_START;
	I2C_Send_Char( SLAVE_ADDR_R );//先发送器件地址和读标志
	for( i=num ; i>0 ; i-- )
		*p_Rx++ = I2C_Get_Char();
	I2C_STOP;
	_EINT();
	return 1;
}
#endif
