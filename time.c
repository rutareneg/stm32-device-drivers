#include "time.h"
#include "stm32f10x.h"
#include "stdio.h"
#include "delay.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_iwdg.h"
#include "stm32f10x_tim.h"
#include "misc.h"



 uint32_t RTC_GetCounter_(void);

//uint32_t tim;


void timer_to_cal (unsigned long timer, unix_cal * unix_time)
{
	unsigned long a;
		char b;
		char c;
		char d;
		unsigned long time;

		time = timer%SEC_A_DAY;
		a = ((timer+43200)/(86400>>1)) + (2440587<<1) + 1;
		a>>=1;
		unix_time->wday = a%7;
		a+=32044;
		b=(4*a+3)/146097;
		a=a-(146097*b)/4;
		c=(4*a+3)/1461;
		a=a-(1461*c)/4;
		d=(5*a+2)/153;
		unix_time->mday=a-(153*d+2)/5+1;
		unix_time->mon=d+3-12*(d/10);
		unix_time->year=100*b+c-4800+(d/10);
		unix_time->hour=time/3600;
		unix_time->min=(time%3600)/60;
		unix_time->sec=(time%3600)%60;

}



unsigned long cal_to_timer (unix_cal * unix_time)
{
	char a;
	int y;
	char m;
	unsigned long Uday;
	unsigned long time;

	a=((14-unix_time->mon)/12);
	y=unix_time->year+4800-a;
	m=unix_time->mon+(12*a)-3;
	Uday=(unix_time->mday+((153*m+2)/5)+365*y+(y/4)-(y/100)+(y/400)-32045)-2440588;
	time=Uday*86400;
	time+=unix_time->sec+unix_time->min*60+unix_time->hour*3600;
	return time;
}
void RTC_Configuration(void)
{
    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Reset Backup Domain */// сброс памяти часов на 0
   // BKP_DeInit();

    /* Enable LSE */
    RCC_LSEConfig(RCC_LSE_ON);
    /* Wait till LSE is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {}

    /* Select LSE as RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    /* Enable RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Set RTC prescaler: set RTC period to 1sec */
    RTC_SetPrescaler(0x7FFF); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    NVIC_Configuration();
}

 uint32_t RTC_GetCounter_(void)                                                             //Получить значение счётчика
{
         return  (uint32_t)((RTC->CNTH << 16) | RTC->CNTL);
}

void RTC_SetCounter_(uint32_t count)                                                    //Записать новое значение счётчика
{
	 RTC->CRL |= RTC_CRL_CNF;    //включить режим конфигурирования
	  RTC->CNTH = count>>16;      //записать новое значение счетного регистра
	  RTC->CNTL = count;          //
	  RTC->CRL &= ~RTC_CRL_CNF;   //выйти из режима конфигурирования
/*  RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;  //Включить тактирование PWR и Backup
  PWR->CR |= PWR_CR_DBP;                                                            //Включить тактирование к  Backup области
  while (!(RTC->CRL & RTC_CRL_RTOFF));                                         //Проверить закончены ли изменения регистров RTC
  RTC->CRL |= RTC_CRL_CNF;                                                          //Разрешить запись в регистры  RTC
  RTC->CNTH = count>>16;                                                              //Записать новое значение счётного регистра
  RTC->CNTL = count;
  RTC->CRL &= ~RTC_CRL_CNF;                                                       //Запретить запись RTC
  while (!(RTC->CRL & RTC_CRL_RTOFF));                                         //Дождаться окончания записи
  PWR->CR &= ~PWR_CR_DBP;
  */                                                     //запретить доступ к  Backup области
}



void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    //* Configure one bit for preemption priority
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    //* Enable the RTC Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
//для мигания двоеточия
void Init_Timer4 ()
	{
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = 10000;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = 3000;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit (TIM4, &timerInitStructure);
	TIM_ITConfig (TIM4, TIM_IT_Update, ENABLE);//авто перезапуск таймера
	TIM_Cmd (TIM4, ENABLE);
	TIM1->DIER |= TIM_DIER_UIE;//включить прерывание по перезагрузке таймера
	NVIC_EnableIRQ(TIM4_IRQn); //Разрешение TIM3_DAC_IRQn прерывания
	//NVIC_SetPriority(TIM3_IRQn ,5);//ПРИОРИТЕТ 5
	TIM_ClearITPendingBit (TIM4, TIM_IT_Update );//сброс флага прерывания
}

