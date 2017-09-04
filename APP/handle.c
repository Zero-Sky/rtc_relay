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
 * 形参：设置0号还是1号时间，1=加，0=减
 * 返回：                            */
/************************************************************************/
static void relay_hour_set(u8 offset,u8 set)
{
	s8 num = gbvar_get(GB_RELAY)-1;
	s8 hour = relay[num].time[offset].hour10*10 + relay[num].time[offset].hour1;

	hour = value_set(set, hour, 0, 23);

	relay[num].time[offset].hour10 = hour/10;
	relay[num].time[offset].hour1 = hour%10;
}

static void relay_min_set(u8 offset, u8 set)
{
	s8 num = gbvar_get(GB_RELAY)-1;
	s8 min = relay[num].time[offset].min10*10 + relay[num].time[offset].min1;

	min = value_set(set, min, 0, 59);

	relay[num].time[offset].min10 = min/10;
	relay[num].time[offset].min1 = min%10;
}
/************************************************************************/
/* 功能：继电器时温度报警
 * 描述：温度为0时不会工作
 * 形参：1=加，0=减
 * 返回：                            */
/************************************************************************/
static void relay_temp_set(u8 set)
{
	s8 num = gbvar_get(GB_RELAY)-1;
	s16 value = relay[num].temperature;

	if(set)		value +=10;
	else 		value -=10;
	
	if(value>1200)		value = -300;
	else if(value<-300)	value = 1200;

	relay[num].temperature = value;
}
/************************************************************************/
/* 功能：闹钟设置
 * 描述：
 * 形参：1=加，0=减
 * 返回：                            */
/************************************************************************/
static void alarm_num_set(u8 set)
{
	s8 num = gbvar_get(GB_ALARM);

	num = value_set(set, num, 1, 9);

	gbvar_set(GB_ALARM,num);
}

static void alarm_hour_set(u8 set)
{
	s8 num = gbvar_get(GB_ALARM)-1;
	s8 hour = alarm[num].hour10*10 + alarm[num].hour1;

	hour = value_set(set, hour, 0, 23);

	alarm[num].hour10 = hour/10;
	alarm[num].hour1 = hour%10;
}

static void alarm_min_set(u8 set)
{
	s8 num = gbvar_get(GB_ALARM)-1;
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
	gbvar_set(GB_NOFLASH, OS_SEC_3);
	u16 offset = gbvar_get(GB_GUI_OFFSET);
	if (status_between(RELAY1,RELAY3))
	{
		switch(offset)
		{
			case 1:relay_hour_set(0,set);break;
			case 2:relay_min_set(0,set);break;
			case 4:relay_hour_set(1,set);break;
			case 5:relay_min_set(1,set);break;
			case 8:relay_temp_set(set);break;
			default:break;
		}
	}
	else if(status == ALARM)
	{
		switch(offset)
		{
			case 8:alarm_num_set(set);break;
			case 4:alarm_hour_set(set);break;
			case 5:alarm_min_set(set);break;
			default:break;
		}	
	}	
	else if (status == TEMP)
	{
		temperature_set(set);
	}
	else if (status == CLOCK)
	{
		switch(offset)
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
	if(gbvar_get(GB_GUI_OFFSET) == 0)
		beep_on_alarm();
	else if(gbvar_get(GB_KEY_L1))
		beep_on_one();
}
/************************************************************************/
/* 功能：进入相关设置
 * 描述：
 * 形参：
 * 返回：                            */
/************************************************************************/
void btn_set(void)
{
	if(status == SHOW)		return;

	switch(status)
	{
		case CLOCK:status_to(RELAY1);gbvar_set(GB_RELAY,1);break;
		case RELAY1:status_to(RELAY2);gbvar_set(GB_RELAY,2);break;
		case RELAY2:status_to(RELAY3);gbvar_set(GB_RELAY,3);break;
	#if 0
		case RELAY3:status_to(TEMP);gbvar_set(GB_GUI_OFFSET,8);break;
		case TEMP:status_to(ALARM);gbvar_set(GB_ALARM,1);gbvar_set(GB_GUI_OFFSET,8);break;
	#endif
		case RELAY3:status_to(ALARM);gbvar_set(GB_ALARM,1);gbvar_set(GB_GUI_OFFSET,8);break;
		case ALARM:status_to(SHOW);break;
		default:break;
	}
	beep_on_one();
}

/************************************************************************/
/* 功能：设置参数加
 * 描述：
 * 形参：
 * 返回：                            */
/************************************************************************/
void btn_up(void)
{
	if(status == SHOW)		return;
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
	if(status == SHOW)		return;
	argv_set(0);
}
/************************************************************************/
/* 功能：额外功能
 * 描述：
 * 形参：
 * 返回：                            */
/************************************************************************/
void btn_ext(void)
{
	if(gbvar_get(GB_ALARM_OPEN))		gbvar_clr(GB_ALARM_OPEN);
	else
	{
		beep_on_one();
		if((status == SHOW) )
			status_to(SLEEP);
		else if(status == SLEEP)
			status_to(SHOW);
	}
}

/************************************************************************/
/* 功能：启动
 * 描述：没有停止？？
 * 形参：
 * 返回：                            */
/************************************************************************/
void btn_start(void)
{
	if(status == SHOW)		return;
	u16 offset = gbvar_get(GB_ALARM) - 1;
	switch(status)
	{
		case RELAY1:relay[0].enable = ~relay[0].enable;break;
		case RELAY2:relay[1].enable = ~relay[1].enable;break;
		case RELAY3:relay[2].enable = ~relay[2].enable;break;
		case ALARM:alarm[offset].enable=~alarm[offset].enable;break;
		default:break;
	}
	beep_on_one();
}
/************************************************************************/
/* 功能：右移
 * 描述：
 * 形参：
 * 返回：                            */
/************************************************************************/
void btn_right(void)
{
	if(status == SHOW)		return;

	gbvar_clr(GB_NOFLASH);

	u16 offset = gbvar_get(GB_GUI_OFFSET);
	if (status == CLOCK)
	{	
		if (offset == 0)
		{
			offset++;
		}
		else if(offset>=6)
		{
			offset=0;
		}
		else
		{
			offset++;
		}
	}
	else if (status_between(RELAY1,RELAY3))
	{
		if(offset == 0)			offset=1;
		else if(offset == 1)	offset=2;
		else if(offset == 2)	offset=4;
		else if(offset == 4)	offset=5;
		else if(offset == 5)	offset=8;
		else 					offset=0;
	}
	else if(status == ALARM)
	{
		if (offset==8)
		{
			offset=4;
		}
		else if (offset==4)
		{
			offset=5;
		}
		else
		{
			offset=8;
		}	
	}
	gbvar_set(GB_GUI_OFFSET,offset);
	beep_on_one();
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
/************************************************************************/
/* 功能：进入设置界面
 * 描述：
 * 形参：
 * 返回：                            */
/************************************************************************/
void btn_set_input(void)
{
	if(status == SHOW)
	{
		status_to(CLOCK);
		beep_on_one();
		time[1] = time[0];		//第一次进入时复制
	}
}