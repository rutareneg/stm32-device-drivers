#include <stm32f10x.h>
#include "delay.h"
//Mini STM32С—Р„В·СћВ°Рµ
//Рљв„–РЈР“SysTickВµР”Р–РҐРќРЃС�Р–РљСЌР”Р€РљР…В¶Р¤РЎРЈС–Р©Р…С€Р Р в„–Р¬РђРЅ
//В°СЊРђРЃdelay_us,delay_ms
	 
static u8  fac_us=0;//usРЎРЈРљВ±В±В¶С–Р›РљСЌ
static u16 fac_ms=0;//msРЎРЈРљВ±В±В¶С–Р›РљСЌ

//С–С…РљС�В»Р‡РЎРЈС–Р©С”Р‡РљСЌ
//SYSTICKВµР”РљВ±Р¦РЈв„–РњВ¶РЃРћР„HCLKРљВ±Р¦РЈВµР”1/8
//SYSCLK:РџВµРќС–РљВ±Р¦РЈ
void delay(vu32 nCount)		 //?Рё?В¤????for?РЅ??nCount??РіРј?СЏ??????????????
{
  for(; nCount != 0; nCount--);
}

void delay_init(u8 SYSCLK)
{
	SysTick->CTRL&=0xfffffffb;//bit2Р—РµС—РҐ,РЎРЋР¤СЃРќРІР†С—РљВ±Р¦РЈ  HCLK/8
	fac_us = SYSCLK/8;
	fac_ms = (u16)fac_us*1000;
}								    
//РЎРЈРљВ±nms
//Р§СћРўРІnmsВµР”В·В¶РћВ§
//SysTick->LOADРћР„24РћВ»С�Р”Т‘Р¶Р–С‡,Р›С‰РўР¤,Р§РѕТ‘СѓРЎРЈРљВ±РћР„:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLKВµТђРћВ»РћР„Hz,nmsВµТђРћВ»РћР„ms
//В¶Р¤72MРњС…С�СЋРџР’,nms<=1864

void delay_ms(u32 nms)
{	 		  	  
	SysTick->LOAD = nms*fac_ms;//РљВ±С�РґС�РЈР¤РЁ(SysTick->LOADРћР„24bit)
	load_cek ();
}   
//****************************************************//
void delay_us(u32 nus)
{		
	SysTick->LOAD=nus*fac_us; //РљВ±С�РґС�РЈР¤РЁ
	load_cek ();
}
//*************************************************//
void delay_s(u32 nms)
{
	while(nms)
	{delay_ms(1000);nms--;}
}
//**************************************************//
void load_cek ()
{	u32 temp;
		SysTick->VAL=0x00;        //Р—РµС—РҐС�Р–РљСЌР–С‡
		SysTick->CTRL=0x01 ;      //С—Р„РљС�Вµв„–РљСЌ
		do
		{
			temp = SysTick->CTRL;
		}
		while(temp&0x01&&!(temp&(1<<16)));//ВµР�Т‘СЌРљВ±С�РґВµР…Т‘Рї
		SysTick->CTRL=0x00;       //в„–РЁВ±РҐС�Р–РљСЌР–С‡
		SysTick->VAL =0X00;       //Р—РµС—РҐС�Р–РљСЌР–С‡

}



















