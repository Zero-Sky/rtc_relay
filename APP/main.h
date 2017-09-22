/************************************************************************/
/* 名称：全局头文件
 * 简介：
 * 编写：邹应龙
 * 更新：2016-11-23                                              */
/************************************************************************/
#ifndef MAIN_H_
#define MAIN_H_

/*********类型*************************************************/
typedef unsigned char		u8;
typedef unsigned int		u16;		//AVR的int是16位
typedef unsigned long		u32;
typedef signed char			s8;
typedef signed int			s16;
typedef signed long			s32;

typedef volatile unsigned char		vu8;
typedef volatile unsigned int		vu16;		//AVR的int是16位
typedef volatile unsigned long		vu32;
typedef volatile signed char		vs8;
typedef volatile signed int			vs16;
typedef volatile signed long		vs32;
/*********************************************************************/
/* 多字节分拆成单字节
 * union/struct固定是从低地址放到高地址。则对于数据0x76543210
 *  低地址---------------------------------高地址
 *  cbuf[0]  |   cbuf[1]  |   cbuf[2]  |  cbuf[3]
 *   0x10    |    0x32    |   0x54     |   0x76     ---小端，低地址是低位
 *   0x76    |    0x54    |   0x32     |   0x10     ---大端，低地址是高位
 *		sbuf[0]		|	sbuf[1]
 *		0x3210      |	0x7654		--little endian
 *		0x7654		|	0x3210 		--big endian
 * AVR 是小端
 * STM8 是大端
 * STM32 是小端
 * 网络TCP/IP传输时，是大端------------------*/
 /*********************************************************************/
s32 is_big_endian(void);	//检查大小端模式

typedef union
{
	u8		 	cbuf[4];
	float		fbuf;
} FLOA_INT;

typedef union
{
	u8  		cbuf[2];
	u16 		ibuf;
} U16_INT;

typedef union		
{
  	u8 			cbuf[4];
	u16			sbuf[2];
	u32		 	ibuf;
} U32_INT;

typedef union
{
	u8			cbuf[2];
	s16 		ibuf;
} S16_INT;

typedef union
{
  	u8 			cbuf[4];
	u16			sbuf[2];
	s32 		ibuf;
} S32_INT;



/************************************************************************/
/* 预定义设置，一般是默认设置                                  */
/************************************************************************/
#define INIT_EEP			0x62		//初始化标志，若初始化值等于该值，则读eeprom数据

#define MAX_ALARM 		10			//支持多少个闹钟

#define DELAY_KEY			OS_SEC_3	//长按键3s后，进入长按键模式
#define DELAY_KEY_2			OS_SEC_5    //长按键5s后，进入十位递增递减模式


/************************************************************************/
/* 名称：时间宏定义，OS_TICK_MS表示一次滴答的时间 */
/************************************************************************/
#define	OS_TICK_MS				10			//主时钟interrupt_systick的中断间隔
#define OS_SEC_004				(40		/OS_TICK_MS)
#define OS_SEC_005				(50		/OS_TICK_MS)
#define OS_SEC_008				(80		/OS_TICK_MS)
#define OS_SEC_01				(100	/OS_TICK_MS)		//0.1s=100ms,10个滴答
#define	OS_SEC_02				(200	/OS_TICK_MS)		//0.2s
#define OS_SEC_018				(180	/OS_TICK_MS)
#define OS_SEC_025				(250	/OS_TICK_MS)		//250ms
#define OS_SEC_03				(300	/OS_TICK_MS)
#define OS_SEC_04 				(400	/OS_TICK_MS)
#define OS_SEC_05				(500	/OS_TICK_MS)		//半秒
#define OS_SEC_1				(1000	/OS_TICK_MS)
#define OS_SEC_2				(2000	/OS_TICK_MS)
#define OS_SEC_3				(3000	/OS_TICK_MS)
#define OS_SEC_5				(5000   /OS_TICK_MS)
#define OS_SEC_6				(6000   /OS_TICK_MS)
#define OS_SEC_8				(8000   /OS_TICK_MS)
#define OS_SEC_10				(10000	/OS_TICK_MS)
#define OS_SEC_15				(15000	/OS_TICK_MS)
#define OS_SEC_20				(20000	/OS_TICK_MS)
#define OS_SEC_30				(30000	/OS_TICK_MS)
#define OS_MIN_1				(60000	/OS_TICK_MS)
#define OS_MIN_2				(120000	/OS_TICK_MS)
#define OS_MIN_5				(300000	/OS_TICK_MS)
#define OS_MIN_10				(600000	/OS_TICK_MS)
#define OS_MIN_30				(1800000	/OS_TICK_MS)
/************************************************************************/
/* 头文件                                 */
/************************************************************************/
#define F_CPU	8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/boot.h>

/************************************************************************/
/* 个人头文件                                 */
/************************************************************************/
#include "data.h"
#include "os/gbval.h"
#include "driver/tm1640.h"
#include "driver/ds1302.h"
#include "driver/ds18b20.h"
#include "driver/beep.h"
#include "board.h"
#include "eeprom.h"
#include "handle.h"
#include "key.h"
#include "kernel.h"
#include "gui.h"
#include "storage.h"

/************************************************************************/
/* main中的函数                              */
/************************************************************************/
void interrupt_systick(void);
void interrupt_os(void);

#endif /* MAIN_H_ */