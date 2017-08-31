#ifndef _BEEP_H_
#define _BEEP_H_
	
typedef enum 			//开启时间
{
  	BEEP_SHORT  = (u16)OS_SEC_01,			//50ms
  	BEEP_MIDDLE = (u16)OS_SEC_03,			//300ms
  	BEEP_LONG   = (u16)OS_SEC_04,  			//400ms

	BEEP_OFF_SHORT = (u16)OS_SEC_005,
	BEEP_OFF_MIDDLE = (u16)OS_SEC_01,
	BEEP_OFF_LONG = (u16)OS_SEC_03,
} BEEP_t;

void beep_reset(void);
void beep_on(BEEP_t last,BEEP_t delay,s8 times);
void beep_on_one(void);
void beep_on_long(void);
void beep_on_alarm(void);

void beep_on_open(void);
u8 is_beep_on(void);

void beep_init(void);
void interrupt_beep(void);
void task_beep(void);
#endif