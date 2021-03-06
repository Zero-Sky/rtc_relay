/************************************************************************/
/* 名称：核心处理文件，可移植脱离板级实现中间层
 * 描述：后台处理，键盘处理，数值计算等等
 * 编写：邹应龙
 * 修改：2015-1-23
 * 缘由：舒华5918 主控芯片---*/
/************************************************************************/
#include "main.h"


/************************************************************************/
/* 功能：启动关闭继电器及其LED
 * 描述：
 * 形参：继电器组数；1=开启，0=停止
 * 返回：  */
/************************************************************************/
static void relay_switch(u8 num, u8 open)
{
	if (open)
	{
		switch(num)
		{
			case 0:PORTD|=0x01;break;
			case 1:PORTC|=0x20;break;
			case 2:PORTC|=0x10;break;
		}
	}
	else
	{
		switch(num)
		{
			case 0:PORTD &=~0x01;break;
			case 1:PORTC &=~0x20;break;
			case 2:PORTC &=~0x10;break;		
		}
	}
}

/************************************************************************/
/* 功能：继电器任务
 * 描述：开启，停止
 * 形参：
 * 返回：  */
/************************************************************************/
void task_relay(void)
{
	u8 tmp_beep=0;
	for(u8 i=0; i<3; i++)
	{
		if(relay[i].enable)
		{
			if((time[0].hour10 == relay[i].time[0].hour10) \
			&&(time[0].hour1 == relay[i].time[0].hour1) \
			&&(time[0].min10 == relay[i].time[0].min10) \
			&&(time[0].min1 == relay[i].time[0].min1) \
			&&(time[0].sec10 == 0) \
			&&(time[0].sec1 == 0))
			{
				relay[i].open = 1;
				tmp_beep=1;
			}
			else if((time[0].hour10 == relay[i].time[1].hour10) \
			&&(time[0].hour1 == relay[i].time[1].hour1) \
			&&(time[0].min10 == relay[i].time[1].min10) \
			&&(time[0].min1 == relay[i].time[1].min1) \
			&&(time[0].sec10 == 0) \
			&&(time[0].sec1 == 0))
			{
				relay[i].open = 0;
				relay[i].enable = 0;
				tmp_beep=1;
			}
			//温度设置不为0时有效
			else if ((temperature.cur >= relay[i].temperature)&&(relay[i].temperature))
			{
				relay[i].open = 0;
				relay[i].enable = 0;
				tmp_beep=1;
			}	
		}
		else
		{
			relay[i].open = 0;
		}
		relay_switch(i, relay[i].open);
		if (tmp_beep)
		{
			beep_on_long();
		}
	}
}

/************************************************************************/
/* 功能：温度任务
 * 描述：报警，停止，计算
 * 形参：
 * 返回：  */
/************************************************************************/
void task_temperature(void)
{
	if(gbvar_get(GB_DS18B20) == 0)	return;
	//3次读取比较，取
	cli();
	temperature.cur = ds18B20_get_temp();
	sei();
}

/************************************************************************/
/* 功能：时间任务
 * 描述：读取时间
 * 形参：
 * 返回：  */
/************************************************************************/
void task_rtc(void)
{
	time[0] = rtc_read_time();
}

/************************************************************************/
/* 功能：闹钟任务
 * 描述：
 * 形参：
 * 返回：  */
/************************************************************************/
void task_alarm(void)
{
	for(u8 i=0; i<9; i++)
	{
		if(alarm[i].enable)	
		{
			if((time[0].hour10 == alarm[i].hour10) \
			&&(time[0].hour1 == alarm[i].hour1) \
			&&(time[0].min10 == alarm[i].min10) \
			&&(time[0].min1 == alarm[i].min1) \
			&&(time[0].sec10 == 0) \
			&&(time[0].sec1 == 0)
			)
			{
				//alarm[i].open = 1;
				gbvar_increase(GB_ALARM_OPEN);
			}
		#if 0
			else if((time[0].hour10 != alarm[i].hour10) \
			|| (time[0].hour1 != alarm[i].hour1) \
			|| (time[0].min10 != alarm[i].min10) \
			|| (time[0].min1 != alarm[i].min1) \
			{
				//alarm[i].open = 0;
			}
		#endif
		}
	}	
}
/************************************************************************/
/* 功能：熄灭屏幕
 * 描述：
 * 形参：
 * 返回：  */
/************************************************************************/
void task_shut(void)
{
	if((gbvar_get(GB_SLEEP) == 0)&&(status == SHOW))
	{
		status_to(SLEEP);
	}
	else if ((gbvar_get(GB_WAIT)==0) &&(status != SHOW))
	{
		status_to(SHOW);
	}
}