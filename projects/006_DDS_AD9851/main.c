#include <msp430.h> 
#include"BCL.h"
#include"my_430.h"
#include"DDS_control.h"

/*
 * main.c
 */
void main(void) {
	double i=0;
	WDT_OUT;
    BCS_Init();
    Port_Init();
    __delay_cycles(8000000);
    AD9851_Reset_Parrel();
	i = 0;
//	AD9851_Write_Parrel(0x01,30000000);
    while(1)
    {
    	__delay_cycles(8000000);
    	AD9851_Write_Parrel(0x01,20000000);
//    	for(i=1000 ; i<10000000 ; i+=1000)
//    	{
//    		__delay_cycles(80000);
//    		AD9851_Write_Parrel(0x01,i);
//    	}
    }
}
