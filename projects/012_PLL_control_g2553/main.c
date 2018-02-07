#include<msp430.h>
#include"BCL.h"
#include"my_430.h"
#include"PLL_control.h"
void main(void)
{
	WDT_OUT;
	BCS_Init();
	Port_Init();
	__delay_cycles(80000);
	ConReg_Emit();
	RefFre_Emit( REF_DATA_12 );
	Tx_Counter_Emit( Rx_16 );
	while(1){}
}
