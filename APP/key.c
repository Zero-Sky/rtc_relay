/************************************************************************/
/* 名称：按键扫描与处理相关
 * 描述：
 * 编写：邹应龙
 * 修改：2015-07-04
 * 缘由：舒华5517*/
/************************************************************************/

#include "main.h"

struct KEY_t
{
	s8 ok;					//有按键按下
	s8 hard;				//已结存储的上一个基于硬件的键值，用于防抖
	s8 soft;				//已结存储的上一个基于软件的键值，用于长按/短按/松开/顺序按
	s8 now;					//当前键值

	s8 press_long;			//短按键按下
	s8 press_short;			//长按键按下

#if 0
	s16	long_delay;			//长按键计时
	s8 press_double;		//多按键按下
	s8 press_release;		//释放按键按下
#endif
};
static struct KEY_t key;




void key_deal_r(u8 key_now)
{

	if (key_now == 10)
	{
		
	}
}
/************************************************************************/
/* 功能：长按键处理
 * 描述：需要200ms调用一次
 * 形参：key.prev_soft,
 * 返回：          */
/************************************************************************/

void key_deal_l(void)
{
  	if(key.press_long)
	{
		os.prompt=1;
        gbvar_set(GB_SLEEP, OS_MIN_5);
		switch(key.soft)
		{
			case 2:btn_up();break;
			case 5:btn_down();break;
			case 6:btn_right();break;
			case 4:btn_start();break;
			case 1:btn_set_input();break;
			default:break;
		}
	}
}
/************************************************************************/
/* 功能：短按键处理
 * 描述：记得根据频率调整顺序
 * 形参：
 * 返回：                  */
/************************************************************************/
static void key_deal_s(u8 key_now)
{
	os.prompt=1;
	switch(key_now)
	{
		case 1:btn_set();break;
		case 2:btn_up();break;
		case 3:btn_ext();break;
		case 4:btn_start();break;
		case 5:btn_down();break;
		case 6:btn_right();break;
		default:break;
	}	
}

/************************************************************************/
/* 功能：键值取得并处理,
 * 描述：长按/短按/松开/组合
 * 形参：本次无关硬件的软键值
 * 返回：
 * 修改：safe                             */
/************************************************************************/
void key_deal(void)
{	
	key.ok = 0;			//先清除信号
	u8 key_now = key.now;

	if((key_now == key.soft) && (key_now !=0)) 	//等于上一个键值，即按键未松开
	{
		if(gbvar_get(GB_KEY_L1) == 0) 				//是否是长按键
		{
		  	key.press_long = 1;						//长按键使能
			gbvar_set (GB_SLEEP, OS_MIN_5);
		}
	}
	else if(key_now != key.soft) 					//键值不等，包括松开(键值0)和新按键
	{
		gbvar_set(GB_KEY_L1,OS_SEC_2);
		//gbvar_set(GB_KEY_L2,OS_SEC_5);							//长按键计时关闭
		gbvar_set(GB_SLEEP, OS_MIN_5);							//待机计时关闭
		gbvar_set(GB_WAIT, OS_SEC_30);

		key.press_long = 0;								//短按键关闭长按键任务	
		if(key_now != 0) 								//不是松开
		{
			//key_deal_d(key.prev_soft,key_now);		//顺序按下的组合键调用		
			key_deal_s(key_now);						//短按键short调用
			key.soft = key_now;							//存储键值
		}
		else 								//是松开release，不保存且清除上一次键值
		{
			//deal_key_r(key.prev_soft);	//按键松开调用
			key.soft= 0;				//清除对应按键信息
		}
	}	
}

/************************************************************************/
/* 功能：按键扫描，上位函数wake_key()
 * 描述：逐次拉高法,至少25us运行一次。可能时间太慢
 * 形参：
 * 返回：                         */
/************************************************************************/
void key_get(u8 key_x,u8 key_y)
{
	u8	 	key_now = 0;						//无关硬件的本次临时键值
 	u16 	key_xy = (key_x<<8)  + key_y;		//根据硬件合并来的硬件键值
 
	switch(key_xy)
	{
		case 0x0502:key_now = 1;break;
		case 0x0504:key_now = 4;break;
		case 0x0902:key_now = 3;break;
		case 0x0904:key_now = 6;break;
		case 0x0C02:key_now = 2;break;
		case 0x0C04:key_now = 5;break;
		//case 0x0906:key_now = 20;break;
		default:key_now = 0;break;
	}

	if(key_now == key.hard)		//判断两次扫描的键值是否一致，排除抖动
	{
		key.ok = 1;					//一致，不是抖动，开始处理本次按键
		key.now = key_now;				
	}
	else
	{
		key.hard = key_now;		//不相等，则存储当前键值
	}
}

/************************************************************************/
/* 功能：按键扫描程序,位反转法，2次中断检测一次
 * 描述：X=PC0~PC3~PB4，Y=PC1~PC2
 * 形参：
 * 返回：     */
/************************************************************************/
void interrupt_key_scan(void)
{
	static s8 flag_key = 0;
	static u8 key_x = 0;
	static u8 key_y = 0;

	//先检测X的电平，X输出低，Y上拉输入。下一次检测Y的电平
	if (flag_key)
	{		
		key_x |= PINC&0x09;
		key_x |= (PINB&0x08)>>1;

		DDRC |=0x09;
		DDRB |=0x08;
		DDRC &=~0x06;
			
		PORTC &=~0x09;
		PORTB &=~0x08;
		flag_key = 0;
	} 
	//先检测Y的电平，再Y输出低，X上拉输入。下一次检测X的电平
	else
	{		
		key_y = PINC&0x06;

		DDRC &=~0x09;	
		DDRB &=~0x08;
		DDRC |=0x06;

		PORTC &=~0x06;

		key_get(key_x,key_y);
		flag_key = 1;
		key_x = 0;
		key_y = 0;
	}

}


#if 0
/************************************************************************/
/* 功能：key_scan按键扫描程序,在中断中
 * 描述：定时先读取上一个端口信息，再拉高下一个端口。循环8次完成
 * 形参：
 * 返回：     */
/************************************************************************/
void key_scan(void)  
{
	static u8 key_x,key_y,key_cnt;

//cnt的值先开始是0~7，经过一次递增后到1~8
//这时候读取cnt-1的端口值，即上一次的0~7
	key_cnt++;			
	if((PINC&0x78) !=0x00)		//有高电平
	{
		key_x |= PINC&0x78;
		key_y |=1<<(key_cnt-1);		//不能用--
	}

//若cnt>=8，改成0并调用按键处理函数
	if(key_cnt>=8)
	{
		key_get(key_x,key_y);
		key_x = 0;
		key_y = 0;
		key_cnt = 0;
	}

//拉高下一段,并调用显示函数
	hc238_disable();
	gui_output(key_cnt);
	hc238_out(1<<key_cnt); 
	hc238_enable();
}
#endif