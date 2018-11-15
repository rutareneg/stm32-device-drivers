#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include<stm32f10x_rcc.h>
#include "stm32f10x_conf.h"
#include "delay.h"
//#include "stm32f10x_rtc.h"
#include "lcd_44780.h"
#include "main.h"
#include "Simv_hd44780.h"

void PORT_LCD_INI()
{  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
   GPIO_InitTypeDef PORT_LCD_ALL;
   PORT_LCD_ALL.GPIO_Pin = E|RS|RW|D4|D5|D6|D7;
   PORT_LCD_ALL.GPIO_Speed = GPIO_Speed_50MHz;
   PORT_LCD_ALL.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_Init(PORT_LCD, &PORT_LCD_ALL);
/*                                        //оподключается по необходимости
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
     GPIO_InitTypeDef PORT_LED;
     PORT_LED.GPIO_Pin = PIN_ERR;
     PORT_LED.GPIO_Speed = GPIO_Speed_50MHz;
     PORT_LED.GPIO_Mode = GPIO_Mode_Out_PP;
     GPIO_Init(PORT_LCD_ERR, &PORT_LED);
*/

   RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
   AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
}
//**************************************************************//
void LCD_WRITE_4BIT(u8 DATA,_Bool RS_,_Bool RW_)
{
	LCD_F_BUSY ();
	GPIO_WriteBit(PORT_LCD,RS,RS_);
	GPIO_WriteBit(PORT_LCD,RW,RW_);

		GPIO_WriteBit(PORT_LCD,D7,(DATA>>7)& 0x01);
		GPIO_WriteBit(PORT_LCD,D6,(DATA>>6)& 0x01);
		GPIO_WriteBit(PORT_LCD,D5,(DATA>>5)& 0x01);
		GPIO_WriteBit(PORT_LCD,D4,(DATA>>4)& 0x01);
			STROB_;
		GPIO_WriteBit(PORT_LCD,D7,(DATA>>3)& 0x01);
		GPIO_WriteBit(PORT_LCD,D6,(DATA>>2)& 0x01);
		GPIO_WriteBit(PORT_LCD,D5,(DATA>>1)& 0x01);
		GPIO_WriteBit(PORT_LCD,D4,     DATA& 0x01);
			STROB_;


}

//****************************************************************//
    void PORT_INI_F_LCD_IN ()//настраиваем 7 порт LCD на вход
{ //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    	GPIO_InitTypeDef F_PORT_IN;
	F_PORT_IN.GPIO_Pin =  D4|D5|D6|D7;;
	F_PORT_IN.GPIO_Speed = GPIO_Speed_50MHz;
	F_PORT_IN.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(PORT_LCD,&F_PORT_IN);
	}
//*****************************************************************//
void PORT_INI_F_LCD_OUT () //настраиваем 7 порт LCD на выход
{ 	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef F_PORT_OUT;
	F_PORT_OUT.GPIO_Pin = D4|D5|D6|D7;;
	F_PORT_OUT.GPIO_Speed = GPIO_Speed_50MHz;
	F_PORT_OUT.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(PORT_LCD,&F_PORT_OUT);
	}
//**********************************************************************//
	
