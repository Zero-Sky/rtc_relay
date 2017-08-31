/************************************************************************
 * 名称：tm1640驱动
 * 描述：
 * 编写：邹应龙
 * 修改：2014-12-04
 ************************************************************************/

#ifndef _TM1640_H_
#define _TM1640_H_

/************************************************************************
 * 关于显存的格式：显存buf_gui[MAX]
 * 一个芯片正好16个地址，每个地址1字节。即显存大小为16*TM1640_NUM
 * 将芯片的数据线DIN进行编号，第一根数据线对应的芯片即为第一片，依次类推。
 * 第一个芯片显存为buf_gui[0~15],第二个为buf_gui[16~31]，依次类推。
 * 并口一次传入全部显存，串口一次传入16bytes显存
 ************************************************************************/

//芯片数量(数据线数目)，时钟线数目
#define TM1640_NUM        	1    
#define TM1640_CLK			1   

#define TM_CLK_H		PORTB |= 0x20
#define TM_CLK_L		PORTB &=~0x20

#define TM_DIN1_H		PORTD |= 0x20
#define TM_DIN1_L		PORTD &=~0x20

#define TM_DIN2_H		PORTB |= 0x20
#define TM_DIN2_L		PORTB &=~0x20

#define TM_DIN3_H	    PORTB |= 0x40	
#define TM_DIN3_L		PORTB &=~0x40

#define TM_DIN4_H       PORTB |= 0x80
#define TM_DIN4_L       PORTB &=~0x80

#define TM_DIN5_H       PORTB |= 0x10
#define TM_DIN5_L       PORTB &=~0x10

#define TM_DIN6_H       PORTB |= 0x20
#define TM_DIN6_L       PORTB &=~0x20

#define TM_DIN7_H       PORTB |= 0x40
#define TM_DIN7_L       PORTB &=~0x40

#define TM_DIN8_H       PORTB |= 0x80
#define TM_DIN8_L       PORTB &=~0x80

#define TM_DIN9_H       PORTB |= 0x80
#define TM_DIN9_L       PORTB &=~0x80

//初始化
void tm1640_init(void);
//显存首地址，亮度0~7
void tm1640_parallel_show(u8 *buf_gui, u8 light);
//时钟线序号，数据线序号，16bytes显存，亮度0~7
void tm1640_serial_show(u8 nclk, u8 ndat, u8 *buf_gui, u8 light);

#endif 
