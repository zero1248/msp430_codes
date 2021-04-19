/*
 * UART.c
 *
 *  Created on: 2015-4-27
 *      Author: Isaac
 */
#include<msp430g2553.h>
#include"UART.h"
#include"com_protocol.h"
//#include"18B20.h"

unsigned char Rx_FIFO[RX_FIFO_SIZE]={0}; //UART接收FIFO数组
unsigned char Rx_FIFO_DataNum = 0;       //UART接收FIFO“空满”指示
unsigned char Rx_FIFO_IndexR = 0;        //UART接收FIFO的模拟“读指针”
unsigned char Rx_FIFO_IndexW = 0;        //UART接收FIFO的模拟“写指针”

unsigned char Tx_FIFO[TX_FIFO_SIZE]={0}; //UART发送FIFO数组
unsigned char Tx_FIFO_DataNum = 0;       //UART发送FIFO“空满”指示
unsigned char Tx_FIFO_IndexR = 0;        //UART发送FIFO的模拟“读指针”
unsigned char Tx_FIFO_IndexW = 0;        //UART发送FIFO的模拟“写指针”

/********************************************************
*名        称：UART_TXRX_OPEN()
*功        能：UART初始化，并打开TXD、RXD
*入口参数：无
*出口参数：无
*说        明：P1.1-RXD P1.2-TXD  9600，8，n，1
		   此函数实测时有问题，可能是由于收发太快造成冲突，暂不使用
********************************************************/
void UART_TXRX_OPEN(void)
{
	//-------开启IO口的TXD和RXD功能-------
	P1DIR |= BIT2;
	P1DIR &= ~BIT1;
	P1SEL = BIT1 + BIT2;
	P1SEL2 = BIT1 + BIT2;
	UCA0CTL1 |= UCSWRST;  //暂时关闭UCA0
	//-------设置UART时钟源--------
	UCA0CTL1 = UCSSEL_1 | UCSWRST; //CLK=ACLK
	//----------设置波特率---------
	UCA0BR0 = 0x03;    //32k/9600=3.41
	UCA0BR1 = 0x00;
	UCA0MCTL = UCBRF_0 | UCBRS_3;
	UCA0CTL1 &= ~UCSWRST;
	IE2 |= UCA0RXIE + UCA0TXIE;
}

/********************************************************
*名        称：UART_TX_OPEN()
*功        能：打开IO的TXD功能并关闭RXD功能，同时让RXD输出低电平
*入口参数：无
*出口参数：无
*说        明：P1.1-RXD P1.2-TXD
********************************************************/
void UART_TX_OPEN(void)
{
	P1DIR |= BIT1;
	P1DIR &= ~BIT2;
	P1OUT &= ~BIT1;
	P1SEL |= BIT2;
	P1SEL &= ~BIT1;
	P1SEL2 |= BIT2;
	P1SEL2 &= ~BIT1;
	UCA0CTL1 |= UCSWRST;  //暂时关闭UCA0
	//-------设置UART时钟源--------
	UCA0CTL1 = UCSSEL_1 | UCSWRST; //CLK=ACLK
	//----------设置波特率---------
	UCA0BR0 = 0x03;    //32k/9600=3.41
	UCA0BR1 = 0x00;
	UCA0MCTL = UCBRF_0 | UCBRS_3;
	UCA0CTL1 &= ~UCSWRST;
	IE2 &= ~UCA0RXIE;
	IE2 |= UCA0TXIE;
}

/********************************************************
*名        称：UART_RX_OPEN()
*功        能：打开IO的RXD功能并关闭TXD功能，同时让TXD输出低电平
*入口参数：无
*出口参数：无
*说        明：P1.1-RXD P1.2-TXD
********************************************************/
void UART_RX_OPEN(void)
{
	P1DIR &= ~BIT1;
	P1DIR |= BIT2;
	P1OUT &= ~BIT2;
	P1SEL |= BIT1;
	P1SEL &= ~BIT2;
	P1SEL2 |= BIT1;
	P1SEL2 &= ~BIT2;
	UCA0CTL1 |= UCSWRST;  //暂时关闭UCA0
	//-------设置UART时钟源--------
	UCA0CTL1 = UCSSEL_1 | UCSWRST; //CLK=ACLK
	//----------设置波特率---------
	UCA0BR0 = 0x03;    //32k/9600=3.41
	UCA0BR1 = 0x00;
	UCA0MCTL = UCBRF_0 | UCBRS_3;
	UCA0CTL1 &= ~UCSWRST;
	IE2 &= ~UCA0TXIE;
	IE2 |= UCA0RXIE;
}


/********************************************************
*名        称：Rx_FIFO_WriteChar()
*功        能：往Rx接收FIFO中写1字节
*入口参数：Data-待写入FIFO的数据
*出口参数：1-成功，0-失败
*说        明：操作FIFO要关总中断
********************************************************/
unsigned char Rx_FIFO_WriteChar(unsigned char Data)
{
	if( Rx_FIFO_DataNum >= RX_FIFO_SIZE ) return (0);//FIFO装满返回0
	_DINT();
	Rx_FIFO_DataNum++;
	Rx_FIFO[Rx_FIFO_IndexW++] = Data;    //数据写入，尾指针移位
	if ( Rx_FIFO_IndexW >= RX_FIFO_SIZE )//判断尾指针是否越界
		Rx_FIFO_IndexW = 0;              //写指针归零
	_EINT();
	return (1);
}



