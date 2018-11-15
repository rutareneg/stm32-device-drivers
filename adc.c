#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include <stm32f10x_rcc.h>
#include "stm32f10x_conf.h"
#include "delay.h"
//#include "stm32f10x_tim.h"
#include "stm32f10x_adc.h"
#include "adc.h"


void adc_init ()
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE); //включаем тактирование ацп1
	//ADC_DeInit(); //сбрасываем настройки АЦП по умолчанию
	ADC_InitTypeDef adc_ini; //создаем структуру
	adc_ini.ADC_ContinuousConvMode = DISABLE;  //преобразование по комманде
	adc_ini.ADC_Mode = ADC_Mode_Independent; //
	adc_ini.ADC_ScanConvMode = DISABLE; //однократное преобразование(только одно ацп)
	adc_ini.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //не использовать триггер
	adc_ini.ADC_DataAlign = ADC_DataAlign_Right; //выравнивание результата по правому краю ADC_DataAlign_Left
	adc_ini.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &adc_ini); //записать настройки
	//ADC_Cmd(ADC1,ENABLE); //включить АЦП
	//ADC1->CR2	|= ADC_CR2_TSVREFE ;

	//adc_port_init ();//настраиваем порт ацп

	}

void adc_port_init ()
{
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //включаем тактирование портаА
		GPIO_InitTypeDef PORT_ADC;                     //создаем структуру
		PORT_ADC.GPIO_Mode = GPIO_Mode_AIN;             //режим - аналоговый вход
		PORT_ADC.GPIO_Speed = GPIO_Speed_50MHz;
		PORT_ADC.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;               //
		GPIO_Init(GPIOA, &PORT_ADC);  					 //записать настройки
}




u16 readADC1_W(u8 channel,float W)
{	u16 res;
	//float W = adc_calib;
	//настройка - канал 1,

  	  ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_55Cycles5);


	ADC_Cmd(ADC1,ENABLE); //включить АЦП
	/* Enable ADC1 reset calibaration register */

	   ADC_ResetCalibration(ADC1);
	   // Check the end of ADC1 reset calibration register
	      while(ADC_GetResetCalibrationStatus(ADC1));
	      // Start ADC1 calibaration
	         ADC_StartCalibration(ADC1);
	        // Check the end of ADC1 calibration
	            while(ADC_GetCalibrationStatus(ADC1));


	      ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
//начать преобразование
   ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	            /* Probably overkill */
  // ждем окончания преобразования
  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

 //  if (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == SET)
 //  { // возвращаем полученное значение
  res =  ADC_GetConversionValue(ADC1);
   /* Probably overkill */

  ADC_Cmd(ADC1,DISABLE); //вЫключить АЦП

  res = (u16)res * (W/100); // пересчёт значения

 return res;
 // return ADC_GetConversionValue(ADC1);
}


float Get_TempSensor(float W,u16 tzer)
{	float res;

  	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_55Cycles5);

	ADC_Cmd(ADC1,ENABLE); //включить АЦП
	// Enable ADC1 reset calibaration register
	ADC_TempSensorVrefintCmd(ENABLE);//подключаем температурный сенсор
  	//Enable temp sensor
	   ADC_ResetCalibration(ADC1);
	   // Check the end of ADC1 reset calibration register
	      while(ADC_GetResetCalibrationStatus(ADC1));
	      // Start ADC1 calibaration
	         ADC_StartCalibration(ADC1);
	        // Check the end of ADC1 calibration
	            while(ADC_GetCalibrationStatus(ADC1));

	      ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
//начать преобразование
   ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	            /* Probably overkill */
  // ждем окончания преобразования
  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

 //  if (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == SET)
 //  { // возвращаем полученное значение
  res =  ADC_GetConversionValue(ADC1);
   /* Probably overkill */
  ADC_TempSensorVrefintCmd(DISABLE);//подключаем температурный сенсор
  ADC_Cmd(ADC1,DISABLE); //вЫключить АЦП
  res = (res -tzer)* (W/100); // пересчёт значения, отнимаем переход через"0"

 return res;
 // return ADC_GetConversionValue(ADC1);
}


