#include "HX_1230_96_68.h"

#include <core_cm3.h>
#include <main.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_spi.h>
#include <stm32f10x_tim.h>

#include "misc.h"

#include "delay.h"

#include "FACII_8_16.h"
#include "nums15x31.h"
#include "SystemRus6x8.h"
#include "nums_j_16x24.h"


GPIO_InitTypeDef SPI_G_lcd;

void gpio_spi_Init()
{
	RCC_APB2PeriphClockCmd(RCC_GPIOB, ENABLE);
    //настраиваем дополнительный порт
    GPIO_InitTypeDef port_lcd;
    port_lcd.GPIO_Pin =  lcd_RST|lcd_SCE |lcd_LED;
    port_lcd.GPIO_Mode = GPIO_Mode_Out_PP;
    port_lcd.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(lcd_PORT, &port_lcd);
//настройка порта под спя

	//GPIO_InitTypeDef SPI_G_lcd;
	SPI_G_lcd.GPIO_Pin = lcd_SCLK | lcd_MOSI ;
	SPI_G_lcd.GPIO_Speed = GPIO_Speed_50MHz;
	SPI_G_lcd.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(lcd_PORT, &SPI_G_lcd);

     // Disable JTAG for release LED PIN
     RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
     AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

    //настраиваем модуль spi
   // SPI_I2S_DeInit(SPIx_lcd);

    	RCC_APB1PeriphClockCmd(RCC_SPI2, ENABLE);

     SPI_InitTypeDef lcd_SPI;

     lcd_SPI.SPI_Direction = SPI_Direction_1Line_Tx;
     lcd_SPI.SPI_Mode = SPI_Mode_Master;
     lcd_SPI.SPI_DataSize = SPI_DataSize_8b;
     lcd_SPI.SPI_CPOL = SPI_CPOL_Low;
     lcd_SPI.SPI_CPHA = SPI_CPHA_1Edge;
     lcd_SPI.SPI_NSS = SPI_NSS_Soft;
     lcd_SPI.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
     lcd_SPI.SPI_FirstBit = SPI_FirstBit_MSB;

 //   SPI_NSSInternalSoftwareConfig(SPI_lcd,SPI_NSSInternalSoft_Set);//устанавливаем NSS 1 как мастер
    SPI_Init(SPI_lcd, &lcd_SPI);


   // SPI_SSOutputCmd(SPIx_lcd,ENABLE);

    SPI_Cmd(SPI_lcd, ENABLE);

  //SPI_CalculateCRC(SPIx_lcd,DISABLE);

}
//**************************************************************************//

	void lcd_wbyte (uint8_t data)

		{

		while (SPI_I2S_GetFlagStatus(SPI_lcd, SPI_I2S_FLAG_TXE) == RESET);
	      SPI_I2S_SendData(SPI_lcd, data);
	      while (SPI_I2S_GetFlagStatus(SPI_lcd, SPI_I2S_FLAG_BSY) == SET);
	   // while (SPI_I2S_GetFlagStatus(SPIx_lcd, SPI_I2S_FLAG_RXNE) == RESET);
	  // SPI_I2S_ReceiveData(SPIx_lcd);
		}

	void lcd_write_cmd(uint8_t data)
	{
		lcd_SCE_L;//включили чип

		SPI_G_lcd.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(lcd_PORT, &SPI_G_lcd);

		lcd_MOSI_L;//реж комманд
		//delay_us(10);
		lcd_SCLK_H;//
		//delay_us(10);
		lcd_SCLK_L;//

		SPI_G_lcd.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(lcd_PORT, &SPI_G_lcd);

		lcd_wbyte (data);

		lcd_SCE_H;
		delay_us(50);

		}
/*	void lcd_write_cmd(uint8_t data)
	{
		uint16_t dat = (u16)data;
		dat<<=7;

		lcd_SCE_L;//включили чип

		lcd_wbyte (dat);
		lcd_SCE_H;//выключили

		}*/

