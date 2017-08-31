/************************************************************************/
/* 名称：全局头文件
 * 简介：
 * 编写：邹应龙
 * 更新：2016-1-19                                              */
/************************************************************************/


#ifndef _GUI_H_
#define _GUI_H_

//CNT表示mbi5024级联的数量
//MAX代表缓存数量，是mbi5024级联数量*16
#define	GUI_CNT		1
#define GUI_MAX		16

#define POINT_NUM	0x80					//八字管小数点

void gui_update(void);
void gui_led(void);
void gui_all_show(u8 set);
void gui_led_all_clr(void);

#endif /* GUI_H_ */