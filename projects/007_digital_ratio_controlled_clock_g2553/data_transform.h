/*
 * data_transform.h
 *
 *  Created on: 2015-7-2
 *      Author: Isaac
 */

#ifndef __DATA_TRANSFORM_H__
#define __DATA_TRANSFORM_H__

extern unsigned int  Simu_Data[7];  //年月日时分秒
extern unsigned char Trans_Data[20];//整理后的数据
extern char Display_Data[11];   //存放年月日显示数据
extern char Display_Data2[9];   //存放时分秒显示数据

extern void Display_TransData(void);
extern void Transform_Data(void);

#endif /* DATA_TRANSFORM_H_ */
