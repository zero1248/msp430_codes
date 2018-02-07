#include<msp430.h>
#include"BCL.h"
#include"TimerA_PWM.h"
#include"system_control.h"

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	BCS_Init();
	Wireless_IO_Init();
	//TimerA_For_Key_Init();
	while(1)
	{
		//Wireless_Key_Read();
		Wireless_Packet_Data();
	}
}
