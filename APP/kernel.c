/************************************************************************/
/* 名称：核心处理文件，可移植脱离板级实现中间层
 * 描述：后台处理，键盘处理，数值计算等等
 * 编写：邹应龙
 * 修改：2015-1-23
 * 缘由：舒华5918 主控芯片---*/
/************************************************************************/
#include "main.h"


/************************************************************************/
/* 功能：温度任务
 * 描述：报警，停止，计算
 * 形参：
 * 返回：  */
/************************************************************************/
void task_temperature(void)
{
	if(sys.ds18b20 == 0)	return;
	//3次读取比较，取
	cli();
	temperature.cur = ds18B20_get_temp();
	sei();

	if(temperature.cur >= temperature.max_stop)
	{
		for(u8 i=0; i<3; i++)
		{
			if(relay[i].enable)
			{
				relay[i].open = 0;
			}
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
	for(u8 i=0; i<3; i++)
	{
		if(relay[i].enable)
		{
			if((time[0].hour10 == relay[i].hour10) \
			&&(time[0].hour1 == relay[i].hour1) \
			&&(time[0].min10 == relay[i].min10) \
			&&(time[0].min1 == relay[i].min1) \
			&&(time[0].sec10 == 0) \
			&&(time[0].sec1 == 0))
			{
				relay[i].open = 1;
				switch(i)
				{
					case 0:PORTD |=0x01;break;
					case 1:PORTC |=0x20;break;
					case 2:PORTC |=0x10;break;
				}
			}
		}

		if(relay[i].open == 0)
		{
			switch(i)
			{
				case 0:PORTD &=~0x01;break;
				case 1:PORTC &=~0x20;break;
				case 2:PORTC &=~0x10;break;
			}
		}
	}	
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
				ext.alarm_beep = 1;
			}
		#if 0
			else if((time[0].hour10 != alarm[i].hour10) \
			|| (time[0].hour1 != alarm[i].hour1) \
			|| (time[0].min10 != alarm[i].min10) \
			|| (time[0].min1 != alarm[i].min1) \
			{
				//alarm[i].open = 0;
				ext.alarm_beep = 0;
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
	if(freq.shut>OS_MIN_10)
	{
		status_to(SLEEP);
	}
	else
	{
		if(status == SLEEP)
		{
			status_to(START);
		}
	}
}