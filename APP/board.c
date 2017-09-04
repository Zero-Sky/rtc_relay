/************************************************************************/
/* 名称：主芯片所有硬件相关函数
 * 描述：所有的初始化，所有的中断函数
 * 编写：邹应龙
 * 修改：2016-03-26*/
/************************************************************************/
#include "main.h"


/************************************************************************/
/* 功能：gpio初始化
 * 描述：
 * 形参：
 * 返回：                               */
/************************************************************************/
static void gpio_init(void)
{
	//RTC PB2/B1/B0 输出
	DDRB |=0x07;

	//TM1640 PB5/D5 OUT
	DDRB |=0x20;
	DDRD |=0x20;

	//KEY X=PC0~PC3~PB4，Y=PC1~PC2
	DDRC |=0x09;
	DDRB |=0x10;
	
	DDRC &=~0x06;
	
	//BEEP
	DDRD |=1<<6;
	
	//DS18B20 PD7
	DDRD |=1<<7; 

	//继电器PC4 PC5 PD0 PD1 PD2 PD3
	DDRC |=0x30;
	DDRD |=0x0F;

	PORTD|=0x08; PORTD &=~0x01;
	PORTD|=0x04; PORTC &=~0x20;
	PORTD|=0x02; PORTC &=~0x10;
}

/************************************************************************/
/* 功能：主时钟T1=10ms，次时钟T2=125us用于蜂鸣器，按键扫描有T0=2ms
 * 描述：优先级T2>T1>T0，不可改变
 * 描述：16M下，一次计数时间为1/16us(分频1)，1/2us(8),4us(64),16us(256),64us(1024)
 * 形参：
 * 返回：                               */
/************************************************************************/
static void tim_init(void)
{	
    //10ms，用于主时钟
    TCCR1A	= 0;					//CTC模式，计数到达OCR1A时溢出
    TCCR1B	= 0x0B;					//64分频 8M晶振下一次计数8us
    TCNT1	= 0;					
    OCR1A	= 1250;					//即10ms时产生输出比较A中断
    TIMSK  |= 0x10;					//T1输出比较中断

    //125us，用于蜂鸣器，优先级最高
    TCCR2	= 0x0A;					//CTC模式，8分频 8M下一次1us
    TCNT2	= 0;					//
    OCR2	= 125;					//250次*0.5us = 125us,即250us一周期，4k
	TIMSK  |= 0x80;					//T2 A通道比较匹配溢出
}
#if 0
/************************************************************************/
/* 功能：外部中断，不改变中断向量表位置 
 * 描述：
 * 形参：
 * 返回：                               */
/************************************************************************/
static void exti_init(void)
{	
	//SAFE INT0 常闭，平时低电平，上升沿表示激活，下降沿表示恢复。都检测
	//HRC INT1 平时高电平，下降沿低电平表示一个脉冲。仅检测下降沿

	//使能INT0/INT1

}

/************************************************************************/
/* 功能：UART0用于IAP和下控，UART1用于蓝牙
 * 描述：4800，2停止位，奇校验
 * 形参：
 * 返回：                               */
/************************************************************************/
static void uart_init(void)
{			
	//设置波特率，1200波特率，16M/16*BAUD =416=0x1A0,417=1A1
#if 0
  	//9600 = 51=0x33
	//UBRR0H =0X00;
	//UBRR0L =0X33;
	//1200
	UBRR0H = 0x01;
	UBRR0L = 0xA0;
	//
	UBRR0H = 0x01;
	UBRR0L = 0xA0;
	//接收/发送中断使能,接收/发送使能，数据位8
	UCSR0B = (1<<7)|(1<<6)|(1<<4)|(1<<3);
	//UCSR0B =(1<<7)|(1<<4)|(1<<3);
	//UCSR0B = 0xD8;
	//设置帧格式，8个数据位，2个停止位，奇校验
	UCSR0C = (1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<1);
//	UCSR0C = (1<<5)|(1<<3)|(1<<2)|(1<<1);
	//UCSR0C = 0x36;


	UCSR0B	= 0xD8;			//发送接收使能，发送接收中断使能
	UCSR0C	= 0x3E;			//异步，8数据位，2停止位，奇校验
	UBRR0	= 832;			//1200波特率，普通速度模式，16M/(16*BAUD)-1
#endif
}

/************************************************************************/
/* 功能：SPI模式。
 * 描述：主输出，16bit，无接收。时钟稳定低，上升沿检测数据。
 * 描述：上升时间<500ns，时钟稳定时间>20ns，数据稳定>3ns，先发MSB
 * 描述：这里选时钟周期是1us,1M。AVR只能8bit传输
 * 形参：
 * 返回：                               */
/************************************************************************/
static void spi_init(void)
{
	SPCR	= 0x51;				//主模式，第一个上升沿检测，1M时钟，中断关
}
/************************************************************************/
/* 功能：开门狗设置
 * 描述：超时复位，1s
 * 形参：
 * 返回：                               */
/************************************************************************/
static void wdt_init(void)
{
//	WDTCSR = 0x18;		//启动开门狗，4个clk内写入数据
//	WDTCSR = 0x16;		//1S
}
#endif
/************************************************************************/
/* 功能：各种外设初始化，硬件密切相关的
 * 描述：通过亮灯暗示客户不能操作
 * 形参：无
 * 返回：
 * 修改：                             */
/************************************************************************/
void board_init(void)
{
	cli();					//关闭中断
    gpio_init();
	tim_init();
	//uart_init();
	//exti_init();
	//spi_init();
	//wdt_init();
	sei();	
}

/************************************************************************/
/* 功能：us级延时。
 * 描述：因出栈入栈的时间，实际延时会比参数长2~3us
 * 形参：需要延时的微秒
 * 返回：                    */
/************************************************************************/

void delay_us(u8 dat)
{ 
	for(u8 i=0; i<5; i++);
}
void delay_ms(u8 nms)
{
	for(u16 i=0; i<(nms*10); i++)
	  	delay_us(2);
}


/************************************************************************/
/* 功能：10ms,用于主时钟
 * 描述：
 * 形参：
 * 返回：     */
/************************************************************************/
ISR(TIMER1_COMPA_vect)
{
  	sei();
	interrupt_systick();
	interrupt_os();
	interrupt_key_scan();
}

/************************************************************************/
/* 功能：125us，用于蜂鸣器
 * 描述：可用
 * 形参：
 * 返回：     */
/************************************************************************/   
ISR(TIMER2_COMP_vect)
{
	interrupt_beep();
}


/************************************************************************/
/* 功能：外部中断处理
 * 描述：
 * 形参：
 * 返回：     */
/************************************************************************/
ISR(INT0_vect) 
{

}

ISR(INT1_vect)
{

}

/************************************************************************/
/* 功能：SPI主模式发送
 * 描述：显示驱动中调用，不能用中断
 * 形参：
 * 返回：     */
/************************************************************************/
void spi_master_transmit(u8 data)
{
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));	
}