/********************************************************
*名        称：Rx_FIFO_ReadChar()
*功        能：从Rx接收FIFO中读1字节
*入口参数：无
*出口参数：temp-从队列中读取的数据
*说        明：操作FIFO要关总中断
********************************************************/
unsigned char Rx_FIFO_ReadChar(void)
{
	unsigned char temp = 0;
	if( Rx_FIFO_DataNum == 0 ) return (0);//没有未读数据返回0
	_DINT();
	Rx_FIFO_DataNum--;                   //待读取数据个数减一
	temp = Rx_FIFO[Rx_FIFO_IndexR++];     //读取数据，头指针移位
	if( Rx_FIFO_IndexR >= RX_FIFO_SIZE )  //判断头指针越界
		Rx_FIFO_IndexR = 0;
	_EINT();
	return(temp);
}

/********************************************************
*名        称：Rx_FIFO_Clear()
*功        能：清空Rx接收FIFO区
*入口参数：无
*出口参数：无
*说        明：只需清零读写指针和满空计数值，不需要每位写0
********************************************************/
void Rx_FIFO_Clear(void)
{
	_DINT();
	Rx_FIFO_DataNum = 0;
	Rx_FIFO_IndexR = 0;
	Rx_FIFO_IndexW = 0;
	_EINT();
}

/********************************************************
*名        称：Tx_FIFO_WriteChar()
*功        能：往Tx发送FIFO中写1字节
*入口参数：Data-待写入FIFO的数据
*出口参数：1-成功，0-失败
*说        明：“全新”一次发送数据必须手动触发Tx中断；“非全新”触发
		    一定不能手动触发Tx中断，全新发送的判据：同时满足FIF
		    O无数据和Tx不BUSY
********************************************************/
unsigned char Tx_FIFO_WriteChar(unsigned char Data)
{
	if( Tx_FIFO_DataNum >= TX_FIFO_SIZE ) return (0); //装满未读数据返回0
	_DINT();
	//---------------------------------
	if( (Tx_FIFO_DataNum == 0) && (!(UCA0STAT & UCBUSY)))
		IFG2 |= UCA0TXIFG;     //“全新”发送手动触发中断
	Tx_FIFO_DataNum++;
	Tx_FIFO[Tx_FIFO_IndexW++] = Data;  //写数据，尾指针移位
	if( Tx_FIFO_IndexW >= TX_FIFO_SIZE)//尾指针越界判断
		Tx_FIFO_IndexW = 0;
	_EINT();
	return(1);
}

/********************************************************
*名        称：Tx_FIFO_ReadChar()
*功        能：从Tx发送FIFO中读1字节
*入口参数：*Chr-待存放字节变量的指针
*出口参数：1-成功，0-失败
*说        明：操作FIFO需关闭总中断
********************************************************/
unsigned char Tx_FIFO_ReadChar(unsigned char *Chr)
{
	if( Tx_FIFO_DataNum == 0 ) return (0); //没有未读数据返回0
	_DINT();
	Tx_FIFO_DataNum--;
	*Chr = Tx_FIFO[Tx_FIFO_IndexR++];//读数据，头指针移位
	if( Tx_FIFO_IndexR >= TX_FIFO_SIZE )//头指针越界判断
		Tx_FIFO_IndexR = 0;
	_EINT();
	return (1);
}

/********************************************************
*名        称：Tx_FIFO_Clear()
*功        能：清空Tx发送FIFO区
*入口参数：无
*出口参数：无
*说        明：只需清零读写指针和满空计数值，不需要每位写0
********************************************************/
void Tx_FIFO_Clear(void)
{
	_DINT();
	Tx_FIFO_DataNum = 0;
	Tx_FIFO_IndexR = 0;
	Tx_FIFO_IndexW = 0;
	_EINT();
}

/********************************************************
*名        称：UART_SendString()
*功        能：发送一帧数据
*入口参数：无
*出口参数：无
*说        明：连续发送
********************************************************/
void UART_SendString( unsigned char *Ptr)
{
	while(*Ptr)
	{
		Tx_FIFO_WriteChar( *Ptr++ );
	}
	//DS18B20_TADelay_us(8000);
	//DS18B20_TADelay_us(8000);
}


/********************************************************
*名        称：UART_OnTx()
*功        能：UART的Tx事件处理函数
*入口参数：无
*出口参数：无
*说        明：Tx_FIFO里有数据就将数据移到TxBuffer寄存器里
********************************************************/
void UART_OnTx(void)
{
	unsigned char Temp = 0;
	if( Tx_FIFO_DataNum > 0)
	{
		Tx_FIFO_ReadChar(&Temp);
		UCA0TXBUF = Temp;
	}
}

/********************************************************
*名        称：UART_OnRx()
*功        能：UART的Rx事件处理函数
*入口参数：无
*出口参数：无
*说        明：对接收到的数据区别对待进行处理
********************************************************/
void UART_OnRx(void)
{
	unsigned char Temp = 0;
	Temp = UCA0RXBUF;
	Rx_FIFO_WriteChar(Temp);
}

/********************************************************
*名        称：UART_ISR()
*功        能：响应Tx中断服务
*入口参数：无
*出口参数：无
*说        明：凡是中断标志位有可能不被自动清除的，均手动清除一次
********************************************************/
#pragma vector=USCIAB0TX_VECTOR
__interrupt void UART_Tx_ISR(void)
{
	IFG2 &= ~UCA0TXIFG;//手动清除标志位
	UART_OnTx();       //调用Tx事件处理函数
}

/********************************************************
*名        称：UART_ISR()
*功        能：响应Rx中断服务
*入口参数：无
*出口参数：无
*说        明：凡是中断标志位有可能不被自动清除的，均手动清除一次
********************************************************/
#pragma vector=USCIAB0RX_VECTOR
__interrupt void UART_Rx_ISR(void)
{
	IFG2 &= ~UCA0RXIFG;//手动清除标志位
	UART_OnRx();       //调用Tx事件处理函数

}
