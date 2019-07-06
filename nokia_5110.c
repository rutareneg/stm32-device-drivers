#include "nokia_5110.h"

#include <core_cm3.h>
#include <main.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_spi.h>
#include <stm32f10x_tim.h>
#include <sys/_stdint.h>
#include "misc.h"

#include "delay.h"

#include "FACII_8_16.h"
#include "nums15x31.h"
#include "SystemRus6x8.h"
#include "nums_j_16x24.h"




void gpio_spi_Init()
{
	RCC_APB2PeriphClockCmd(RCC_GPIOB, ENABLE);
    //настраиваем дополнительный порт
    GPIO_InitTypeDef lcd_port;
     lcd_port.GPIO_Pin =  lcd5110_RST|lcd5110_SCE |lcd5110_DC|lcd5110_LED;
     lcd_port.GPIO_Mode = GPIO_Mode_Out_PP;
     lcd_port.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(lcd5110_PORT, &lcd_port);
//настройка порта под спя

	GPIO_InitTypeDef SPI_G_lcd5110;
	SPI_G_lcd5110.GPIO_Pin = lcd5110_SCLK | lcd5110_MOSI ;
	SPI_G_lcd5110.GPIO_Speed = GPIO_Speed_50MHz;
	SPI_G_lcd5110.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(lcd5110_PORT, &SPI_G_lcd5110);

     // Disable JTAG for release LED PIN
     RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
     AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

    //настраиваем модуль spi
   // SPI_I2S_DeInit(SPIx_lcd);

    	RCC_APB1PeriphClockCmd(RCC_SPI2, ENABLE);

     SPI_InitTypeDef SPI_lcd5110;

     SPI_lcd5110.SPI_Direction = SPI_Direction_1Line_Tx;
     SPI_lcd5110.SPI_Mode = SPI_Mode_Master;
     SPI_lcd5110.SPI_DataSize = SPI_DataSize_8b;
     SPI_lcd5110.SPI_CPOL = SPI_CPOL_Low;
     SPI_lcd5110.SPI_CPHA = SPI_CPHA_1Edge;
     SPI_lcd5110.SPI_NSS = SPI_NSS_Soft;
     SPI_lcd5110.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
     SPI_lcd5110.SPI_FirstBit = SPI_FirstBit_MSB;
    // SPI_lcd12864.SPI_CRCPolynomial = 7;
    //
 //   SPI_NSSInternalSoftwareConfig(SPI_lcd,SPI_NSSInternalSoft_Set);//устанавливаем NSS 1 как мастер
    SPI_Init(SPI_lcd, &SPI_lcd5110);


   // SPI_SSOutputCmd(SPIx_lcd,ENABLE);

    SPI_Cmd(SPI_lcd, ENABLE);

  //SPI_CalculateCRC(SPIx_lcd,DISABLE);

}
/***********************************************************************/
	void lcd_wbyte ( uint8_t data)

	{ while (SPI_I2S_GetFlagStatus(SPI_lcd, SPI_I2S_FLAG_TXE) == RESET);
      SPI_I2S_SendData(SPI_lcd, data);
      while (SPI_I2S_GetFlagStatus(SPI_lcd, SPI_I2S_FLAG_BSY) == SET);
   // while (SPI_I2S_GetFlagStatus(SPIx_lcd, SPI_I2S_FLAG_RXNE) == RESET);
  // SPI_I2S_ReceiveData(SPIx_lcd);
	}



	void lcd_write_cmd (uint8_t data)
	{

	    lcd5110_DC_L;//режим команд
	   // delay_us(50);
		lcd_wbyte (data);
		//delay_us(50);
		//lcd5110_DC_H;//режим данных
		//lcd5110_SCE_H;

		}

//*********************************************************************//

	void lcd_write_dat (uint8_t data)
		{
			//lcd5110_SCE_L;
			lcd5110_DC_H;//режим данных
			lcd_wbyte (data);
			//lcd5110_SCE_H;

			}

