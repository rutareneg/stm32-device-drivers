

///*******************************//
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_conf.h"
//#include "stm32f10x_adc.h"
#include "stm32f10x_tim.h"
//*********************************//
//#include "Key.h"
#include "main.h"
#include "delay.h"
#include "stepmotor.h"
//********************************************************************************//
void INI_STEP_MOTO()
{
	INI_MOTO_GPIO();
	INI_MOTO_TIM();
	 GPIO_WriteBit(PORT_MOTO, X_PIN_DIR, EN);
	  GPIO_WriteBit(PORT_MOTO, X_PIN_DIR, DR);
}
//*****************************************************************************//
 void INI_MOTO_GPIO()
 {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	 GPIO_InitTypeDef PO_;
	 PO_.GPIO_Pin = X_PIN_STEP;
	 PO_.GPIO_Mode = GPIO_Mode_AF_PP;//подключаемся к таймеру 4
	 PO_.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(PORT_MOTO, &PO_);

	 GPIO_InitTypeDef PDE;
	 PDE.GPIO_Pin = X_PIN_DIR|X_PIN_EN;
	 PDE.GPIO_Mode = GPIO_Mode_Out_PP;
	 PDE.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(PORT_MOTO, &PDE);
 }
//**************************************************************************************//
 void INI_MOTO_TIM()
 {
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	    TIM_TimeBaseInitTypeDef tm;
	    tm.TIM_Prescaler = PRESCALER;
	    tm.TIM_Period = PER;
	    tm.TIM_ClockDivision = 0;
	    tm.TIM_CounterMode = TIM_CounterMode_Down;
	    TIM_TimeBaseInit (TIM4, &tm);

	    TIM_OCInitTypeDef tmPWM;
	    tmPWM.TIM_Pulse = STEP;
	    tmPWM.TIM_OCMode = TIM_OCMode_PWM1;
	    tmPWM.TIM_OutputState = TIM_OutputState_Enable;
	    //tmPWM.TIM_OCPolarity = TIM_OCPolarity_High; //TIM_OCPolarity_Low;//
	    if(S){tmPWM.TIM_OCPolarity = TIM_OCPolarity_High;}
	    			else{tmPWM.TIM_OCPolarity =TIM_OCPolarity_Low;}
	    TIM_OC4Init(TIM4, &tmPWM);

	    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	    TIM4->DIER |= TIM_DIER_UIE;//включить прерывание по перезагрузке таймера

	    	TIM_ClearITPendingBit (TIM4, TIM_IT_Update );//сброс флага прерывания
	    	//TIM_CtrlPWMOutputs(TIM4,ENABLE);
	    	//TIM_ITConfig(TIM4, TIM_IT_CC4, ENABLE);
	    	//TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	    	NVIC_EnableIRQ(TIM4_IRQn); //Разрешение TIM4_DAC_IRQn прерывания

	   // TIM_Cmd(TIM4, ENABLE); //включать по необходимости!!!
 }
//**********************************************************************************************//

 void StartMotor()
 {

	 GPIO_WriteBit(PORT_MOTO, X_PIN_EN, EN);
	  GPIO_WriteBit(PORT_MOTO, X_PIN_DIR, DR);
	  TIM_Cmd(TIM4, ENABLE);
 }

//**********************************************************************************************//

 void StoptMotor()
  {
 	 TIM_Cmd(TIM4, DISABLE);
	 GPIO_WriteBit(PORT_MOTO, X_PIN_EN, EN-1);

  }


 //********************************************************************************************//








