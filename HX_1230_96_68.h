//**********************************//
//пишем по схеме..
//1  - GND
//2  - VCC
//3  - SCE    //вкл выкл дисплэя
//4   -RST
//5  - D/S    //команды ,данные
//6  - MOSI   //данные
//7  - SLCK   //синхро
//8  - LED

#include <stm32f10x.h>

/*
#define lcd_DC       GPIO_Pin_9// D5 d/S   GPIO_Pin_10
#define lcd_RST       GPIO_Pin_8//  D4 reset GPIO_Pin_11
#define lcd_SCE	      GPIO_Pin_7//
#define lcd_DIN        GPIO_Pin_15//  MOSI--D16
#define lcd_SCLK      GPIO_Pin_13// SCK--D15
#define lcd_LED      GPIO_Pin_12//  D8
#define lcd_PORT     GPIOB

#define SPI_lcd  SPI2
//#define SPIx_lcd  SPI1


 */


//****************************************************************************

//****************************************************************************

//*****************************************************************************

//*****************************************************************************

#define lcd_DC_H     GPIO_SetBits(lcd_PORT,lcd_DC)//команда или данные
#define lcd_DC_L     GPIO_ResetBits(lcd_PORT,lcd_DC)
#define lcd_RST_H    GPIO_SetBits(lcd_PORT,lcd_RST)
#define lcd_RST_L    GPIO_ResetBits(lcd_PORT,lcd_RST)
#define lcd_MOSI_H    GPIO_SetBits(lcd_PORT,lcd_MOSI)
#define lcd_MOSI_L    GPIO_ResetBits(lcd_PORT,lcd_MOSI)
#define lcd_SCLK_H   GPIO_SetBits(lcd_PORT,lcd_SCLK)
#define lcd_SCLK_L   GPIO_ResetBits(lcd_PORT,lcd_SCLK)
#define lcd_SCE_H     GPIO_SetBits(lcd_PORT,lcd_SCE)
#define lcd_SCE_L     GPIO_ResetBits(lcd_PORT,lcd_SCE)
#define lcd_LED_ON    GPIO_SetBits(lcd_PORT,lcd_LED)
#define lcd_LED_OFF    GPIO_ResetBits(lcd_PORT,lcd_LED)


//************************************************************************//
//размер в пиксилях
#define	Y_height    8
#define	X_width		96
//контраст
#define contr_LCD   31
//************************************************************************//
#define P_ON  		0x1f       //вкл чип 0
#define P_OFF 		0x10       //выкл чип 1
#define CNTRST      0x80		//контраст (0-32)
#define E_L    		0xA6	  //нет инверсии
#define E_H	   		0xA7	  //инверсия изобр
#define D_H    		0xA4      //все точки выкл(нормально)
#define D_L    		0xA5      //точки включены
#define E_LED		0xAF      //отображение включено
#define D_LED		0xAE      //отобрачение выключено
#define V_VER  		0xB0       //адресса по вертикали 1
#define V_GOR_L     0x00		//младшие  по горизонтали  4
#define V_GOR_H  	0x10		//старшие по горизонтали  3
#define SCAN  	    0x40		//Start Line

#define SetY        0xB0                //установка курсора по Y (значения от 0-8)
#define SetX_L 		0x00				//установка курсора по X (0 )
#define SetX_H 		0x10				//установка курсора по X (0 )


//***********************************************************************//
u8 oscl[85];//для бегущей строки
u8 x_oscl;//текущие координаты бег строки
u8 i_oscl;//для массива
//*************************************************************************//

void gpio_spi_Init();
void lcd_wbyte ( uint8_t data);
void lcd_write_cmd (uint8_t data);
void lcd_write_dat (uint8_t data);
void lcd_clear();
void init_lcd();
void lcd_set_pos (uint8_t  y, uint8_t  x);
void lcd_set_sector(unsigned char y_pos,unsigned char  x_pos, unsigned char  y_s, unsigned char  x_s, unsigned char *data,_Bool invert);
//void lcd_set_char_5x8 (unsigned char y_pos,unsigned char  x_pos, unsigned char data ,_Bool invert);
//void lcd_set_str_5x8 (unsigned char y_pos,unsigned char  x_pos, unsigned char *data ,_Bool invert);
void lcd_set_char (unsigned char y_pos,unsigned char  x_pos, /*unsigned*/ char data ,_Bool invert);
//void lcd_set_st (unsigned char y_pos,unsigned char  x_pos, unsigned char  y_s, unsigned char  x_s, unsigned char *data ,_Bool invert);
void lcd_set_strs (unsigned char y_pos,unsigned char  x_pos, unsigned char R ,/*unsigned*/ char *data ,_Bool invert);
void lcd_set_rect (unsigned char y, unsigned char x ,uint32_t L, _Bool invert);//рисуем прямоугольник висотой 8 пикселей
void Init_Timer2 ();
void TIM2_IRQHandler ();
u8  oscl_tr (u32 r, u16 d);
void oscl_set (u8 y);
void Set_contr(u8 level_contr);
