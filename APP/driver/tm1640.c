/************************************************************************
 * 名称：TM1640驱动，同时支持并口和串口
 * 简介：串口支持不同亮度和多根时钟线，并口只能单根时钟线
 * 编写：邹应龙
 * 更新：2017-8-17  
 ************************************************************************/
 #include "../main.h"

/************************************************************************
 * 亮度，8级，越低越暗                     
 ************************************************************************/
 static u8 light_led[8] = {0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F};

/************************************************************************
 * 功能：指针函数，延迟函数
 * 描述：时钟和数据
 * 形参：
 * 返回：                       
 ************************************************************************/
static void (*tm1640_clk_h[1])(void);
static void (*tm1640_clk_l[1])(void);
static void (*tm1640_din_h[TM1640_NUM])(void);
static void (*tm1640_din_l[TM1640_NUM])(void);
static void inline tm1640_delay(void)
{
	delay_us(2);
}

/************************************************************************
 * 功能：指针函数的具体实现
 * 描述：
 * 形参：
 * 返回：                        
 ************************************************************************/
static inline void u1_clk_h(void)
{
	TM_CLK_H;
}
static inline void u1_clk_l(void)
{
	TM_CLK_L;
}

///////////////////////////////////////////////
static inline void u1_din_h(void)
{
	TM_DIN1_H;
}
static inline void u1_din_l(void)
{
	TM_DIN1_L;
}

///////////////////////////////
static inline void u2_din_h(void)
{
	TM_DIN2_H;
}
static inline void u2_din_l(void)
{
	TM_DIN2_L;
}

//////////////////////////////
static inline void u3_din_h(void)
{
	TM_DIN3_H;
}
static inline void u3_din_l(void)
{
	TM_DIN3_L;
}

//////////////////////////////
static inline void u4_din_h(void)
{
	TM_DIN4_H;
}
static inline void u4_din_l(void)
{
	TM_DIN4_L;
}
#if 0
//////////////////////////////
static inline void u5_din_h(void)
{
	TM_DIN5_H;;
}
static inline void u5_din_l(void)
{
	TM_DIN5_L;
}

///////////////////////////////
static inline void u6_din_h(void)
{
	TM_DIN6_H;;
}
static inline void u6_din_l(void)
{
	TM_DIN6_L;
}

///////////////////////////////
static inline void u7_din_h(void)
{
	TM_DIN6_H;;
}
static inline void u7_din_l(void)
{
	TM_DIN6_L;
}

///////////////////////////////
static inline void u8_din_h(void)
{
	TM_DIN6_H;;
}
static inline void u8_din_l(void)
{
	TM_DIN6_L;
}

///////////////////////////////
static inline void u9_din_h(void)
{
	TM_DIN6_H;;
}
static inline void u9_din_l(void)
{
	TM_DIN6_L;
}
#endif

/************************************************************************/
/* 功能：初始化驱动,IO默认高，注意先拉高CLK，再拉高DIN
 * 描述：可能是内联的关系，没#define的函数必须#if 0，否则会有BUG
 * 形参：
 * 返回：                        */
/************************************************************************/
void tm1640_init(void)
{
	tm1640_clk_h[0] = u1_clk_h;
	tm1640_clk_l[0] = u1_clk_l;

	////////////////////////////
	tm1640_din_h[0] = u1_din_h;
	tm1640_din_l[0] = u1_din_l;
#if 0
	tm1640_din_h[1] = u2_din_h;
	tm1640_din_l[1] = u2_din_l;


	tm1640_din_h[2] = u3_din_h;
	tm1640_din_l[2] = u3_din_l;
		
	tm1640_din_h[3] = u4_din_h;
	tm1640_din_l[3] = u4_din_l;

	tm1640_din_h[4] = u5_din_h;
	tm1640_din_l[4] = u5_din_l;

	tm1640_din_h[5] = u6_din_h;
	tm1640_din_l[5] = u6_din_l;

	tm1640_din_h[6] = u7_din_h;
	tm1640_din_l[6] = u7_din_l;

	tm1640_din_h[7] = u8_din_h;
	tm1640_din_l[7] = u8_din_l;

	tm1640_din_h[8] = u9_din_h;
	tm1640_din_l[8] = u9_din_l;
#endif

	tm1640_clk_h[0]();
	for(u8 i=0; i<TM1640_NUM; i++)
		tm1640_din_h[i]();
}

