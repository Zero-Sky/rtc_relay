/************************************************************************/
/* 名称：核心处理头文件
 * 描述：
 * 编写：邹应龙
 * 修改：2014-12-25
 * 缘由：*/
/************************************************************************/
#ifndef _KERNEL_H_
#define _KERNEL_H_

void task_rtc(void);
void task_relay(void);
void task_temperature(void);
void task_alarm(void);
void task_shut(void);
#endif
