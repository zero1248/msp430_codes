/*
 * com_protocol.h
 *
 *  Created on: 2015-4-30
 *      Author: Isaac
 */

#ifndef __COM_PROTOCOL_H__
#define __COM_PROTOCOL_H__
//----------条件编译选择-----------
//#define MONITOR_TERMINAL
#define MONITOR_NODE1
//#define MONITOR_NODE2
//----------通信配置-----------
#define MATCHING_CODE  0x55    //匹配码（起始码）
#define HOST_ADDRESS   0x40    //主机地址
#define NODE_1 0x41            //节点1地址
#define NODE_2 0x42            //节点2地址

extern unsigned char Tx_Data_Packet[];
extern unsigned char Rx_Data_Packet[];
extern char temperature_temp[];
extern void Display_Data(void);
extern void Display_FixedData(void);
extern void Packet_Data( unsigned char source_address,unsigned char target_address );
extern unsigned char Extract_Data(void);
extern void Analysis_Data(void);

#endif /* COM_PROTOCOL_H_ */
