/*
 * data_transform.c
 *
 *  Created on: 2015-7-2
 *      Author: Isaac
 */
#include"data_transform.h"
#include"system_control.h"

//----转换后数据----
unsigned int  Simu_Data[7] = {0};  //年月日时分秒
unsigned char Trans_Data[20] = {0};//整理后的数据
char Display_Data[11] = {0};   //存放年月日显示数据
char Display_Data2[9] = {0};   //存放时分秒显示数据
//----接受数据（只声明）----
//unsigned char rec_foot;
//unsigned char intsec_flag;//整0、20、40秒标识
//unsigned char Receive_Data[20]; //接收到时钟数据

/********************************************************
*名        称：Display_TransData()
*功        能：将时间数据转换成显示的数据
*入口参数 ：无
*出口参数 ：无
*说        明：
********************************************************/
void Display_TransData(void)
{
	//----年----
	Display_Data[0] = (Simu_Data[0]/1000)+'0';
	Display_Data[1] = (Simu_Data[0]%1000)/100+'0';
	Display_Data[2] = (Simu_Data[0]%100)/10+'0';
	Display_Data[3] = (Simu_Data[0]%10)+'0';
	Display_Data[4] = '/';
	//----月----
	if(Simu_Data[1] >= 10)
	{
		Display_Data[5] = (Simu_Data[1]/10)+'0';
		Display_Data[6] = (Simu_Data[1]%10)+'0';
	}
	else
	{
		Display_Data[5] = '0';
		Display_Data[6] = Simu_Data[1]+'0';
	}
	Display_Data[7] = '/';
	//----日----
	if(Simu_Data[2] >= 10)
	{
		Display_Data[8] = (Simu_Data[2]/10)+'0';
		Display_Data[9] = (Simu_Data[2]%10)+'0';
	}
	else
	{
		Display_Data[8] = '0';
		Display_Data[9] = Simu_Data[2]+'0';
	}
	Display_Data[10] = '\0';
	//----时----
	if(Simu_Data[3] >= 10)
		{
			Display_Data2[0] = (Simu_Data[3]/10)+'0';
			Display_Data2[1] = (Simu_Data[3]%10)+'0';
		}
		else
		{
			Display_Data2[0] = '0';
			Display_Data2[1] = Simu_Data[3]+'0';
		}
	Display_Data2[2] = ':';
	//----分----
	if(Simu_Data[4] >= 10)
	{
		Display_Data2[3] = (Simu_Data[4]/10)+'0';
		Display_Data2[4] = (Simu_Data[4]%10)+'0';
	}
	else
	{
		Display_Data2[3] = '0';
		Display_Data2[4] = Simu_Data[4]+'0';
	}
	Display_Data2[5] = ':';
	//----秒----
	if(Simu_Data[5] >= 10)
	{
		Display_Data2[6] = (Simu_Data[5]/10)+'0';
		Display_Data2[7] = (Simu_Data[5]%10)+'0';
	}
	else
	{
		Display_Data2[6] = '0';
		Display_Data2[7] = Simu_Data[5]+'0';
	}
	Display_Data2[8] = '\0';
}

/********************************************************
*名        称：Transform_Data()
*功        能：将接收的数据转换成时间数据并存储
*入口参数 ：无
*出口参数 ：无
*说        明：先将数据按顺序排列，再分别转换，最后转化成要显示的数据
********************************************************/
void Transform_Data(void)
{
	unsigned char n = 0;
	unsigned char i = 0;
	for(n = intsec_flag + 1 ; n < 20 ; n++)
	{
		Trans_Data[i] = Receive_Data[n];
		i++;
	}
	for(n = 0 ; n <= intsec_flag ; n++)
	{
		Trans_Data[i] = Receive_Data[n];
		i++;
	}
	Simu_Data[6] = Trans_Data[7]*4+Trans_Data[8]; //星期
	Simu_Data[5] = Trans_Data[0]*20+1+19-intsec_flag-1;//秒
	Simu_Data[4] = Trans_Data[4]*16+Trans_Data[5]*4+Trans_Data[6];//分
	if(Trans_Data[4]&0x02)  //时
		Simu_Data[3] = 12+Trans_Data[2]*4+Trans_Data[3];
	else Simu_Data[3] = Trans_Data[2]*4+Trans_Data[3];
	Simu_Data[2] = Trans_Data[10]*16+Trans_Data[11]*4+Trans_Data[12];//日
	Simu_Data[1] = Trans_Data[13]*4+Trans_Data[14];  //月
	Simu_Data[0] = Trans_Data[15]*16+Trans_Data[16]*4+Trans_Data[17]; //年
}



