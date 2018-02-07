/*
 * SPWM.h
 *
 *  Created on: 2015-5-24
 *      Author: Isaac
 */

#ifndef __SPWM_H__
#define __SPWM_H__

#define CSMCLK  12000000  //SMCLK为12MHz
#define SIN_F   200       //定义输出正弦波频率为200Hz
#define SIN_NUM 16        //定义正弦波查表采样数据

extern void ADC10_WaveSample(void);
extern void Timer0_A_Init(void);
extern void Change_Duty(void);



#endif /* SPWM_H_ */
