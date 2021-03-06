/************************************************************************/
/* 名称：存储的静态用户数据
 * 描述：显示数据，预定义速度数据等
 * 编写：邹应龙
 * 修改：2015-1-24
 * 缘由：舒华5918*/
/************************************************************************/
#include "main.h"
								
/************************************************************************
 * 名称：led 八字形数码管数据
 * mem_led是标准模式下的数据，其格式是Dp-G-F-E--D-C-B-A
 * 转换小程序在LittleTools中 
 ************************************************************************/

PROGMEM const u8 mem_led_num[]=	{	
							0x3F,				//0
                      		0x06,				//1
                     		0x5B,				//2
                      		0x4F,				//3
                      		0x66,				//4
                     		0x6D,				//5-S
                      		0x7D,				//6
                      		0x07,				//7
                     		0x7F,				//8
                    		0x6F,				//9
							
							0x77,				//A--10						
							0x71, 				//F--11
							0x79,				//E--12
							0x73,				//P--13
							0x3E,				//U--14
							0x50, 				//r--15
							0x40,				//---16
							0x37,				//N--17
					  		};

