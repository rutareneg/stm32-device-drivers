//************************************//
//#include "stm32f10x.h"

#include "stm32f10x_conf.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
//#include "stm32f10x_flash.h"
#include "stm32f10x_spi.h"
#include "delay.h"
#include "lcd12864.h"
#include "key.h"
#include "FACII_8_16.h"
#include "main.h"
#include "ASCII_5x8_ALL.h"
#include "SystemRus6x8.h"



void gpio_spi_Init()
{
	RCC_APB2PeriphClockCmd(RCC_GPIOB, ENABLE);
    //настраиваем дополнительный порт
    GPIO_InitTypeDef lcd_port;
     lcd_port.GPIO_Pin =  lcd12864_RST|lcd12864_RW |lcd12864_RS;
     lcd_port.GPIO_Mode = GPIO_Mode_Out_PP;
     lcd_port.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(lcd12864_PORT, &lcd_port);
//настройка порта под спя

	GPIO_InitTypeDef SPI_G_lcd12864;
	SPI_G_lcd12864.GPIO_Pin = lcd12864_SCLK | lcd12864_SID ;
	SPI_G_lcd12864.GPIO_Speed = GPIO_Speed_50MHz;
	SPI_G_lcd12864.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(lcd12864_PORT, &SPI_G_lcd12864);

     // Disable JTAG for release LED PIN
     RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
     AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

    //настраиваем модуль spi
   // SPI_I2S_DeInit(SPIx_lcd);

    	RCC_APB1PeriphClockCmd(RCC_SPI2, ENABLE);

     SPI_InitTypeDef SPI_lcd12864;

     SPI_lcd12864.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
     SPI_lcd12864.SPI_Mode = SPI_Mode_Master;
     SPI_lcd12864.SPI_DataSize = SPI_DataSize_8b;
     SPI_lcd12864.SPI_CPOL = SPI_CPOL_Low;
     SPI_lcd12864.SPI_CPHA = SPI_CPHA_1Edge;
     SPI_lcd12864.SPI_NSS = SPI_NSS_Hard;
     SPI_lcd12864.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
     SPI_lcd12864.SPI_FirstBit = SPI_FirstBit_MSB;
    // SPI_lcd12864.SPI_CRCPolynomial = 7;
    //
 //   SPI_NSSInternalSoftwareConfig(SPI_lcd,SPI_NSSInternalSoft_Set);//устанавливаем NSS 1 как мастер
    SPI_Init(SPI_lcd, &SPI_lcd12864);


   // SPI_SSOutputCmd(SPIx_lcd,ENABLE);

    SPI_Cmd(SPI_lcd, ENABLE);

  //SPI_CalculateCRC(SPIx_lcd,DISABLE);

}
//***********************************************************************//
	void lcd_wbyte ( uint16_t data)

	{ while (SPI_I2S_GetFlagStatus(SPI_lcd, SPI_I2S_FLAG_TXE) == RESET);
      SPI_I2S_SendData(SPI_lcd, data);
      //проверяем занятость
      lcd_rstatus();

    while((0x8000&lcdstat)>0);
	}
//***********************************************************************//
	 lcd_rstatus ()
	{
		 while (SPI_I2S_GetFlagStatus(SPI_lcd, SPI_I2S_FLAG_RXNE) == RESET);
		 lcd12864_RW_H;//чтение
		 lcd12864_RS_L;//режим комманд

		 lcdstat = SPI_I2S_ReceiveData(SPI_lcd);

		 //return  SPI_I2S_ReceiveData(SPI_lcd);

	}
//**********************************************************************//


//***********************************************************************//
	void lcd_write_cmd (uint16_t data)
	{	lcd12864_RW_L;//запись
		lcd12864_RS_L;//режим команд
		lcd_wbyte (data);

		//delay_us(50);
		}

//*********************************************************************//

	void lcd_write_dat (uint16_t data)
		{	lcd12864_RW_L;//запись
			lcd12864_RS_H;//режим данных
			lcd_wbyte (data);

			//delay_us(50);
			}

//************************************************************************//

	void init_lcd_12864 ()
	{

		lcd12864_RS_L;
		lcd12864_RW_L;
		lcd12864_RST_L;
		delay_ms(1000);
		lcd12864_RST_H;
		delay_ms(100);

	    lcd_write_cmd(RESETL);//сбросим адресса в 0
		lcd_write_cmd(PGLP);//
		lcd_write_cmd(COLDOW);//пишем сверху в низ
		lcd_write_cmd(0xA3);// select bias
		lcd_write_cmd(0x2C);// voltage converter on
		delay_ms(10);

		lcd_write_cmd(0x2E);// voltage regulator on
		delay_ms(10);
		lcd_write_cmd(0x2F);// voltage follower on
		lcd_write_cmd(0x27); // regulator resistor
		lcd_write_cmd(0x81);// set reference voltage
		lcd_write_cmd(0x26); // reference voltage value (contraste)
		lcd_write_cmd(DISPON);
		lcd_write_cmd(ENTERON);//all on
		delay_ms(1000);
		lcd_clear();
		lcd_write_cmd(ENTEROFF); // all off

	}
//***********************************************************************//
	void rest_lcd_12864 ()
	{
				lcd12864_RST_L;
			    delay_ms(1);
			    lcd12864_RST_H;

			    lcd_write_cmd(RESETL);//сбросим адресса в 0
				lcd_write_cmd(PGLP);//
				lcd_write_cmd(COLDOW);//пишем сверху в низ
				lcd_write_cmd(0xA3);// select bias
				lcd_write_cmd(0x2C);// voltage converter on
				//delay_ms(2);
				lcd_write_cmd(0x2E);// voltage regulator on
				//delay_ms(2);
				lcd_write_cmd(0x2F);// voltage follower on
				lcd_write_cmd(0x27); // regulator resistor
				lcd_write_cmd(0x81);// set reference voltage
				lcd_write_cmd(0x26); // reference voltage value (contraste)
	}
