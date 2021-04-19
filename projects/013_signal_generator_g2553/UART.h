/*
 * UART.h
 *        Port: P1.1-RXD P1.2-TXD
 *  Created on: 2015-4-27
 *      Author: Isaac
 */
#ifndef __UART_H__
#define __UART_H__

#define RX_FIFO_SIZE 128
#define TX_FIFO_SIZE 16

extern unsigned char Rx_FIFO[RX_FIFO_SIZE];
extern unsigned char Rx_FIFO_DataNum;
extern unsigned char Rx_FIFO_IndexR;
extern unsigned char Rx_FIFO_IndexW;

extern unsigned char Tx_FIFO[TX_FIFO_SIZE];
extern unsigned char Tx_FIFO_DataNum;
extern unsigned char Tx_FIFO_IndexR;
extern unsigned char Tx_FIFO_IndexW;
//------------------初始化操作-------------------
extern void UART_TX_OPEN(void);
extern void UART_RX_OPEN(void);
extern void UART_TXRX_OPEN(void);
//------------------RXD相关操作------------------
extern unsigned char Rx_FIFO_WriteChar(unsigned char Data);
extern unsigned char Rx_FIFO_ReadChar(void);
extern void Rx_FIFO_Clear(void);
//------------------TXD相关操作------------------
extern unsigned char Tx_FIFO_WriteChar(unsigned char Data);
extern unsigned char Tx_FIFO_ReadChar(unsigned char *Chr);
extern void Tx_FIFO_Clear(void);
//-----------------串口传输字符串-----------------
extern void UART_SendString(unsigned char *Ptr);

#endif /* UART_H_ */
