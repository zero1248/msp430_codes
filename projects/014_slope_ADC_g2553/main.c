#include <msp430.h> 
#include<msp430g2553.h>

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
}