void LCD_F_BUSY ()	   
{
	PORT_INI_F_LCD_IN();
	u16 i = 10000;
	READ_BUSE;//режим чтения
	EE_ON; delay_us(5);
	while (i)
		{ i--;
	if(  GPIO_ReadInputDataBit(PORT_LCD,D7) == 0)
			{i=0;}
		//если время вышло рисует ошибку
		/*if(i<100){GPIO_ResetBits(PORT_LCD_ERR,PIN_ERR);}
			else{GPIO_SetBits(PORT_LCD_ERR,PIN_ERR);}*/
		}EE_OFF;
	PORT_INI_F_LCD_OUT();
}
//*******************************************************************//
void LCD_INI ()
  {	  
  		PORT_LCD_INI();
  		GPIO_SetBits(PORT_LCD,D4); //РАЗРЯДЕНОСТЬ ШИНЫ 8
  		GPIO_SetBits(PORT_LCD,D5); //РАЗРЯДЕНОСТЬ ШИНЫ 8
  		GPIO_ResetBits(PORT_LCD,D6);
  		GPIO_ResetBits(PORT_LCD,D7);
  		STROB_;	
	   delay_ms(10);
		 STROB_;
		 delay_us(200);
		   STROB_;
		   delay_us(200);
		   GPIO_ResetBits(PORT_LCD,D4);
		   //РАЗРЯДЕНОСТЬ ШИНЫ 4
		  // GPIO_SetBits(PORT_LCD,D5); //РАЗРЯДЕНОСТЬ ШИНЫ 4
		  // GPIO_ResetBits(PORT_LCD,D6);
		  // GPIO_ResetBits(PORT_LCD,D7);
		  STROB_;
		  delay_us(200);

		LCD_WRITE_4BIT(DISPLEY_DATA|F_4BIT|F_5X8|F_2LINE,BCMD,BWRITE); //4БИТА, 2СТРОКИ, 5Х8
		delay_us(100);
		LCD_WRITE_4BIT(SHIFT_POSITION|F_SHIFT_R|F_SHIFT_KURSOR,BCMD,BWRITE); // СДВИГ ЗНАКОМЕСТА В ПРАВО
		delay_us(200);
		LCD_WRITE_4BIT (DISPLEY_MODE|F_DISPLEY_ON|F_BLINC_OFF,BCMD,BWRITE);	// вкл ИЗОБРАЖЕНИЕ, ВЫКЛ КУРСОР, ВЫКЛЮЧЕН МИГАНИЕ ЗНАКОМЕСТА
		delay_us(200);
		LCD_WRITE_4BIT (CLEAR,BCMD,BWRITE);//очистка
		LCD_WRITE_4BIT (RETURN_HOME,BCMD,BWRITE);// начало 00 00
			
  }
//*************************************************//

   void LCD_GOTO (u8 X, u8 Y)
	{	
		switch(Y)
	{
	case 0:          ; break;
	case 1: X = 0x40+X; break;
	case 2: X = 0x14+X; break;
	case 3: X = 0x54+X; break;
	default:         ;
	}
	  	X+= F_MESTO;
	
		LCD_WRITE_4BIT(X,BCMD,BWRITE);
	}
//*******************************************************//

//*********************************************************//
void LCD_SETPC ( unsigned char c[])
 {
	LCD_WRITE_4BIT(F_RAMM,BCMD,BWRITE);//00  устанавливаем адрес для перезаписи

	while(*c!=0){LCD_WRITE_4BIT(*c++,BDATA,BWRITE);}
	   }
//********************************************************//
void LCD_PUTC ( unsigned char c)

	{LCD_WRITE_4BIT(c,BDATA,BWRITE);}
//********************************************************//

void LCD_PUTS (unsigned char *str)
	{
		while (*str != 0)
		   {
		      if (*str < 0xC0)
		      {
		    	  LCD_WRITE_4BIT(*str++,BDATA,BWRITE);
		      }
		      else
		      {
		    	  LCD_WRITE_4BIT(ANSI1251_RU[*str++ - 0xC0],BDATA,BWRITE);
		      }
		   }
	}
//********************************************************//

void LCD_PUTST (unsigned char *str, u8 L) //l длинна строки
	{
		while (*str != 0 && L)
		   {L--;//ищим конец знакомест
		      if (*str < 0xC0)
		      {
		    	  LCD_WRITE_4BIT(*str++,BDATA,BWRITE);
		      }
		      else
		      {
		    	  LCD_WRITE_4BIT(ANSI1251_RU[*str++ - 0xC0],BDATA,BWRITE);
		      }
		   }
		while (L){LCD_WRITE_4BIT(' ',BDATA,BWRITE);L--;}
	}
//********************************************************//


