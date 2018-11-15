#include "d_7_led.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include<stm32f10x_rcc.h>
#include "stm32f10x_conf.h"
#include "delay.h"
#include "stm32f10x_tim.h"
#include "main.h"


//#define ILL_max  20//максимально значение яркости

	u16  M[] = {M0, M1, M2, M3, M4, M5, M6, M7, M8, M9};//массив с числами
	u16 _MR[]= {_MR1,_MR2,_MR3,_MR4};//разряды экрана
	u16 _MRF[]= {_MR1,_MR2,_MR3,_MR4,_MR5,_MR6};//разряды   экрана


//настройка портов контролёра для сетодиодного индикктаор..........///////
	//*****************************************************//

void Init_LED_7S()
{
	RAZ = 0; // разряд индикатора
	 REG = 0;// контроль номера массива
	 ILLR = 0;//отсчёт прерываний яркости экрана

	 ILL0 =20;//ILL_max;//сюда установаливается текущая яркость
	 ILL1 =20;//сюда установаливается 2я яркость
	PORT_LED_OUT();//настройка порта для LED7S тип 036+
	Init_key_ILL();//настройка порта для управленияя яркостью
	//Init_Timer3 ();//настройка прирывания для переключения между разрядами
	Init_Timer2 ();//настройка прирывания для переключения между разрядами
	}

//#define  In_ILL0  if(ILL0 >= ILL_max)ILL0 = ILL_max//ограничиваем первую яркост
//#define  In_ILL1  if(ILL1 >= ILL_max)ILL1 = ILL_max// ограничиваем втрую яркость
void Init_LED_7SF()
{
	RAZ = 0; // разряд индикатора
	 REG = 0;// контроль номера массива
	 ILLR = 0;//отсчёт прерываний яркости экрана

	 ILL0 =20;//ILL_max;//сюда установаливается текущая яркость
	 ILL1 =20;//сюда установаливается 2я яркость
	PORT_LED_OUTF();//настройка порта для LED7S тип 036+
	Init_key_ILL();//настройка порта для управленияя яркостью
	//Init_Timer3 ();//настройка прирывания для переключения между разрядами
	Init_Timer2 ();//настройка прирывания для переключения между разрядами
	}




void PORT_LED_OUT()
{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
   GPIO_InitTypeDef PORT_d_7_led;
   PORT_d_7_led.GPIO_Pin = _A|_B|_C|_D|_E|_F|_G|_MR1|_MR2|_MR3|_MR4|_TN;
   PORT_d_7_led.GPIO_Speed = GPIO_Speed_50MHz;
   PORT_d_7_led.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_Init(PORT_LED, &PORT_d_7_led);

   // Disable JTAG for release LED PIN
      RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
   	   AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
}

void PORT_LED_OUTF()
{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
   GPIO_InitTypeDef PORT_d_7_led;
   PORT_d_7_led.GPIO_Pin = _A|_B|_C|_D|_E|_F|_G|_TN;
   PORT_d_7_led.GPIO_Speed = GPIO_Speed_50MHz;
   PORT_d_7_led.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_Init(PORT_LED, &PORT_d_7_led);

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
      GPIO_InitTypeDef PORT_d_7_ledF;
      PORT_d_7_ledF.GPIO_Pin = _MR1|_MR2|_MR3|_MR4|_MR5|_MR6;
      PORT_d_7_ledF.GPIO_Speed = GPIO_Speed_50MHz;
      PORT_d_7_ledF.GPIO_Mode = GPIO_Mode_Out_PP;
      GPIO_Init(PORT_LEDF, &PORT_d_7_ledF);

   // Disable JTAG for release LED PIN
      RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
   	   AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
}

void  Init_key_ILL()
{	    	GPIO_InitTypeDef Pin_KEY_ILL;
		    RCC_APB2PeriphClockCmd(RCC_PORT_KEY_ILL, ENABLE);
		    Pin_KEY_ILL.GPIO_Speed = GPIO_Speed_50MHz;
		    Pin_KEY_ILL.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
		    Pin_KEY_ILL.GPIO_Pin   = PIN_IN_ILL ;
		    GPIO_Init(PORT_KEY_ILL, &Pin_KEY_ILL);
}