/************************************************************************/
/* 功能：开始传输，并口
 * 描述：CLK为高时，DIN由高变低
 * 形参：
 * 返回：                           */
/************************************************************************/
static void tm1640_parallel_start(void)
{
	tm1640_clk_h[0]();
	tm1640_delay();

	for(u8 i=0; i<TM1640_NUM; i++)
	{
		tm1640_din_h[i]();
	}
	tm1640_delay();
	for(u8 i=0; i<TM1640_NUM; i++)
	{
		tm1640_din_l[i]();		
	}
	tm1640_delay();

	tm1640_clk_l[0]();
	tm1640_delay();
}

/************************************************************************/
/* 功能：开始传输，串口
 * 描述：CLK为高时，DIN由高变低
 * 形参：时钟线序号，数据线序号
 * 返回：                    */
/************************************************************************/
static void tm1640_serial_start(u8 nclk, u8 ndat)
{
	tm1640_clk_h[nclk]();
	tm1640_delay();
	tm1640_din_h[ndat]();
	tm1640_delay();
	tm1640_din_l[ndat]();
	tm1640_delay();
	tm1640_clk_l[nclk]();
	tm1640_delay();
}

/************************************************************************
 * 功能：结束传输,并口
 * 描述：CLK为高时，DIN由低变高
 * 形参：
 * 返回：
 ************************************************************************/
static void tm1640_parallel_stop(void)
{
	tm1640_clk_l[0]();
	tm1640_delay();

	for(u8 i=0; i<TM1640_NUM; i++)
	{
		tm1640_din_l[i]();
	}
	tm1640_delay();

	tm1640_clk_h[0]();
	tm1640_delay();

	for(u8 i=0; i<TM1640_NUM; i++)
	{
		tm1640_din_h[i]();	
	}
	tm1640_delay();
}

/************************************************************************/
/* 功能：结束传输,串口
 * 描述：CLK为高时，DIN由低变高
 * 形参：
 * 返回：
 * 修改：                             */
/************************************************************************/
static void tm1640_serial_stop(u8 nclk, u8 ndat)
{
	tm1640_clk_l[nclk]();
	tm1640_delay();

	tm1640_din_l[ndat]();
	tm1640_delay();

	tm1640_clk_h[nclk]();
	tm1640_delay();
	
	tm1640_din_h[ndat]();		//最后拉高锁住
	tm1640_delay();
}

/************************************************************************
 * 功能：不同芯片写入*不同的*数据
 * 描述：data格式是TM1[0~15]--TM2[0~15]--TMn[0~15]
 * 描述：这里需要改成{TM1[0]-TM2[0]...} -- {TM1[1]-TM2[1]...}
 * 形参：芯片内地址的偏移量，数据全局数组
 * 返回：                 
 ************************************************************************/
static void tm1640_parallel_data(u8 addr, u8 *data)
{
	u8 i,j,buf_data[TM1640_NUM];

	//获取本次发送的数据，共TM1640_NUM个，规则见描述
	for(i=0; i<TM1640_NUM; i++)
	{
		buf_data[i] = data[16*i + addr];
	}

	for (j=0; j<8; j++)
	{
		tm1640_clk_l[0]();			//数据改变必须先拉低
		tm1640_delay();

		for(i=0; i<TM1640_NUM; i++)
		{
			if(buf_data[i]&0x01)	//LSB
				tm1640_din_h[i]();
			else
				tm1640_din_l[i]();

			buf_data[i] >>=1;
		}
		tm1640_delay();

		tm1640_clk_h[0]();
		tm1640_delay();

		tm1640_clk_l[0]();			
		tm1640_delay();
	}
}

/************************************************************************
 * 功能：不同芯片写入相同的数据。一般用于命令模式
 * 描述：
 * 形参：数据
 * 返回：                
 ************************************************************************/
