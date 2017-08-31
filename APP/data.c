/************************************************************************/
/* 名称：关键变量
 * 描述：关键的变量
 * 编写：邹应龙
 * 修改：2015-6-24
 * 缘由：舒华5517*/
/************************************************************************/
#include "main.h"

//需要计算的数据
RTC_t				time[2];
ALARM_t 			alarm[MAX_ALARM];
TEMP_t 				temperature;
RELAY_t 			relay[3];

//状态变量
STATUS_t			status;

//其他结构体
struct EVENT_t		event;
struct FREQ_t		freq;
struct SYS_t		sys;
struct EXT_t		ext;
struct OS_t			os;

/************************************************************************/
/* 功能：比较当前status是否属于两个status之间
 * 描述：暂停状态清速度不清其他。再按一次到停止清空
 * 形参：小的在前，大的在后。反过来不对，为了精简代码量
 * 返回：              */
/************************************************************************/
int status_between(STATUS_t smaller, STATUS_t bigger)
{
	if( (status >= smaller)&&(status <= bigger) )
		return 1;
	else
		return 0;	
}
/************************************************************************/
/* 功能：切换status状态显示
 * 描述：有些切换要做一些显示或数据的改变
 * 形参：
 * 返回：              */
/************************************************************************/
void status_to(STATUS_t sta)
{
	status = sta;
	ext.relay=0;
	ext.alarm=0;
	ext.gui_offset=0;	  	
}