void Init_Timer2 ()
	{
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = 3000;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = 3;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit (TIM2, &timerInitStructure);
	TIM_ITConfig (TIM2, TIM_IT_Update, ENABLE);//авто перезапуск таймера
	TIM_Cmd (TIM2, ENABLE);
	TIM1->DIER |= TIM_DIER_UIE;//включить прерывание по перезагрузке таймера
	NVIC_EnableIRQ(TIM2_IRQn); //Разрешение TIM3_DAC_IRQn прерывания
	//NVIC_SetPriority(TIM3_IRQn ,5);//ПРИОРИТЕТ 5
	TIM_ClearITPendingBit (TIM2, TIM_IT_Update );//сброс флага прерывания
}

void Init_Timer3 ()
	{
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = 3000;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = 3;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit (TIM3, &timerInitStructure);
	TIM_ITConfig (TIM3, TIM_IT_Update, ENABLE);//авто перезапуск таймера
	TIM_Cmd (TIM3, ENABLE);
	TIM3->DIER |= TIM_DIER_UIE;//включить прерывание по перезагрузке таймера
	NVIC_EnableIRQ(TIM3_IRQn); //Разрешение TIM3_DAC_IRQn прерывания
	//NVIC_SetPriority(TIM3_IRQn ,5);//ПРИОРИТЕТ 5
	TIM_ClearITPendingBit (TIM3, TIM_IT_Update );//сброс флага прерывания
}

//*************************************************************************************//
//*******//
//установка значения с прерываниями..//

 void Set_RAZ (u16 i,u8 R)//i код символа, Zразряд
 {
 	 GPIO_ResetBits(PORT_LED,_MR_ALL);// тушим знакоместо
 	// GPIO_SetBits(PORT_LED,M[8]); //обнуляем предыдущее значение
 	GPIO_SetBits(PORT_LED,M8); //обнуляем предыдущее значение
 	 GPIO_ResetBits(PORT_LED,M[i]); //уст новое значение
 	 GPIO_SetBits(PORT_LED,_MR[R]);// включаем  знакоместа
 }

 void SetC_RAZ (u16 i,u8 R)//i код символа, Zразряд
  {
  	 GPIO_ResetBits(PORT_LED,_MR_ALL);// тушим знакоместо
  	 GPIO_SetBits(PORT_LED,M8); //обнуляем предыдущее значение
  	 GPIO_ResetBits(PORT_LED,i); //уст новое значение
  	 GPIO_SetBits(PORT_LED,_MR[R]);// включаем  знакоместа
  }

//****************************************************************************************//

