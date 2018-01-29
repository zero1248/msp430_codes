#ifndef __TIMERA_H__
#define __TIMERA_H__

#define CCR0_VALUE        50000 //定时器计数值

#define PWM_MODE1_CCR2  1000//TA2低电平持续PWM_MODE1_CCR2个ACLK周期后自动变高

#define PWM_MODE2_6_CCR0  30000-1 //PWM周期=30000个ACLK周期
#define PWM_MODE2_6_CCR1  25000  //第一路高电平持续5000个ACLK周期
#define PWM_MODE2_6_CCR2  5000    //第二路高电平持续5000个ACLK周期

#define PWM_MODE3_CCR0  10000 - 1 //PWM总周期 =10000个ACLK周期
#define PWM_MODE3_CCR2  1000  //低电平时间= 1000个ACLK周期

#define PWM_MODE4_CCR0  1000 - 1 //输出方波周期 =2000个ACLK周期

#define PWM_MODE5_CCR2  1000  //TA2高电平持续1000个ACLK周期后自动变低

#define PWM_MODE7_CCR0  10000 - 1  //PWM总周期 =10000个ACLK周期
#define PWM_MODE7_CCR2  1000   //高电平时间= 1000个ACLK周期
#endif // __TIMERA_H__
