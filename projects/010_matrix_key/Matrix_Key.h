/*
 * Matrix_Key.h
 *
 *  Created on: 2015-6-26
 *      Author: Isaac
 */

#ifndef __MATRIX_KEY_H__
#define __MATRIX_KEY_H__
//----硬件接口----
#define MAT_SCAN_IO P1DIR
#define MAT_SCAN_OUT P1OUT	 //4×4键盘扫描端口，低4位是行线，高4位是列线。
			 //采用逐列扫描的方法，无按键时，低4位输出1，高4位输出0。
			 //当有按键时，高4位输出扫描电位，低4位输入扫描结果。
#define MAT_SCAN_IN  P1IN

#define MAT_KEYBUFF_SIZE   50    /*键盘缓冲区大小,根据程序需要自行调整*/

extern void Mat_Scan_Delay(unsigned int);
extern void P1_For_Scan_Init(void);
extern void TimerA_For_Scan_Init(void);
extern unsigned char MatKey_GetKey(void);
#endif /* MATRIX_KEY_H_ */
