#include "main.h"

/************************************************************************/
/* 功能：任务
 * 描述：AVR的OS调度有问题，只能这样做，不过看起来容量小了很多
 * 形参：
 * 返回：         */
/************************************************************************/
void task_50ms(void)
{
	if(os.ms50 == 0)
	{
		os.ms50 = OS_SEC_005;
		task_beep();
	}
}
void task_100ms(void)
{
	if(os.ms100 == 0)
	{
		os.ms100 = OS_SEC_01;
	}
}

void task_250ms(void)
{
	if(os.ms250 == 0)
	{
		os.ms250 = OS_SEC_025;
		key_deal_l();
	}
}
void task_500ms(void)
{
	if(os.ms500 == 0)
	{
		os.ms500 = OS_SEC_05;
		task_rtc();
		task_temperature();
		task_relay();
		task_alarm();	
		if(gbvar_get(GB_FLASH) == 0)
			gbvar_set(GB_FLASH, OS_SEC_1);		
		gui_led();
	}
}

void task_1s(void)
{
	if(os.sec1 == 0)
	{
		os.sec1 = OS_SEC_1;						
	}
}

void task_5s(void)
{
	if(os.sec5 == 0)
	{
		os.sec5 = OS_SEC_5;
	}
}
//时间动态变化的
void task_dynamic(void)
{
	if(os.prompt)		//立刻显示
	{
	  	os.prompt = 0;	
		gui_led();
	}
}


/************************************************************************/
/* 功能：系统初始化
 * 描述：跟硬件/系统/任务无关，用于参数初始化
 * 形参：
 * 返回：         */
/************************************************************************/
void sys_init(void)
{	
	tm1640_init();
	beep_init();
	while(os.jiffies<OS_SEC_05);
	gbvar_init();
	u8 tmp = ds18b20_init();
	gbvar_set(GB_DS18B20,tmp);
	eeprom_init();
	ds1302_init();
	status = SHOW;
}

int main(void)
{
	board_init();
	sys_init();
	while(1)
	{	
		key_deal();
		task_500ms();
		task_250ms();
		task_50ms();
		task_dynamic();
	}
}

/************************************************************************/
/* 功能：定时时各种变量处理
 * 描述：同os的定时器一起处理
 * 形参：
 * 返回：          */
/************************************************************************/
void interrupt_systick(void)
{
	gbvar_tmr();
}
/************************************************************************/
/* 功能：定时时各种变量处理
 * 描述：同os的定时器一起处理
 * 形参：
 * 返回：          */
/************************************************************************/
void interrupt_os(void)
{
	os.jiffies++;
	if (os.ms50)		os.ms50--;
	if (os.ms100)		os.ms100--;
	if (os.ms250)		os.ms250--;
	if (os.ms500)		os.ms500--;
	if (os.sec1)		os.sec1--;
	if (os.sec5)		os.sec5--;
}

/************************************************************************/
/* 功能：判断CPU的大小端模式
 * 描述：
 * 形参：
 * 返回：大端=1，小端=0          */
/************************************************************************/
s32 is_big_endian(void)
{
	U32_INT data;
	data.ibuf=0x76543210;
	if(data.cbuf[0] == 0x10)
		return 0;   			//小端,低地址是低位
	else
		return 1;				//大端，低地址是高位
}