/**************************************************************************
 * Данная библиотека предназначена дла работы с RTC, а точнее с Unix Time *
 * Позволяет переводить из счетчика в календарь(день, месяц, год) 		  *
 * и время (часы, минуты, секунды), и обратно							  *
 **************************************************************************/
 //#include "stm32f10x.h"

#define tm_def   1521000000  // начало калаендаря по умолчанию
#define SEC_A_DAY 86400
typedef struct	   
	{
	int  year;
	char mon;
	char mday;
	char wday;
	char hour;
	char min;
	char sec;
	} unix_cal;// обзываем структуру

	unix_cal unix_time; //создаём обьёкт преременных из структур
	unix_cal set_time; // обьект для настройки часов

//u8 const *WEEK_STR[]= {"ПНД.","ВТР.","СРД.","ЧТВ.","ПТН.","СБТ.","ВСК."};

void timer_to_cal (unsigned long timer , unix_cal * unix_time);

void RTC_Configuration(void);
//void RTC_IRQHandler(void);
void NVIC_Configuration(void);
unsigned long cal_to_timer (unix_cal * unix_time);
void Init_Timer4 ();


