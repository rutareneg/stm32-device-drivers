
//#include "stm32f10x.h"
//#include "stm32f10x_gpio.h"
//#include<stm32f10x_rcc.h>
//#include "stm32f10x_conf.h"
//#include "stm32f10x_adc.h"

 //u16 adc_calib;//значение умножения значений назначить в main
 //#define adc_calinc  adc_calib/500 // еденица увеличения  или уменьшения

   void adc_init ();
   u16 readADC1_W(u8 channel,float W);
   void adc_port_init ();
   float Get_TempSensor(float W,u16 tzer);
