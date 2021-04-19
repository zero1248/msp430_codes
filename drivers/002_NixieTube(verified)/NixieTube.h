/*****************共阴数码管**********************/
/******74HC573OE直接接地，LE直接接VCC *******/
#ifndef   __NIXIETUBE_H__
#define  __NIXIETUBE_H__

//P3OUT段选数据
//P4OUT位选数据
extern unsigned char Segment_Tab[19];
/**************数码管输出数据*****************/
#define  NIXIE_DISPLAY0  P3OUT = Segment_Tab[0]
#define  NIXIE_DISPLAY1  P3OUT = Segment_Tab[1]
#define  NIXIE_DISPLAY2  P3OUT = Segment_Tab[2]
#define  NIXIE_DISPLAY3  P3OUT = Segment_Tab[3]
#define  NIXIE_DISPLAY4  P3OUT = Segment_Tab[4]
#define  NIXIE_DISPLAY5  P3OUT = Segment_Tab[5]
#define  NIXIE_DISPLAY6  P3OUT = Segment_Tab[6]
#define  NIXIE_DISPLAY7  P3OUT = Segment_Tab[7]
#define  NIXIE_DISPLAY8  P3OUT = Segment_Tab[8]
#define  NIXIE_DISPLAY9  P3OUT = Segment_Tab[9]
#define  NIXIE_DISPLAYA  P3OUT = Segment_Tab[10]
#define  NIXIE_DISPLAYB  P3OUT = Segment_Tab[11]
#define  NIXIE_DISPLAYC  P3OUT = Segment_Tab[12]
#define  NIXIE_DISPLAYD  P3OUT = Segment_Tab[13]
#define  NIXIE_DISPLAYE  P3OUT = Segment_Tab[14]
#define  NIXIE_DISPLAYF  P3OUT = Segment_Tab[15]
/**************数码管输出数据*****************/

/*74HC573位选、段选的通断控制*/
#define SEG_SEL  P3OUT        //段选
#define BIT_SEL  P4OUT        //位选
/*#define SEG_ON    PxOUT |= BITx    //段选高电平选通  （74HC573）
#define SEG_OFF    PxOUT &= ~BITx
#define BIT_ON     PxOUT |= BITx   //位选高电平选通
#define BIT_OFF    PxOUT &= ~BITx   */

/*数码管各位低电平选通*/
#define  BIT_SEL0  P4OUT = 0xfe
#define  BIT_SEL1  P4OUT = 0xfd
#define  BIT_SEL2  P4OUT = 0xfb
#define  BIT_SEL3  P4OUT = 0xf7
#define  BIT_SEL4  P4OUT = 0xef
#define  BIT_SEL5  P4OUT = 0xdf
#define  BIT_SEL6  P4OUT = 0xbf
#define  BIT_SEL7  P4OUT = 0x7f

/*数码管输出高电平防止混乱,74HC OE、LE开启时用*/
#define  NIXIE_DISPLAY_INIT  P3OUT |= 0xff

void Nixie_Delay(void);
void Nixie_Display(unsigned char bit , unsigned char segment );// 位数据 段数据
void Nixie_All_Display(void);
#endif // __NIXIETUBE_H__
