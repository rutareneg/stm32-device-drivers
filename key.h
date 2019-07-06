
#include <stm32f10x.h>

	u16 level_ill;//уровень €ркости

 #define KEY_1  GPIO_ReadInputDataBit(PORT_KEY,PIN_KEY_1)
 #define KEY_2  GPIO_ReadInputDataBit(PORT_KEY,PIN_KEY_2)
 #define KEY_3  GPIO_ReadInputDataBit(PORT_KEY,PIN_KEY_3)
 #define KEY_4  GPIO_ReadInputDataBit(PORT_KEY,PIN_KEY_4)
 #define KEY_D	GPIO_ResetBits(PORT_KEY,PIN_OUT1)

	void  init_in_out();

	void NVIC_EXTI_Configuration(void);
	//void  EXTI0_IRQHandler(void);

	u8 keys;  //значение нажатой кнопки
	void key_sk();
	void key_st(u8 T);//врем€ удержани€
	void key_init();
	u8   key();
	void key_ini();

	void blinc_init();
	void blinc (u16 t);

	void BIP_init();
	void BIP(int p,int t);
	void BipP(int p,int t);
	void BipStop();
	void BipExit();
	void ILL_init();
	 void level (u8 level_ill);
	 void ILL_ON();
	 void ILL_OFF();
