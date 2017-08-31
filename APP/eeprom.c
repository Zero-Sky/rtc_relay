/************************************************************************/
/* 名称：EEPROM相关文件
 * 描述：
 * 编写：邹应龙
 * 修改：2015-3-21
 * 缘由：舒华5918*/
/************************************************************************/

#include "main.h"
/************************************************************************/
/* 功能：三组继电器时钟   无使能状态，即上电后继电器不会自行启动 */
/************************************************************************/
EEMEM u8 mem_eep_relay[12] = {	
							1,9,4,8,		//十位时，个位时，十位分，个位分
							1,2,3,4,		//第二组
							0,5,3,6
};

/************************************************************************/
/* 功能：温度    */
/************************************************************************/

EEMEM u8 mem_eep_temp[] = {0,0};

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
		relay[i].hour10 = eeprom_read_byte(addr+4*i+0);
		relay[i].hour1  = eeprom_read_byte(addr+4*i+1);
		relay[i].min10 = eeprom_read_byte(addr+4*i+2);
		relay[i].min1  = eeprom_read_byte(addr+4*i+3);
	}

	addr = mem_eep_temp;
	temperature.max_stop = (u16)(eeprom_read_byte(addr+0)<<8) + eeprom_read_byte(addr+1);

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
		eeprom_write_byte(addr+4*i+0, relay[i].hour10);
		eeprom_write_byte(addr+4*i+1, relay[i].hour1);
		eeprom_write_byte(addr+4*i+2, relay[i].min10);
		eeprom_write_byte(addr+4*i+3, relay[i].min1);
	}

	addr = mem_eep_temp;
	eeprom_write_byte(addr, temperature.max_stop>>8);
	eeprom_write_byte(addr+1, temperature.max_stop);

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
