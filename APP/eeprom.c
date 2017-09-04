/************************************************************************/
/* 名称：EEPROM相关文件
 * 描述：
 * 编写：邹应龙
 * 修改：2015-3-21
 * 缘由：舒华5918*/
/************************************************************************/

#include "main.h"
/************************************************************************/
/* 功能：每组继电器有8个u8时间，1个s16温度，共10字节 */
/************************************************************************/
EEMEM u8 mem_eep_relay[30] = {0};

/************************************************************************/
/* 功能：温度，3组温度，对应三组继电器    */
/************************************************************************/

EEMEM u8 mem_eep_temp[3] = {0};

/************************************************************************/
/* 功能：9组闹钟  十个时，十个分，使能状态  */
/************************************************************************/
EEMEM u8 mem_eep_alarm[45] = {0};
/************************************************************************/
/* 功能：eeprom初始化,读取eeprom的值，存入到对应的地方
 * 描述：上电读，修改时不需要，因为修改时是先修改系统值再修改eeprom的值
 * 形参：
 * 返回：     */
/************************************************************************/
void  eeprom_init(void)
{
	u8 *addr; 
	u8 i;
	addr = mem_eep_relay;
	for(i=0; i<3; i++)
	{
		relay[i].time[0].hour10 = eeprom_read_byte(addr+8*i+0);
		relay[i].time[0].hour1  = eeprom_read_byte(addr+8*i+1);
		relay[i].time[0].min10  = eeprom_read_byte(addr+8*i+2);
		relay[i].time[0].min1   = eeprom_read_byte(addr+8*i+3);

		relay[i].time[1].hour10 = eeprom_read_byte(addr+8*i+4);
		relay[i].time[1].hour1  = eeprom_read_byte(addr+8*i+5);
		relay[i].time[1].min10  = eeprom_read_byte(addr+8*i+6);
		relay[i].time[1].min1   = eeprom_read_byte(addr+8*i+7);

		relay[i].temperature = (s16)(eeprom_read_byte(addr+8*i+8)<<8) + eeprom_read_byte(addr+8*i+9);
	}

	//addr = mem_eep_temp;
	//temperature.max_stop = (u16)(eeprom_read_byte(addr+0)<<8) + eeprom_read_byte(addr+1);

	addr = mem_eep_alarm;
	for(i=0; i<9; i++)
	{
		alarm[i].hour10 = eeprom_read_byte(addr + 4*i + 0);
		alarm[i].hour1  = eeprom_read_byte(addr + 4*i + 1);
		alarm[i].min10  = eeprom_read_byte(addr + 4*i + 2);
		alarm[i].min1   = eeprom_read_byte(addr + 4*i + 3);
		alarm[i].enable = eeprom_read_byte(addr + 4*i + 4);
	}
}

/************************************************************************/
/* 功能：eeprom记录
 * 描述：
 * 形参：
 * 返回：     */
/************************************************************************/
void eeprom_record(void)
{
	u8 *addr,i;
	addr = mem_eep_relay;
	for(i=0; i<3; i++)
	{
		eeprom_write_byte(addr+8*i+0, relay[i].time[0].hour10);
		eeprom_write_byte(addr+8*i+1, relay[i].time[0].hour1);
		eeprom_write_byte(addr+8*i+2, relay[i].time[0].min10);
		eeprom_write_byte(addr+8*i+3, relay[i].time[0].min1);

		eeprom_write_byte(addr+8*i+4, relay[i].time[1].hour10);
		eeprom_write_byte(addr+8*i+5, relay[i].time[1].hour1);
		eeprom_write_byte(addr+8*i+6, relay[i].time[1].min10);
		eeprom_write_byte(addr+8*i+7, relay[i].time[1].min1);

		eeprom_write_byte(addr+8*i+8, relay[i].temperature>>8);
		eeprom_write_byte(addr+8*i+8, relay[i].temperature);
	}

//	addr = mem_eep_temp;
//	eeprom_write_byte(addr, temperature.max_stop>>8);
//	eeprom_write_byte(addr+1, temperature.max_stop);

	addr = mem_eep_alarm;
	for(i=0; i<9; i++)
	{
		eeprom_write_byte(addr+4*i+0, alarm[i].hour10);
		eeprom_write_byte(addr+4*i+1, alarm[i].hour1);
		eeprom_write_byte(addr+4*i+2, alarm[i].min10);
		eeprom_write_byte(addr+4*i+3, alarm[i].min1);
		eeprom_write_byte(addr+4*i+4, alarm[i].enable);
	}		
} 
