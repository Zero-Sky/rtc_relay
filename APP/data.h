/************************************************************************/
/* 名称：动态数据/全局变量
 * 描述：这里的数据和宏定义尽量不改，能改的放main.h中
 * 编写：邹应龙
 * 修改：2014-12-04
 * 缘由：						*/
/************************************************************************/
#ifndef _DATA_H_
#define _DATA_H_

//时分设置结构体
typedef struct
{
	vu8			min1;		
	vu8			min10;
	vu8			hour1;
	vu8			hour10;
}HM_t;


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
	//vu8			day;			//周几，bit计数,bit0~6代表礼拜日~礼拜6
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

//继电器控制，6组，12，34,56分别对应123继电器
typedef struct
{
	vu8 			enable;			//=1，使能
	vu8  			open;			//=1,继电器开
	HM_t			time[2];		//0是开启时间，1是关闭时间	
	vs16			temperature;	//高于该温度停止，=0时无效,-128~127
}RELAY_t;


/*************************************************************************
 * 运行状态，系统根据运行状态来决定行为
 *************************************************************************/
typedef enum  
{
	SHOW,			//没有设置时，正常显示
	CLOCK,			//时钟设置
	RELAY1,			//继电器,内部3个
	RELAY2,
	RELAY3,
	TEMP,				//温度
	ALARM,				//闹钟
	SLEEP				//熄灭屏幕
}STATUS_t;

/************************************************************************/
/* status的便捷操作函数 */
/************************************************************************/
int status_between(STATUS_t smaller, STATUS_t bigger);
void status_to(STATUS_t sta);


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
extern RELAY_t 				relay[6];

extern   	  STATUS_t		status;

extern struct OS_t			os;

#endif
