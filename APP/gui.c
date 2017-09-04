/************************************************************************/
/* 名称：GUI显示模块
 * 简介：
 * 编写：邹应龙
 * 更新：2016-3-24                                              */
/************************************************************************/
#include "main.h"


/************************************************************************/
/*动态显存                                                 */
/************************************************************************/
static u8 	buf_gui[GUI_MAX];				//显存

/************************************************************************/
/* 显示部分的基础函数，主要用于被其他函数调用            */
/************************************************************************/

/************************************************************************/
/* 功能：数字拆分成数组(最大四位)
 * 描述：传入123，则arr[2]=3,arr[1]=2,arr[0]=1
 * 形参：数组长度/需要拆分的位数，要拆分的数字，传回参数的数组
 * 返回：无    */
/************************************************************************/
static void num_to_array(s8 cnt, s16 num, u8 *arr)
{
	u8 digit[4];
	digit[3] = num/1000;
	digit[2] = num/100%10;
	digit[1]  = num/10%10;
	digit[0] = num%10;

	for(u8 i=0; i<cnt; i++)
	{
		arr[i] = digit[i];
	}
} 


/************************************************************************/
/* 功能：LED/点阵显示缓存初始化
 * 描述：根据具体情况来使用，按键可能也需要使用
 * 形参：
 * 返回：无    */
/************************************************************************/
void gui_led_all_clr(void)
{
	for (u8 i=0; i<GUI_MAX; i++)
	{
		buf_gui[i]  = 0;
	}
}

/************************************************************************/
/* 以下是用户显示级函数，主要涉及裸露的显示部分              */
/************************************************************************/
/************************************************************************/
/* 功能：显存驱动输出
 * 描述：
 * 形参：
 * 返回：                  */
/************************************************************************/
void gui_update(void)
{
	tm1640_parallel_show(buf_gui,0);
}
/************************************************************************/
/* 功能：数码管年，月，日
 * 描述：
 * 形参：十位，个位
 * 返回：                  */
/************************************************************************/
static void gui_led_year(u8 year10, u8 year1)
{
	buf_gui[3] = pgm_read_byte(&mem_led_num[year10]);
	buf_gui[2] = pgm_read_byte(&mem_led_num[year1]);
	if(year10==0)		buf_gui[3] = 0;	
}
static void gui_led_month(u8 month10, u8 month1)
{
	buf_gui[5] = pgm_read_byte(&mem_led_num[month10]);
	buf_gui[4] = pgm_read_byte(&mem_led_num[month1]);
	if(month10==0)	buf_gui[5] = 0;
}
static void gui_led_date(u8 date10, u8 date1)
{
	buf_gui[10] = pgm_read_byte(&mem_led_num[date10]);
	buf_gui[9] = pgm_read_byte(&mem_led_num[date1]);		
	if(date10==0)		buf_gui[10] = 0;
}

/************************************************************************/
/* 功能：数码管时，分，秒
 * 描述：
 * 形参：
 * 返回：                  */
/************************************************************************/
static void gui_led_hour(u8 hour10, u8 hour1)
{
	buf_gui[1] = pgm_read_byte(&mem_led_num[hour10]);
	buf_gui[0] = pgm_read_byte(&mem_led_num[hour1]);
	if(hour10==0)		buf_gui[1] = 0;
}

static void gui_led_min(u8 min10, u8 min1)
{
	buf_gui[6] = pgm_read_byte(&mem_led_num[min10]);
	buf_gui[7] = pgm_read_byte(&mem_led_num[min1]);
	if(min10==0)		buf_gui[6] = 0;
}
static void gui_led_sec(u8 sec10, u8 sec1)
{
	buf_gui[8] = pgm_read_byte(&mem_led_num[sec10]);
	buf_gui[14] = pgm_read_byte(&mem_led_num[sec1]);
	if(sec10==0)		buf_gui[8] = 0;
}
/************************************************************************/
/* 功能：当前处于运行态/停止态
 * 描述：
 * 形参：
 * 返回：                  */
