/*
 * BCL.c
 *
 *  Created on: 2015-4-27
 *      Author: Isaac
 */
#include<msp430g2553.h>
#include"BCL.h"
void BCS_Init(void)
{
    BCSCTL2 = SELM_0 | DIVM_0 | DIVS_0;
    if (CALBC1_8MHZ != 0xFF)
    {
        __delay_cycles(5000);
        DCOCTL = 0x00;
        BCSCTL1 = CALBC1_8MHZ;     /* Set DCO to 8MHz */
        DCOCTL = CALDCO_8MHZ;
    }
    BCSCTL1 |= XT2OFF | DIVA_0;
    BCSCTL3 = XT2S_0 | LFXT1S_0 | XCAP_1;
}



