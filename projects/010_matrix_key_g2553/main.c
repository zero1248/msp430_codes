#include <msp430.h> 
#include"my_430.h"
#include"Matrix_Key.h"
void main(void){
    unsigned char key_value ;
	WDT_OUT;
	IO_INIT;
	Clk_Init();
	P1_For_Scan_Init();
	TimerA_For_Scan_Init();
	_EINT();
    while(1){
        key_value = MatKey_GetKey();
		switch(key_value){
		case 1 : P3OUT = 0x01; break;
		case 2 : P3OUT = 0x02; break;
		case 3 : P3OUT = 0x04; break;
		case 4 : P3OUT = 0x08; break;
		case 5 : P3OUT = 0x10; break;
		case 6 : P3OUT = 0x20; break;
		case 7 : P3OUT = 0x40; break;
		case 8 : P3OUT = 0x80; break;
		default :P3OUT = 0x00; break;
		}
		Mat_Scan_Delay(1);
    }
}
