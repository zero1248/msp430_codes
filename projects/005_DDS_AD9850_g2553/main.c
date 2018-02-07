#include<msp430.h>
#include"my_430.h"
#include"BCL.h"
#include"DDS_control.h"

void main(void) {
	double i = 1.0;
	WDT_OUT;
    BCS_Init();
    Port_Init();
    __delay_cycles(8000000);
    AD9850_Reset_Parrel();
    i = 0;
    while(1)
    {
    	__delay_cycles(8000000);
    	AD9850_Write_Parrel(0x00,27000000);
//    	for(i=1000 ; i<10000000 ; i+=1000)
//    	{
//    		__delay_cycles(800000);
//    		AD9850_Write_Parrel(0x00,i);
//    	}
    }
}
