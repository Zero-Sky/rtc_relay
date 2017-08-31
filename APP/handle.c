/************************************************************************/
/* 名称：按键具体处理的核心文件
 * 描述：设想中，这里的函数要么被key.c调用，要么就被本身调用，不能满足不放这里
 * 编写：邹应龙
 * 修改：2015-07-06
 * 缘由：						*/
/************************************************************************/
#include "main.h"

/************************************************************************/
/* 功能：参数设置
 * 描述：
 * 形参：1=加，0=减；带设置参数；下限；上限
 * 返回：已设置参数                            */
/************************************************************************/
static s8 value_set(u8 set, s8 value, s8 min, s8 max)
{
	if(set)		value++;
	else 		value--;
	
	if(value>max)		value = min;
	else if(value<min)	value = max;

	return value;
}
/************************************************************************/
/* 功能：继电器时，分设置
 * 描述：
 * 形参：1=加，0=减
 * 返回：                            */
/************************************************************************/
static void relay_hour_set(u8 set)
{
	s8 num = ext.relay-1;
	s8 hour = relay[num].hour10*10 + relay[num].hour1;

	hour = value_set(set, hour, 0, 23);

	relay[num].hour10 = hour/10;
	relay[num].hour1 = hour%10;
}

static void relay_min_set(u8 set)
{
	s8 num = ext.relay-1;
	s8 min = relay[num].min10*10 + relay[num].min1;

	min = value_set(set, min, 0, 59);

	relay[num].min10 = min/10;
	relay[num].min1 = min%10;
}


/************************************************************************/
/* 功能：闹钟设置
 * 描述：
 * 形参：1=加，0=减
 * 返回：                            */
/************************************************************************/
static void alarm_num_set(u8 set)
{
	s8 num = ext.alarm;

	num = value_set(set, num, 1, 9);

	ext.alarm = num;
}

static void alarm_hour_set(u8 set)
{
	s8 num = ext.alarm-1;
	s8 hour = alarm[num].hour10*10 + alarm[num].hour1;

	hour = value_set(set, hour, 0, 23);

	alarm[num].hour10 = hour/10;
	alarm[num].hour1 = hour%10;
}

static void alarm_min_set(u8 set)
{
	s8 num = ext.alarm-1;
	s8 min = alarm[num].min10*10 + alarm[num].min1;

	min = value_set(set, min, 0, 59);

	alarm[num].min10 = min/10;
	alarm[num].min1 = min%10;	
}

/************************************************************************/
/* 功能：时钟设置
 * 描述：
 * 形参：1=加，0=减
 * 返回：                            */
/************************************************************************/
static void rtc_year_set(u8 set)
{
	s8 year = time[1].year10*10 + time[1].year1;

	year = value_set(set, year, 0, 99);

	time[1].year10 = year/10;
	time[1].year1  = year%10;
}

static void rtc_month_set(u8 set)
{
	s8 month = time[1].month10*10 + time[1].month1;

	month = value_set(set, month, 1, 12);

	time[1].month10 = month/10;
	time[1].month1  = month%10;
}
//有闰月的设定，暂未实现
static void rtc_date_set(u8 set)
{
	s8 date = time[1].date10*10 + time[1].date1;

	date = value_set(set, date, 1, 31);

	time[1].date10 = date/10;
	time[1].date1  = date%10;
}

static void rtc_hour_set(u8 set)
{
	s8 hour = time[1].hour10*10 + time[1].hour1;

	hour = value_set(set, hour, 0, 23);

	time[1].hour10 = hour/10;
	time[1].hour1 = hour%10;	
}
static void rtc_min_set(u8 set)
{
	s8 min = time[1].min10*10 + time[1].min1;

	min = value_set(set, min, 0, 59);

	time[1].min10 = min/10;
	time[1].min1 = min%10;	
}
static void rtc_sec_set(u8 set)
{
	s8 sec = time[1].sec10*10 + time[1].sec1;

	sec = value_set(set, sec, 0, 59);

	time[1].sec10 = sec/10;
	time[1].sec1 = sec%10;	
}
/************************************************************************/
/* 功能：温度设置
 * 描述：
 * 形参：1=加，0=减
 * 返回：                            */
/************************************************************************/
static void temperature_set(u8 set)
{
	s16 temp = temperature.max_stop;
	if(set)		temp +=10;
	else		temp -=10;

	if(temp>1200)	temp = 0;
	else if(temp<-500)	temp = 1200;

	temperature.max_stop = temp;
}
/************************************************************************/
/* 功能：设置参数设置
 * 描述：
 * 形参：1=加，0=减
 * 返回：                            */
