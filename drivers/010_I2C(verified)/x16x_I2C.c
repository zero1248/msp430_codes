/*
 * I2C169.c
 *        Port: P1.6-SCL P1.7-SDA
 *  Created on: 2015-6-26
 *      Author: Isac
 */
 #include<msp430f169.h>
 #include"I2C.h"


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
