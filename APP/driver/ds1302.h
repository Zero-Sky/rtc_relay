/************************************************************************
 * 名称：DS1302驱动
 * 描述：
 * 编写：邹应龙
 * 修改：2014-12-04
 ************************************************************************/

#ifndef _DS1302_H_
#define _DS1302_H_

#define DS1302_SCLK_H	PORTB |=0x01
#define DS1302_SCLK_L	PORTB &=~0x01

#define DS1302_IO_H		PORTB |=0x02
#define DS1302_IO_L		PORTB &=~0x02

#define DS1302_IO_R		PINB&0x02

#define DS1302_IO_IN	DDRB &=~0x02
#define DS1302_IO_OUT	DDRB |=0x02

#define DS1302_RST_H	PORTB |=0x04
#define DS1302_RST_L	PORTB &=~0x04

//RAM的开始两个字节作为标识符，判断芯片是否已被初始化
#define DS1302_INITA		0x68
#define DS1302_INITB 		0x20

//闹钟，必须在主芯片上电时才可用
//数据保存在RAM的第3/4/5字节
//DS1302断电后需重新设置
typedef struct
{
	u8 		hour;
	u8 		min;
	u8 		sec;
}RTC_alarm_t;

void ds1302_w_data(u8 cmd, u8 ten, u8 unit);
void ds1302_r_data(u8 cmd, vu8 *ten, vu8 *unit);
void rtc_write_time(RTC_t data); 
RTC_t rtc_read_time(void);
void ds1302_init(void);
void ds1302_debug(void);

#endif 