static void tm1640_parallel_cmd(u8 data)
{
	u8 i,j;
	for (j=0; j<8; j++)
	{
		tm1640_clk_l[0]();
		tm1640_delay();

		for(i=0; i<TM1640_NUM; i++)
		{
			if(data&0x01)
				tm1640_din_h[i]();
			else
				tm1640_din_l[i]();	
		}
		data >>=1;
		tm1640_delay();

		tm1640_clk_h[0]();
		tm1640_delay();

		tm1640_clk_l[0]();
		tm1640_delay();
	}
}

/************************************************************************/
/* 功能：写入显存数据，地址自动加1，多个芯片同时写
 * 描述：开始-设置数据-结束 开始-设置地址-显示数据1-显示数据n-结束 开始-控制显示-结束
 * BUG:发现连续发送时，每个芯片的第一个数据总是有误，因此最后固定地址发送一次。
 * 形参：传输的数据缓存
 * 返回：    */
/************************************************************************/
void tm1640_parallel_show(u8 *buf_gui, u8 light)
{
	tm1640_parallel_start();
	tm1640_parallel_cmd(0x40);		//地址自动加1模式	
	tm1640_parallel_stop();

	tm1640_parallel_start();
	tm1640_parallel_cmd(0xC0);		//设置写入地址0xC0+addr，这里永远从第0个地址开始

	for (u8 i=0; i<16; i++)	        //每个芯片16个地址
	{
		tm1640_parallel_data(i,buf_gui);
	}

	tm1640_parallel_stop();

	tm1640_parallel_start();
	tm1640_parallel_cmd(light_led[light]);		//调节亮度
	tm1640_parallel_stop();
}


/************************************************************************
 * 功能：写入1，串口
 * 描述：
 * 形参：时钟线序号，数据线序号
 * 返回：               
 ************************************************************************/
static void tm1640_serial_1(u8 nclk, u8 ndat)
{
	tm1640_clk_l[nclk]();
	tm1640_delay();

	tm1640_din_h[ndat]();
	tm1640_delay();

	tm1640_clk_h[nclk]();
	tm1640_delay();

	tm1640_clk_l[nclk]();
	tm1640_delay();
}
/************************************************************************
 * 功能：写入0
 * 描述：
 * 形参：时钟线序号，数据线序号
 * 返回：
 ************************************************************************/
static void tm1640_serial_0(u8 nclk, u8 ndat)
{
	tm1640_clk_l[nclk]();
	tm1640_delay();

	tm1640_din_l[ndat]();
	tm1640_delay();

	tm1640_clk_h[nclk]();
	tm1640_delay();

	tm1640_clk_l[nclk]();
	tm1640_delay();
}

/************************************************************************
 * 功能：写入8个数据,串口
 * 描述：LSB first
 * 形参：时钟线序号，数据线序号，一字节数据
 * 返回：                          
 ************************************************************************/
static void tm1640_serial_byte(u8 nclk, u8 ndat, u8 data)
{
	for (u8 i=0;i<8;i++)
	{
		if (data&0x01)
			tm1640_serial_1(nclk, ndat);
		else
			tm1640_serial_0(nclk, ndat);
		data >>= 1;
	}
}

/************************************************************************
 * 功能：连续写16字节显存数据，地址自动加1
 * 描述：开始-设置数据-结束 开始-设置地址-显示数据1-显示数据n-结束 开始-控制显示-结束
 * 形参：16字节数据数组地址，亮度
 * 返回：
 ************************************************************************/
void tm1640_serial_show(u8 nclk, u8 ndat, u8 *buf_gui, u8 light)
{
	tm1640_serial_start(nclk, ndat);
	tm1640_serial_byte(nclk, ndat, 0x40);		//0100 0000地址自动加1	
	tm1640_serial_stop(nclk, ndat);

	tm1640_serial_start(nclk, ndat);
	tm1640_serial_byte(nclk, ndat,0xC0);		//1100 XXXX设置地址数据，第一个地址
	for (u8 i=0; i<16; i++)	        //连续输入数据
	{
		tm1640_serial_byte(nclk, ndat, buf_gui[i]);
	}

	tm1640_serial_stop(nclk, ndat);

	tm1640_serial_start(nclk, ndat);
	tm1640_serial_byte(nclk, ndat,light_led[light]);		//调节亮度
	tm1640_serial_stop(nclk, ndat);
}