/************************************************************************/
static void gui_led_run(void)
{
#if 0
	static u8 offset=0;
	for (u8 i=2; i<=5; i++)
	{
		buf_gui[i] = 1<<offset;
	}
	buf_gui[9] = 1<<offset;
	buf_gui[10] = 1<<offset;

	offset++;
	if(offset>5)	offset=0;
#endif
	buf_gui[8] = pgm_read_byte(&mem_led_num[0]);
	buf_gui[14] = pgm_read_byte(&mem_led_num[17]);
}
static void gui_led_stop(void)
{
#if 0
	for (u8 i=2; i<=5; i++)
	{
		buf_gui[i] = 0x40;
	}
	buf_gui[9] = 0x40;
	buf_gui[10] = 0x40;
#endif
	buf_gui[8] = pgm_read_byte(&mem_led_num[0]);
	buf_gui[14] = pgm_read_byte(&mem_led_num[11]);
}
/************************************************************************/
/* 功能：闪烁对应位
 * 描述：
 * 形参：1=年，2=月，3=日，4=时，5=分，6=秒，7=序号，8=温度
 * 返回：                  */
/************************************************************************/
static void gui_led_flash(u8 offset)
{
	if (gbvar_get(GB_FLASH) > OS_SEC_05)	//bug 不能小于？？
	{
		switch(offset)
		{
			case 1:buf_gui[3]=buf_gui[2]=0;break;
			case 2:buf_gui[5]=buf_gui[4]=0;break;
			case 3:buf_gui[10]=buf_gui[9]=0;break;
			case 4:buf_gui[1]=buf_gui[0]=0;break;
			case 5:buf_gui[6]=buf_gui[7]=0;break;
			case 6:buf_gui[8]=buf_gui[14]=0;break;
			case 7:buf_gui[15]=0;break;
			case 8:buf_gui[11]=buf_gui[12]=buf_gui[13]=0;break;
			default:break;
		}
	}
}
/************************************************************************/
/* 功能：显示设置序号
 * 描述：
 * 形参：
 * 返回：                  */
/************************************************************************/
static void gui_led_num(u8 dat)
{
	buf_gui[15] = pgm_read_byte(&mem_led_num[dat]);
}

/************************************************************************/
/* 功能：显示温度
 * 描述：范围-9.9~99.9度
 * 形参：
 * 返回：                  */
/************************************************************************/
static void gui_led_temperature(vs16 data)
{
	u8 arr[3],negative,point;

	//测量范围-55~+125，单位0.1
	if ((data<-550) || (data>1250))
		return;

	//判断正负和小数点
	if(data>=0)
	{	
		negative=0;
		if(data>=1000)	
		{
			point = 0;	//无小数点,且抛弃末位
			data = data/10;
		}
		else
		{
			point = 1;
		}
	}
	else
	{
		negative=1;
		data = -1*data;
		if(data>=100)	
		{
			point =0;
			data = data/10;
		}
		else
		{
			point = 1;
		}
	}

	num_to_array(3,data,arr);
	buf_gui[11] = pgm_read_byte(&mem_led_num[arr[2]]);
	buf_gui[12] = pgm_read_byte(&mem_led_num[arr[1]]);
	buf_gui[13] = pgm_read_byte(&mem_led_num[arr[0]]);

	if(point)	buf_gui[12] += POINT_NUM;
	else		buf_gui[13] += POINT_NUM;

	if(arr[2] == 0)
	{
		buf_gui[11] = 0;
		if(arr[1] == 0)		buf_gui[12] = 0;
	}

	if(negative)	buf_gui[11] = pgm_read_byte(&mem_led_num[16]);		

	if (status == TEMP)
	{
		gui_led_run();
		gui_led_num(4);
		buf_gui[0]=buf_gui[1]=buf_gui[6]=buf_gui[7]=buf_gui[8]=buf_gui[14]=0x40;
	}
}


/************************************************************************/
/* 功能：显示当前真实时间
 * 描述：来自RTC
 * 形参：
 * 返回：                  */
/************************************************************************/
static void gui_led_rtc(void)
{
	gui_led_year(time[0].year10, time[0].year1);
	gui_led_month(time[0].month10, time[0].month1);
	gui_led_date(time[0].date10, time[0].date1);
	gui_led_hour(time[0].hour10,time[0].hour1);
	gui_led_min(time[0].min10, time[0].min1);
	gui_led_sec(time[0].sec10, time[0].sec1);	
}
/************************************************************************/
/* 功能：显示设置时间
 * 描述：来自RTC
 * 形参：
 * 返回：                  */
