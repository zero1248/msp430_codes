#include<msp430.h>
#include"TimerA_PWM.h"
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    BCS_Init();
}
