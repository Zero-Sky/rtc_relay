/************************************************************************/
/* 名称：动态数据/全局变量
 * 描述：这里的数据和宏定义尽量不改，能改的放main.h中
 * 编写：邹应龙
 * 修改：2014-12-04
 * 缘由：						*/
/************************************************************************/
#ifndef _DATA_H_
#define _DATA_H_


//实时时钟，不支持12/24小时
//因为ds1302采用BCD存储，所以这里也改成BCD
typedef struct
{
	vu8			sec1;
	vu8			sec10;
	vu8			min1;
	vu8			min10;
	vu8			hour1;
	vu8			hour10;
	vu8 		date1;
	vu8 		date10;			//日期
	vu8			month1;
	vu8			month10;
	vu8			day;			//周几，bit计数,bit0~6代表礼拜日~礼拜6
	vu8			year1;
	vu8			year10;
}RTC_t;

//闹钟
typedef struct
{
	vu8 		enable;			//使能
	vu8			open;			//开闭状态
	vu8			min1;
	vu8			min10;
	vu8			hour1;
	vu8			hour10;
	vu8 		date;			//日期
	///////////////////////////////
}ALARM_t;




//温度
typedef struct
{
	vs16		cur;				//纠正后的当前温度
	vs16 		value[3];				//当前温度，冗余3个
	vs16 		min_stop;			//低于该值停止
	vs16 		max_stop;			//高于该值停止
	vs16 		min;				//最小显示值
	vs16		max;				//最大显示值
}TEMP_t;

//继电器控制，3组
typedef struct
{
	vu8 		enable;		//=1，使能
	vu8  		open;		//=1,继电器开
	vu8			min1;		//何时打开
	vu8			min10;
	vu8			hour1;
	vu8			hour10;	
}RELAY_t;


/*************************************************************************
 * 运行状态，系统根据运行状态来决定行为
 *************************************************************************/
typedef enum  
{
	START,			//没有设置时
	RELAY1,			//继电器,内部3个
	RELAY2,
	RELAY3,
	TEMP,				//温度
	ALARM,				//闹钟
	SLEEP
}STATUS_t;

/************************************************************************/
/* status的便捷操作函数 */
/************************************************************************/
int status_between(STATUS_t smaller, STATUS_t bigger);
void status_to(STATUS_t sta);

/************************************************************************/
/* 系统参数，用于保存系统默认参数等等                                   */
/************************************************************************/
struct SYS_t
{
	u8 	ds18b20;				//ds18b20,=0不存在，=1存在
};


/************************************************************************/
/* 事件，用于和中断的通讯，不包括定时                                  */
/************************************************************************/
struct EVENT_t
{	
	volatile u8 motor_ok;		//完整的接收到一次完整的下控变频器/调速器数据
	vu32 		mbox[2];		//64个消息队列		
};

/************************************************************************/
/* 通讯，和中断无关的全局变量                                 */
/************************************************************************/
struct EXT_t
{
	vs16		debug;
	vs16 		gui_offset;				//当前GUI偏移	
	vu8			alarm;					//当前闹钟序号,1开始 
	vu8			alarm_beep;				//蜂鸣器闹钟	
	vu8			relay;					//当前继电器序号，1开始		
	vu8			save;					//数据有修改，需要保存到eeprom	
};

/************************************************************************/
/* 定时，用于计算时间用的。                              */
/************************************************************************/
struct FREQ_t
{
	volatile u8 		flash;				//只用于半秒闪烁，但不是所有的半秒闪烁
	volatile u16 		noflash;			//按键按下后对应设置界面保存一段时间不闪烁"NOFLASH"s

	volatile u32		shut;				//关闭计时，也用于启动时的计时

	volatile u16		delay_beep;			//蜂鸣器计时
	volatile u16 		delay_key;			//按键长按定时

};

/************************************************************************/
/* 用于模拟OS任务的定时变量                              */
/************************************************************************/
struct OS_t
{
	volatile u8			prompt;			//立即调用，但是不会破坏原来的定时规则
	volatile u8 		ms50;
	volatile u8			ms100;
	volatile u16 		ms250;
	volatile u16		ms500;
	volatile u16		sec1;
	volatile u16		sec5;
	volatile u32		jiffies;		//启动后到现在的滴答时间
};


/************************************************************************/
/* 名称：已定义的结构体说明 */
/************************************************************************/
extern RTC_t				time[2];		//0是当前，1是设置
extern ALARM_t 				alarm[MAX_ALARM];
extern TEMP_t 				temperature;
extern RELAY_t 				relay[3];

extern   	  STATUS_t		status;

extern struct EVENT_t  		event;
extern struct FREQ_t 		freq;
extern struct SYS_t			sys;
extern struct EXT_t 		ext;
extern struct OS_t			os;

#endif