/************************************************************************/
static void gui_led_clock(void)
{
	gui_led_year(time[1].year10, time[1].year1);
	gui_led_month(time[1].month10, time[1].month1);
	gui_led_date(time[1].date10, time[1].date1);
	gui_led_hour(time[1].hour10,time[1].hour1);
	gui_led_min(time[1].min10, time[1].min1);
	gui_led_sec(time[1].sec10, time[1].sec1);	
	gui_led_num(0);
}
/************************************************************************/
/* 功能：显示闹钟
 * 描述：年显示序号，A亮表示开启，D亮表示关闭
 * 形参：
 * 返回：                  */
/************************************************************************/
static void gui_led_alarm(void)
{
	s8 offset = gbvar_get(GB_ALARM)-1;
	if(offset <0)		return;

	

	if (alarm[offset].enable)		gui_led_run();		
	else							gui_led_stop();

	gui_led_year(time[0].year10, time[0].year1);
	gui_led_month(time[0].month10, time[0].month1);
	gui_led_date(time[0].date10, time[0].date1);

	gui_led_hour(alarm[offset].hour10,alarm[offset].hour1);
	gui_led_min(alarm[offset].min10, alarm[offset].min1);

	//闹钟序号，1~9
	buf_gui[13] = pgm_read_byte(&mem_led_num[offset+1]);

	gui_led_num(4);
}

/************************************************************************/
/* 功能：显示继电器
 * 描述：
 * 形参：
 * 返回：                  */
/************************************************************************/
static void gui_led_relay(void)
{
	vu8 offset = gbvar_get(GB_RELAY)-1;
	if(offset<0)		return;

	gui_led_num(offset+1);
	gui_led_year(relay[offset].time[0].hour10,relay[offset].time[0].hour1);
	gui_led_month(relay[offset].time[0].min10, relay[offset].time[0].min1);	
	gui_led_hour(relay[offset].time[1].hour10,relay[offset].time[1].hour1);
	gui_led_min(relay[offset].time[1].min10, relay[offset].time[1].min1);
	gui_led_temperature(relay[offset].temperature);

	if(relay[offset].enable)	gui_led_run();
	else						gui_led_stop();
}
static void gui_led_relay2(void)
{
	if (relay[0].enable)	PORTD&=~0x02;
	else					PORTD|=0x02;
	if (relay[1].enable)	PORTD&=~0x04;
	else					PORTD|=0x04;
	if (relay[2].enable)	PORTD&=~0x08;
	else					PORTD|=0x08;
	
}

/************************************************************************/
/* 功能：DEBUG
 * 描述：温度处显示
 * 形参：
 * 返回：                  */
/************************************************************************/
static void gui_led_debug(void)
{
	u8 arr[3];
	num_to_array(3,gbvar_get(GB_DEBUG),arr);
	buf_gui[11] = pgm_read_byte(&mem_led_num[arr[2]]);
	buf_gui[12] = pgm_read_byte(&mem_led_num[arr[1]]);
	buf_gui[13] = pgm_read_byte(&mem_led_num[arr[0]]);
}





/************************************************************************/
/* 功能：数码管显示数字
 * 描述：包括米字管显示数字，没有秒点，1秒一次
 * 形参：
 * 返回：                  */
/************************************************************************/
void gui_led(void)
{
	gui_led_all_clr();
	
	switch (status)
	{
		case CLOCK:gui_led_clock();break;
		case RELAY1:
		case RELAY2:
		case RELAY3:gui_led_relay();break;
		case TEMP:gui_led_temperature(temperature.max_stop);break;
		case ALARM:gui_led_alarm();break;
		case SLEEP:break;
		default:gui_led_rtc();gui_led_temperature(temperature.cur);break;
	}
	gui_led_relay2();
	if(gbvar_get(GB_NOFLASH) == 0)
		gui_led_flash(gbvar_get(GB_GUI_OFFSET));
	gui_update();
}

/************************************************************************/
/* 功能：LCD全显，全暗可用gui_grid_all_clr
 * 描述：
 * 形参：
 * 返回：                  */
/************************************************************************/
void gui_all_show(u8 set)
{
 	s8 i=0;
	u16 data;
	if(set)		data = 0xFF;
	else		data = 0;
	
	for(i=0; i<GUI_MAX; i++)
	{
		buf_gui[i] = data;
	}	
	gui_update();
}
