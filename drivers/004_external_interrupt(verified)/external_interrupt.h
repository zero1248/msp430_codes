#ifndef  __EXTERNAL_INTERRUPT_H__
#define __EXTERNAL_INTERRUPT_H__

#define EX_INTRPT_ALL  0xff
#define RAISING_EDGE   1
#define FALLING_EDGE   0

void P1_Interrupt_Init(unsigned char io_sel , unsigned char up_down );
void P2_Interrupt_Init(unsigned char io_sel , unsigned char up_down );
#endif // __EXTERNAL_INTERRUPT_H__

