#include<msp430g2553.h>
#include"BCL.h"
#include"my_430.h"
#include"OLED.h"
#include"I2C.h"
#include"key.h"
#include"system_control.h"
#include"data_transform.h"

void main(void)
{
	WDT_OUT;
	Port_Init();
	BCS_Init();
	Key_Init();
	TA_Capture0_Init();
	Display_Fixed();
	Get_Data();
	Simu_Clock_Open();
	while(1)
	{
		if(getdata_flag == 1)
		{
			getdata_flag = 0;
			Get_Data();
			Simu_Clock_Open();
		}
	}
}