//************************************************************************//

	void lcd_set_pos (unsigned char  x, unsigned char  y)
	{
		lcd_write_cmd(0x10 | (x>>4));
		lcd_write_cmd(x&0x0f);
		lcd_write_cmd(0xB0 + y);
	}


 //***********************************************************************//

	void lcd_clear() //очистить отображение
	{
		unsigned char  h;
		    unsigned char  w;
		    for(h=0;h<8;h++){lcd_set_pos(0,h);
		    		    		for(w=0;w<LCD_W;w++)
		    		    			lcd_write_dat(0x00);}
	}
//********рисуем сектор экрана*****************//

	void lcd_set_sector(unsigned char x_pos,unsigned char  y_pos, unsigned char  x_s, unsigned char  y_s, unsigned char *data ,_Bool invert)//ирина области, высота области
	{		unsigned char  w;
			unsigned char  h;
			unsigned char tmp_char;
		for(h=0;h<y_s/8;h++)
		{	lcd_set_pos(x_pos,y_pos);
		for(w=0;w<x_s;w++)
		{	tmp_char = *data++;
			if(invert) {tmp_char = ~tmp_char;}
			lcd_write_dat(tmp_char);}
		y_pos++;

		}
	}
//*******выводим шрифт ASCII******************//

	void lcd_set_char (unsigned char x_pos,unsigned char  y_pos, unsigned char  x_s, unsigned char  y_s, unsigned char data ,_Bool invert)
	{
		u8 tmp_char=0;
		unsigned char  w;
		unsigned char  h;
		unsigned char i=0;

			for(h=0;h<y_s/8;h++)
			{
				lcd_set_pos(x_pos,y_pos);
			for(w=0;w<x_s;w++)
			{
				tmp_char = FACII_8_16[((data - 23)* y_s)+i];i++;
				if(invert) {tmp_char = ~tmp_char;}
				lcd_write_dat(tmp_char);}
				y_pos++;}
	}


	void lcd_set_st (unsigned char x_pos,unsigned char  y_pos, unsigned char  x_s, unsigned char  y_s, unsigned char *data ,_Bool invert)

	{
		u8 tmp_char=0;
				unsigned char  w;
				unsigned char  h;
				unsigned char  posx,fx;
				unsigned char  posy,fy;
				unsigned char i=0;
				unsigned char char_s;

				while (*data!=0){char_s = *data++;
				posx = x_pos;
				posy = y_pos;

				i =0;
				lcd_set_pos(x_pos,y_pos);

					for(h=0;h<y_s/8;h++)
					{
					for(w=0;w<x_s;w++)
						{
						tmp_char = FACII_8_16[((char_s - 32)* y_s)+i];i++;

						if(invert) {tmp_char = ~tmp_char;}

						lcd_write_dat(tmp_char);
						}
						posy++;
						lcd_set_pos(posx,posy);
					}

				if	(x_pos < 128 - x_s ) {x_pos += x_s;}
				else{y_pos += y_s/8; x_pos = 0;}
				if(y_pos >= 8){return 0;}
				}
	}

	void lcd_set_strs (unsigned char x_pos,unsigned char  y_pos, unsigned char R ,unsigned char *data ,_Bool invert)
					//R высота шрифта
		{  unsigned char  x_s ;//ширина символа
			unsigned char  y_s ;//высота символа
			switch(R)
					{
				case 16 : x_s = FACII_8_16[0];  y_s = FACII_8_16[1];break;
				case 8 :  x_s =  SystemRus6x8[0]; y_s = SystemRus6x8[1];break;
				default : return;
					}
			unsigned char  L_s = (y_s/8) * x_s ;//длинна массива символа

					u8 tmp_char=0;
					unsigned char  w;
					unsigned char  h;
					unsigned char  posx,fx;
					unsigned char  posy,fy;
					unsigned char i=0;

					//unsigned short char_s;//short чтобы влез весь шрифт
					unsigned char char_s;//

					while (*data!=0){
										char_s = *data++;
										if(char_s>0x7f){char_s -= 95;}
										else {char_s -= 31;}//сдвигаем относительно шрифта

					posx = x_pos;
					posy = y_pos;
					i = 0;
					lcd_set_pos(x_pos,y_pos);

						for(h=0;h<y_s/8;h++)
						{
						for(w=0;w<x_s;w++)
							{
							switch(R)
								{
							case 8 :{ tmp_char = SystemRus6x8[((char_s /*- H*/)* L_s)+i];i++; tmp_char=tmp_char<<1; break;}
							case 16 : tmp_char = FACII_8_16[((char_s /*- 31*/)* L_s)+i];i++;  break;
							default : return;
								}
							if(invert) {tmp_char = ~tmp_char;}
							lcd_write_dat(tmp_char);
							}
							posy++;
							lcd_set_pos(posx,posy);
						}

					if	(x_pos < 128 - x_s ) {x_pos += x_s;}
					else{y_pos += y_s/8; x_pos = 0;}
					if(y_pos >= 8){break;/*return 0;*/}
					}
		}


void lcd_set_rect (u8 x, u8 y ,u8 l, _Bool invert)//координаты х, у, длинаа л, высота всегда 8 пикселей
{	u8 dat=0;if(invert) {dat = ~dat;}

	while(l)
			{	lcd_set_pos(x,y);
				lcd_write_dat(dat);
			x++; l--;}

	}