/************************************************************************/
static void argv_set(u8 set)
{
	freq.noflash = OS_SEC_3;
	if (status_between(RELAY1,RELAY3))
	{
		switch(ext.gui_offset)
		{
			case 4:relay_hour_set(set);break;
			case 5:relay_min_set(set);break;
			default:break;
		}
	}
	else if(status == ALARM)
	{
		switch(ext.gui_offset)
		{
			case 7:alarm_num_set(set);break;
			case 4:alarm_hour_set(set);break;
			case 5:alarm_min_set(set);break;
			default:break;
		}	
	}	
	else if (status == TEMP)
	{
		temperature_set(set);
	}
	else if (status == START)
	{
		switch(ext.gui_offset)
		{
			case 1:rtc_year_set(set);break;
			case 2:rtc_month_set(set);break;
			case 3:rtc_date_set(set);break;
			case 4:rtc_hour_set(set);break;
			case 5:rtc_min_set(set);break;
			case 6:rtc_sec_set(set);break;
			default:break;
		}
	}
}
/************************************************************************/
/* 功能：进入相关设置
 * 描述：
 * 形参：
 * 返回：                            */
/************************************************************************/
void btn_set(void)
{
	switch(status)
	{
		case START:status_to(RELAY1);ext.relay=1;break;
		case RELAY1:status_to(RELAY2);ext.relay=2;break;
		case RELAY2:status_to(RELAY3);ext.relay=3;break;
		case RELAY3:status_to(TEMP);ext.gui_offset=8;break;
		case TEMP:status_to(ALARM);ext.alarm=1;ext.gui_offset=7;break;
		case ALARM:status_to(START);break;
		default:break;
	}
}

/************************************************************************/
/* 功能：设置参数加
 * 描述：
 * 形参：
 * 返回：                            */
/************************************************************************/
void btn_up(void)
{
	argv_set(1);
}

/************************************************************************/
/* 功能：设置参数减
 * 描述：
 * 形参：
 * 返回：                            */
/************************************************************************/
void btn_down(void)
{
	argv_set(0);
}
/************************************************************************/
/* 功能：参数保存
 * 描述：
 * 形参：
 * 返回：                            */
/************************************************************************/
void btn_save(void)
{
	beep_on_one();
	if((status == START)&&(ext.gui_offset == 0))
	{
		status_to(SLEEP);
	}
	else if(status == SLEEP)
	{
		status_to(START);
	}
	else
	{
		rtc_write_time(time[1]);
		eeprom_record();
	}

	if(ext.alarm_beep)		ext.alarm_beep=0;
}

/************************************************************************/
/* 功能：启动
 * 描述：没有停止？？
 * 形参：
 * 返回：                            */
/************************************************************************/
void btn_start(void)
{
	switch(status)
	{
		case RELAY1:relay[0].enable = ~relay[0].enable;break;
		case RELAY2:relay[1].enable = ~relay[1].enable;break;
		case RELAY3:relay[2].enable = ~relay[2].enable;break;
		case ALARM:alarm[ext.alarm-1].enable=~alarm[ext.alarm-1].enable;break;
		default:break;
	}
}
/************************************************************************/
/* 功能：右移
 * 描述：
 * 形参：
 * 返回：                            */
/************************************************************************/
void btn_right(void)
{
#if 0
	for(u8 i=0; i<9; i++)
	{
		if(alarm[i].open)
			alarm[i].open = 0;
		return;
	}
#endif
	freq.noflash=0;

	if (status == START)
	{	
		if (ext.gui_offset == 0)
		{
			time[1] = time[0];		//第一次进入时复制
			ext.gui_offset++;
		}
		else if(ext.gui_offset>=6)
		{
			ext.gui_offset=0;
		}
		else
		{
			ext.gui_offset++;
		}
	}
	else if (status_between(RELAY1,RELAY3))
	{
		if (ext.gui_offset==0)
		{
			ext.gui_offset=4;
		} 
		else if (ext.gui_offset==4)
		{
			ext.gui_offset=5;
		}
		else
		{
			ext.gui_offset=0;
		}
	}
	else if(status == ALARM)
	{
		if (ext.gui_offset==7)
		{
			ext.gui_offset=4;
		}
		else if (ext.gui_offset==4)
		{
			ext.gui_offset=5;
		}
		else
		{
			ext.gui_offset=7;
		}	
	}
}

/************************************************************************/
/* 功能：熄灭屏幕
 * 描述：
 * 形参：
 * 返回：                            */
/************************************************************************/
void btn_sleep(void)
{

}