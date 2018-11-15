#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include<stm32f10x_rcc.h>
#include "stm32f10x_conf.h"


/******************************************************
#define  PORT_LED GPIOB
#define  _A GPIO_Pin_3
#define  _B GPIO_Pin_4//GPIO_Pin_1
#define  _C GPIO_Pin_5
#define  _D GPIO_Pin_6
#define  _E GPIO_Pin_7
#define  _F GPIO_Pin_8//GPIO_Pin_0
#define  _G GPIO_Pin_9
#define  _TN   GPIO_Pin_0
#define  _MR1 GPIO_Pin_12
#define  _MR2 GPIO_Pin_13
#define  _MR3 GPIO_Pin_14
#define  _MR4 GPIO_Pin_15

//*************************************************
#define  PORT_LED GPIOB
#define  _A GPIO_Pin_3
#define  _B GPIO_Pin_4//GPIO_Pin_1
#define  _C GPIO_Pin_5
#define  _D GPIO_Pin_6
#define  _E GPIO_Pin_7
#define  _F GPIO_Pin_8//GPIO_Pin_0
#define  _G GPIO_Pin_9
#define  _TN   GPIO_Pin_0 //точка

#define  PORT_LEDF  GPIOA
#define  _MR1 GPIO_Pin_2//ƒл€ порта ј
#define  _MR2 GPIO_Pin_3//
#define  _MR3 GPIO_Pin_4//
#define  _MR4 GPIO_Pin_5//
#define  _MR5 GPIO_Pin_6//
#define  _MR6 GPIO_Pin_7//разр€да
//******************************************************
//#define  PORT_LEDT GPIOA
//#define  _TN       GPIO_Pin_0
#define  _TCLOCK   GPIO_Pin_15 //двоеточие часы ит верхн€€ точка

#define  RCC_PORT_KEY_ILL	RCC_APB2Periph_GPIOA
#define  PIN_IN_ILL  GPIO_Pin_15//порт включени€ изменени€ €ркости даЄм + -> падает €ркость
*/


#define  M0   _A|_B|_C|_D|_E|_F
#define  M1   _B|_C
#define  M2   _A|_B|_G|_E|_D
#define  M3   _A|_B|_G|_C|_D
#define  M4   _F|_G|_B|_C
#define  M5   _A|_F|_G|_C|_D
#define  M6   _A|_F|_G|_E|_D|_C
#define  M7   _A|_B|_C
#define  M8   _A|_B|_C|_D|_E|_F|_G
#define  M9   _G|_F|_A|_B|_C|_D
#define  E_	  _A|_D|_E|_F|_G
#define  r_   _E|_G
#define  P_   _A|_B|_E|_F|_G
#define  C_	  _A|_D|_E|_F
#define  c_	  _D|_E|_G
#define  m_   _G
#define  A_  _A|_B|_C|_E|_F|_G
#define  L_  _D|_E|_F
#define  l_  _D|_E
#define  tire_  _G
#define  c_k  _A|_D|_E|_G
#define minus    m_
#define	  V_   _B|_C|_D|_E|_F
#define	  v_   _C|_D|_E
#define	  v_k   _A|_C|_D|_E


 //char buf[];//масив дл€ экрана
//u8   ill = 20;// €ркость экрана
// u16  M[] = {M0, M1, M2, M3, M4, M5, M6, M7, M8, M9, E_, r_, P_, C_,c_,c_k, m_, A_,L_,V_};//массив с числами
 //u16 _MR[]= {_MR1,_MR2,_MR3,_MR4};//разр€ды экрана
#define _MR_ALL  _MR1|_MR2|_MR3|_MR4
#define _MR_FLL  _MR1|_MR2|_MR3|_MR4 |_MR5 |_MR6

#define  mess     buf[0] = V_;buf[1] = V_;buf[2] = V_;buf[3] = V_;
#define  errors   buf[0] = E_;buf[1] = r_;buf[2] = r_;buf[3] = simv;
    u8 RAZ ; // разр€д индикатора
	u8 REG ;// контроль номера массива
	u8 ILLR ;//отсчЄт прерываний €ркости экрана
	uint8_t ILL0;//ILL_max;//сюда установаливаетс€ текуща€ €ркость
	uint8_t ILL1;//сюда установаливаетс€ 2€ €ркость
#define  ILL_max  20//максимально значение €ркости
#define  In_ILL0  if(ILL0 >= ILL_max)ILL0 = ILL_max//ограничиваем первую €ркост
#define  In_ILL1  if(ILL1 >= ILL_max)ILL1 = ILL_max// ограничиваем втрую €ркость

u16  simv;//доп символна экране.
 void PORT_LED_OUT();
 void PORT_LEDT_OUT();
 void Set_RAZ (u16 i,u8 R);
 void SetC_RAZ (u16 i,u8 R);
 void SetS_RAZ (u16 *stru8,u8 ms_taim);
 void SetD_RAZ (char *str,u8 ms_taim);
 void Init_key_ILL();
 void Init_LED_7S();
 void Init_Timer3 ();
 void Init_Timer1 ();
 void SetD_I_RAZ (u16 buf[], u8 R, u16 C);//R разр€деость LED//— символ в конце экрана

