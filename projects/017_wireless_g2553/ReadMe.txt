/*
 * 无线环境监测系统
 *
 *  Created on: 2015-5-1
 *      Author: Isaac
 */


P1.1   UARTRXD             <UART.c>   //有待优化
P1.2   UARTTXD             <UART.c>   //有待优化
P1.3   电量监测比较器输入端                         <electric_monitor.h>
P1.4   18B20数据线                                       <18B20.h>
P1.5   光敏电阻数据端                                       <photoresistance.h>
P1.6   OLEDSCL             <I2C.c>    //有待优化
P1.7   OLEDSDA             <I2C.c>    //有待优化

！！！！！在com_protocol.h(通信协议)文件中配置整个工程是服务于监测终端还是监测节点
！！！！！在UART.h(UART通信)文件中配置接收队列和发送队列长度
在I2C.h(I2C通信)文件中配置软件I2C模式或硬件I2C模式
在my_430.h文件中配置单片机型号