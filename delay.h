#ifndef __DELAY_H
#define __DELAY_H 			   
#include <stm32f10x.h>
//Mini STM32开发板
//使用SysTick的普通计数模式对延迟进行管理
//包括delay_us,delay_ms
void delay_init(u8 SYSCLK);
void delay_ms(u32 nms);
void delay_us(u32 nus);
void delay_s(u32 nms);

void delay(vu32 nCount);
void load_cek ();
#endif





























