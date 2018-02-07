#include <msp430.h> 
#include"BCL.h"
#include"codetab.h"
#include"com_protocol.h"
#include"I2C.h"
#include"key.h"
#include"my_430.h"
#include"OLED.h"
#include"UART.h"
#include"ADC10.h"
void main(void)
{
	WDT_OUT;
	Port_Init();
	P1OUT &= ~BIT4;
	BCS_Init();
	I2C_Init();
	OLED_I2C_Init();
	OLED_Init();
	UART_RX_OPEN();
	Display_FixedData();
	ADC10_Init();
	_EINT();

	while(1)
	{

		if( Extract_Data() )
		{
			__delay_cycles(8000);
			Analysis_Data();
		}
		__delay_cycles(800000);
		Start_ADC10();
	}
}