//************************************************************************//

	void init_lcd_5110 ()
	{


		//delay_ms(50);

		//lcd5110_SCE_H; //откл отображение
		lcd5110_SCE_L; //выбрать чип
		lcd5110_RST_L; //сброс
		delay_ms(100); //ждём
		lcd5110_RST_H; //

		lcd_write_cmd(FSet|PD_ON|V_GOR|H_H);//вкл чип, горизонтально, расширенные инструкции
		lcd_write_cmd(VOP|0x42);// 0x38 устанавливаем опорное напряжение 0
		lcd_write_cmd(TC|0x02); //тепер коф
		lcd_write_cmd(BIAS|0x13);//смещение
		lcd_write_cmd(FSet|PD_ON|V_GOR|H_B);//вкл чип, горизонтально,  инструкции
		lcd_write_cmd(D_ALL);

		//delay_ms(500);
		//lcd_write_cmd(D_blank);//очистим экран, нормальное отображение
		lcd_write_cmd(D_normal);

		lcd_clear();
		lcd5110_LED_ON;

	}

//************************************************************************//

	void lcd_set_pos (uint8_t  y, uint8_t  x)
	{
		//if (x>83) {x=0;}
		//if (y>5 ) {y=0;}
		lcd_write_cmd(SetX | x);
		lcd_write_cmd(SetY | y);

	}


 //***********************************************************************//

	void lcd_clear() //очистить отображение
	{

		uint16_t L=0;
		lcd_set_pos(0,0);
		while(L<504)
		{
			lcd_write_dat(0x00);L++;

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
    /*******выводим шрифт ASCII_5*7 ******************/
/*
	void lcd_set_char_5x8 (unsigned char y_pos,unsigned char  x_pos, unsigned char data ,_Bool invert)

	{
		unsigned char tmp_char=0;

				unsigned char i=0;
						lcd_set_pos(y_pos,x_pos);
					while(i<5)
					{
						tmp_char = ASCII_5x8_ALL[((data - 32)* 5)+i];i++;
						if(invert) {tmp_char = ~tmp_char;}
						lcd_write_dat(tmp_char);
					}
					NOP;
	}

	void lcd_set_str_5x8 (unsigned char y_pos,unsigned char  x_pos, unsigned char *data ,_Bool invert)
	{
		//lcd_set_pos(y_pos,x_pos);
		unsigned char tmp_char=0;
		unsigned char char_s=0;
		unsigned char i=0;

		while (*data!=0){char_s = *data++;
			if (char_s > 0x7E) char_s -= 0x40;//компенсация урезанной таблицы кодов
								i=0;
							while(i<5)
							{
								lcd_set_pos(y_pos,x_pos);
								tmp_char = ASCII_5x8_ALL[((char_s - 31)* 5)+i];i++;
								if(invert) {tmp_char = ~tmp_char;}
								lcd_write_dat(tmp_char);
									x_pos += 1;
							}
							lcd_set_pos(y_pos,x_pos);
							x_pos += 1;
							lcd_write_dat(0);//разделительная пустышка меж символьная
					}
		//NOP; ненужен уже есть пуст
	}
*/
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



/*
	void lcd_set_st (unsigned char y_pos,unsigned char  x_pos, unsigned char  y_s, unsigned char  x_s, unsigned char *data ,_Bool invert)

	{
		u8 tmp_char=0;
				unsigned char  w;
				unsigned char  h;
				unsigned char  posx;
				unsigned char  posy;
				unsigned char i=0;
				unsigned char char_s;

				while (*data!=0){char_s = *data++;
				posx = x_pos;
				posy = y_pos;

				i =0;
				lcd_set_pos(y_pos,x_pos);

					for(h=0;h<y_s/8;h++)
					{
					for(w=0;w<x_s;w++)
						{
						tmp_char = FACII_8_16[((char_s - 32)* y_s)+i];i++;

						if(invert) {tmp_char = ~tmp_char;}

						lcd_write_dat(tmp_char);
						}
						posy++;
						lcd_set_pos(posy,posx);
					}

				if	(x_pos < 84 - x_s ) {x_pos += x_s;}
				else{y_pos += y_s/8; x_pos = 0;}
				if(y_pos >= 8){return;}
				}
	}
*/
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
					unsigned char i=0;

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

					if	(x_pos < 84 - x_s ) {x_pos += x_s;}
					else{y_pos += y_s/8; x_pos = 0;}
					if(y_pos > 5){break;/*return 0;*/}
					}
		}
//*********************************num15x31***************************//

/********************************прямоугольник***********************/

void lcd_set_rect (unsigned char y, unsigned char x ,unsigned char L, _Bool invert)//координаты х, у, длинаа л, высота всегда 8 пикселей
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

	for(x_oscl=84;x_oscl>0;x_oscl--)
	 			{
	 			  lcd_set_pos(y,x_oscl);
	 			  lcd_write_dat(oscl[i_oscl++]);
	 			  if(i_oscl>84){i_oscl=0;}
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



