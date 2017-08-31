/************************************************************************
 * 名称：tm1640驱动
 * 描述：
 * 编写：邹应龙
 * 修改：2014-12-04
 ************************************************************************/

#ifndef _DS18B20_H_
#define _DS18B20_H_

//IO方向设置
#define DS18B20_IO_IN	DDRD &=~0x80
#define DS18B20_IO_OUT	DDRD |=0x80

////IO操作函数											   
#define	DS18B20_IO_H	PORTD |=0x80
#define	DS18B20_IO_L	PORTD &=~0x80

//读IO数据
#define DS18B20_IO_R	(PIND&0x80)

   	
u8 ds18b20_init(void);//初始化DS18B20
s16 ds18B20_get_temp(void);//获取温度

#endif 