//*********************************************************************//

	void lcd_write_dat(uint8_t data)
		{
			lcd_SCE_L;//включили чип


					SPI_G_lcd.GPIO_Mode = GPIO_Mode_Out_PP;
					GPIO_Init(lcd_PORT, &SPI_G_lcd);

					lcd_MOSI_H;//реж данных
					//delay_us(10);
					lcd_SCLK_H;//
					//delay_us(10);
					lcd_SCLK_L;//

					lcd_MOSI_L;

					SPI_G_lcd.GPIO_Mode = GPIO_Mode_AF_PP;
					GPIO_Init(lcd_PORT, &SPI_G_lcd);


			lcd_wbyte (data);

			lcd_SCE_H;//отключили чип
			delay_us(50);

			}
/*	void lcd_write_dat(uint8_t data)
		{
			uint16_t dat =(u16) data;
			dat<<=7;
			dat = dat||0x8000;
			lcd_SCE_L;//включили чип

			lcd_wbyte (dat);
			lcd_SCE_H;//выключили

			}*/

//************************************************************************//

	void init_lcd()
	{

		lcd_RST_H; //
		delay_ms(50);
		lcd_SCE_H; //выбрать чип
		lcd_RST_L; //сброс
		delay_ms(50); //ждём
		lcd_RST_H; //
		lcd_SCE_L;
		delay_ms(5); //ждём
		lcd_SCE_H;
		delay_ms(5); //ждём

		//lcd_write_cmd(0xE2);//ресет
		//delay_ms(100); //ждём
		 lcd_write_cmd(0x2F);
		 lcd_write_cmd(0x90 );//контраст
		 lcd_write_cmd(0xA6);
		 lcd_write_cmd(0xA4);
		 lcd_write_cmd(0xAf);
		 lcd_write_cmd(0x40);
		 lcd_write_cmd(0xB0);
		 lcd_write_cmd(0x10);
		 lcd_write_cmd(0x00);


		lcd_clear();

	}
//*************************************************************************//

  void Set_contr(u8 level_contr)
  {

	  if(31>level_contr>0)

	  lcd_write_cmd(CNTRST | level_contr);//вкл чип, горизонтально, расширенные инструкции

  }

//************************************************************************//

	void lcd_set_pos (uint8_t  y, uint8_t  x)
	{
		lcd_write_cmd(0xB0 + y);
		lcd_write_cmd(0x10 |((x & 0x7F)>>4));
		lcd_write_cmd(0x0F & x);



	}


 //***********************************************************************//

	void lcd_clear() //очистить отображение
	{

		uint16_t L=0;
		lcd_set_pos(0,0);
		while(L<864)
		{
			lcd_write_dat(0x00); L++;

		}

	}