void SetD_I_RAZ (u16 buf[], u8 R, u16 C)//R разрядеость LED//С символ в конце экрана
  {	u8 ill;
  u8 Rsm = R-1;//двигаем символ последнего разряда
	if (GPIO_ReadInputDataBit(PORT_KEY_ILL,PIN_IN_ILL)){ill = ILL1;}//проверяем сигналл изменения яркости
	else ill = ILL0;

		ILLR++;
		if(ILLR >= ill){GPIO_ResetBits(PORT_LED,_MR_ALL);}//режим ауты по числу яркости
		if(ILLR < ILL_max){ return;}
		ILLR=0;

          if( C>0 && RAZ == Rsm){SetC_RAZ (C, RAZ); RAZ++; REG++;return;}//выставляем символ в конце строки(от длинны)

	if ( RAZ > (R-1)){REG = 0; RAZ = 0;}//(= 4-1)4 - разрядеомть индикатора

	if (buf[REG] == 0 ){GPIO_ResetBits(PORT_LED,_MR[RAZ - 1] | _MR[RAZ]); REG++; RAZ++;}//i код символа, Zразряд}//тушим знакоместо
		else
	{
  		 if(buf[REG] == '-') { SetC_RAZ(minus,RAZ); REG++; RAZ++;}// установка минуса
  else
  	{
  	if(buf[REG]>=0x30 && 0x39 >= buf[REG] )//если цыфра и разряд на один меньше разрядности
  		  	{Set_RAZ (buf[REG]-0x30,RAZ); RAZ++; REG++;}//М массив  цифр
  					else{SetC_RAZ (buf[REG],RAZ); RAZ++; REG++;}// для сложных элементов(символы)

  		if(buf [REG] == '.'&& RAZ <= R-2){ GPIO_ResetBits(PORT_LED,_TN);REG++;}//установка точки
  	    				else {GPIO_SetBits(PORT_LED,_TN);}// тушим точку
	 }
   }
}
//************************************************************************************//
//установка значения с прерываниями..//

 void Set_RAZF (u16 i,u8 R)//i код символа, Zразряд
 {
 	 GPIO_ResetBits(PORT_LEDF,_MR_FLL);// тушим знакоместо
 	// GPIO_SetBits(PORT_LED,M[8]); //обнуляем предыдущее значение
 	GPIO_SetBits(PORT_LED,M8); //обнуляем предыдущее значение
 	 GPIO_ResetBits(PORT_LED,M[i]); //уст новое значение
 	 GPIO_SetBits(PORT_LEDF,_MRF[R]);// включаем  знакоместа
 }

 void SetC_RAZF (u16 i,u8 R)//i код символа, Zразряд
  {
  	 GPIO_ResetBits(PORT_LEDF,_MR_FLL);// тушим знакоместо
  	 GPIO_SetBits(PORT_LED,M8); //обнуляем предыдущее значение
  	 GPIO_ResetBits(PORT_LED,i); //уст новое значение
  	 GPIO_SetBits(PORT_LEDF,_MRF[R]);// включаем  знакоместа
  }

//****************************************************************************************//
void SetD_I_RAZF (u16 buf[], u8 R)//R разрядеость LED
  {	u8 ill;
  u8 Rsm = R-1;//двигаем символ последнего разряда
	if (GPIO_ReadInputDataBit(PORT_KEY_ILL,PIN_IN_ILL)){ill = ILL1;}//проверяем сигналл изменения яркости
	else ill = ILL0;

		ILLR++;
		if(ILLR >= ill){GPIO_ResetBits(PORT_LEDF,_MR_FLL);}//режим ауты по числу яркости
		if(ILLR < ILL_max){ return;}
		ILLR=0;

         // if( C>0 && RAZ == Rsm){SetC_RAZ (C, RAZ); RAZ++; REG++;return;}//выставляем символ в конце строки(от длинны)

	if ( RAZ > (R-1)){REG = 0; RAZ = 0;}//R - разрядность индикатора

	if (buf[REG] == 0 ){GPIO_ResetBits(PORT_LEDF,_MR[RAZ - 1] | _MR[RAZ]); REG++; RAZ++;}//i код символа, Zразряд}//тушим знакоместо
		else
	{
  		 if(buf[REG] == '-') { SetC_RAZF(minus,RAZ); REG++; RAZ++;}// установка минуса
  else
  	{
  	if(buf[REG]>=0x30 && 0x39 >= buf[REG] )//если цыфра и разряд на один меньше разрядности
  		  	{Set_RAZF (buf[REG]-0x30,RAZ); RAZ++; REG++;}//М массив  цифр
  					else{SetC_RAZF (buf[REG],RAZ); RAZ++; REG++;}// для сложных элементов(символы)

  		if(buf [REG] == '.'&& RAZ <= R-1){ GPIO_ResetBits(PORT_LED,_TN);REG++;}//установка точки
  	    				else {GPIO_SetBits(PORT_LED,_TN);}// тушим точку
	 }
   }
}





/************************************************/
/**************************************************/
//установка значений без прерывания

void SetS_RAZ (u16 *str,u8 ms_taim)
{	u8 R= 0;
	while (*str != 0) {SetC_RAZ (*str++,R++);delay_ms(ms_taim);}
	}

void SetD_RAZ (char *str,u8 ms_taim)
{	u8 R=0;
	while (*str != 0)
	{
	SetC_RAZ (M[(*str++)-0x30],R++);delay_ms(ms_taim);
	if(R==1)GPIO_ResetBits(PORT_LED,_TN);//установка точки
	    		else GPIO_SetBits(PORT_LED,_TN);// тушим точку
	}
}
