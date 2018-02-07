/*
 * com_protocol.h
 *
 *  Created on: 2015-4-30
 *      Author: Isaac
 */

#ifndef __COM_PROTOCOL_H__
#define __COM_PROTOCOL_H__

//----------通信配置-----------
#define MATCHING_CODE  0x55    //匹配码（起始码）


extern unsigned char Tx_Data_Packet[];
extern unsigned char Rx_Data_Packet[];
extern char temperature_temp[];
extern void Display_Data(void);
extern void Display_FixedData(void);
extern char Extract_Data(void);
extern void Analysis_Data(void);

#endif /* COM_PROTOCOL_H_ */