//********рисуем сектор экрана*****************//

	void lcd_set_sector(unsigned char y_pos,unsigned char  x_pos, unsigned char  y_s, unsigned char  x_s, unsigned char *data ,_Bool invert)//ирина области, высота области
	{		unsigned char  w;
			unsigned char  h;
			unsigned char tmp_char;

		for(h=0;h<y_s/8;h++)
		{	lcd_set_pos(y_pos,x_pos);
		for(w=0;w<x_s;w++)
		{	tmp_char = *data++;
			if(invert) {tmp_char = ~tmp_char;}
			lcd_write_dat(tmp_char);}
		y_pos++;

		}
	}

	/*******выводим шрифт ASCII_8*16 ******************/

	void lcd_set_char (unsigned char y_pos,unsigned char  x_pos, /*unsigned*/ char data ,
				_Bool invert)
	{
		u8 tmp_char=0;
		unsigned char  w;
		unsigned char  h;
		unsigned char i=0;

			for(h=0;h<2;h++)
			{
				lcd_set_pos(y_pos,x_pos);
			for(w=0;w<8;w++)
			{
				tmp_char = FACII_8_16[((data - 23)* 16)+i];i++;
				if(invert) {tmp_char = ~tmp_char;}
				lcd_write_dat(tmp_char);}
				y_pos++;}
	}



	void lcd_set_strs (unsigned char y_pos,unsigned char  x_pos, unsigned char R ,/*unsigned*/ char *data ,_Bool invert)
					//R высота шрифта
		{  unsigned char  x_s ;//ширина символа
			unsigned char  y_s ;//высота символа
			switch(R)
					{
				case 8 :  x_s = SystemRus6x8[0]; y_s = SystemRus6x8[1];break;
				case 16 : x_s = FACII_8_16[0];  y_s = FACII_8_16[1];break;
				case 24 : x_s = nums_j_16x24[0];  y_s = nums_j_16x24[1];break;
				case 32 : x_s = nums15x31[0];  y_s = nums15x31[1];break;
				default : return;
					}
			unsigned char  L_s = (y_s/8) * x_s ;//длинна символа массива символа

					u8 tmp_char=0;
					unsigned char  w;
					unsigned char  h;
					unsigned char  posx;
					unsigned char  posy;
					unsigned char  i=0;

					//unsigned short char_s;//short чтобы влез весь шрифт
					unsigned char char_s;//

					while (*data!=0){

										char_s = *data++;
									if(R==32||R==24){char_s -= 43;}//для большого шрифта только цишры и мат знаки
									else {if(char_s > 0x7f){char_s -= 95;}//сли русский алфавит
											else{char_s -= 31;}
											}

					posx = x_pos;
					posy = y_pos;
					i = 0;
					lcd_set_pos(y_pos,x_pos);

						for(h=0;h<y_s/8;h++)
						{
						for(w=0;w<x_s;w++)
							{
							switch(R)
								{
							case 8  : tmp_char = SystemRus6x8[(char_s * L_s)+i];i++; /*tmp_char=tmp_char<<1;*/ break;
							case 16 : tmp_char = FACII_8_16[(char_s * L_s)+i];i++;  break;
							case 24 : tmp_char = nums_j_16x24[((char_s * L_s)+2)+i];i++;  break;//+2 -- данные в файле шрифта
							case 32 : tmp_char = nums15x31[((char_s * L_s)+2)+i];i++;  break;//+2 -- данные в файле шрифта
							default : return;
								}
							if(invert) {tmp_char = ~tmp_char;}
							lcd_write_dat(tmp_char);
							}
							posy++;
							lcd_set_pos(posy,posx);
						}

					if	(x_pos < X_width - x_s ) {x_pos += x_s;}
					else {y_pos += y_s/8; x_pos = 0;}
					if(y_pos > Y_height){break;/*return 0;*/}
					}
		}
//*********************************num15x31***************************//

/********************************прямоугольник***********************/

void lcd_set_rect (unsigned char y, unsigned char x ,uint32_t L, _Bool invert)//координаты х, у, длинаа л, высота всегда 8 пикселей
{	u8 dat= 0xff; if(invert) {dat = ~dat;}

					lcd_set_pos(y,x);
	while(L)
			{
				lcd_write_dat(dat);
			 L--;}
	}

/*************************************/
u8  oscl_tr (u32 r, u16 d)//зделитель d
{	u8 F = 0xff;
	while(r>0 && F>0)
	{
		if(d==1||d==0){r = r-1;}
		else {r = r/d;}
		F = F>>1;
	}
		F=~F;
		return F;
	}
//**************переписываем всю строку оцил на заданную длинну lcd**************//

void oscl_set (u8 y){

	for(x_oscl=X_width;x_oscl>0;x_oscl--)
	 			{
	 			  lcd_set_pos(y,x_oscl);
	 			  lcd_write_dat(oscl[i_oscl++]);
	 			  if(i_oscl>X_width){i_oscl=0;}
	 			}
}

//*****************oscl**************//


/*void TIM2_IRQHandler ()
   {
 	if(TIM_GetITStatus(TIM2, TIM_IT_Update)== SET)

 	{
 			TIM_ClearITPendingBit (TIM2, TIM_IT_Update );


   }
   }
*/

//**************прерывания 2******************/
//

void Init_Timer2 ()
	{
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = 72000;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = 1000;
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



