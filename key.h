
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include<stm32f10x_rcc.h>
#include "stm32f10x_conf.h"



 #define KEY_1  GPIO_ReadInputDataBit(PORT_KEY,PIN_KEY_1)
 #define KEY_2  GPIO_ReadInputDataBit(PORT_KEY,PIN_KEY_2)
 #define KEY_3  GPIO_ReadInputDataBit(PORT_KEY,PIN_KEY_3)
 #define KEY_4  GPIO_ReadInputDataBit(PORT_KEY,PIN_KEY_4)
 #define KEY_D	GPIO_ResetBits(PORT_KEY,PIN_OUT1)



	u8 keys;  //значение нажатой кнопки
	void key_sk();
	void key_st(u8 T);//время удержания
	void key_init();
	u8   key();
	void key_ini();

	void blinc_init();
	void blinc (u16 t);
	void BIP(int p,int t);
	void BipP(int p,int t);
	void BipStop();
	void BipExit();
