#include <msp430x16x.h>
#include "ADC12.h"
/*
 * main.c
 */
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    Init_ADC();
    _EINT();
    while(1);
}
