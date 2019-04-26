//**********************************//
//пишем по схеме..
//1  - GND
//2  - VCC
//3  - SCE    //вкл выкл дисплэ€
//4   -RST
//5  - D/S    //команды ,данные
//6  - MOSI   //данные
//7  - SLCK   //синхро
//8  - LED

#include <stm32f10x.h>

/*
#define lcd5110_DC       GPIO_Pin_9// D5 d/S   GPIO_Pin_10
#define lcd5110_RST       GPIO_Pin_8//  D4 reset GPIO_Pin_11
#define lcd5110_SCE	      GPIO_Pin_7//
#define lcd5110_DIN        GPIO_Pin_15//  MOSI--D16
#define lcd5110_SCLK      GPIO_Pin_13// SCK--D15
#define lcd5110_LED      GPIO_Pin_12//  D8
#define lcd5110_PORT     GPIOB

#define SPI_lcd  SPI2
//#define SPIx_lcd  SPI1


 */
//****************************************************************************

//****************************************************************************

//*****************************************************************************

//*****************************************************************************

#define lcd5110_DC_H     GPIO_SetBits(lcd5110_PORT,lcd5110_DC)
#define lcd5110_DC_L     GPIO_ResetBits(lcd5110_PORT,lcd5110_DC)
#define lcd5110_RST_H    GPIO_SetBits(lcd5110_PORT,lcd5110_RST)
#define lcd5110_RST_L    GPIO_ResetBits(lcd5110_PORT,lcd5110_RST)
#define lcd5110_MOSI_H    GPIO_SetBits(lcd5110_PORT,lcd5110_MOSI)
#define lcd5110_MOSI_L    GPIO_ResetBits(lcd5110_PORT,lcd5110_MOSI)
#define lcd5110_SCLK_H   GPIO_SetBits(lcd5110_PORT,lcd5110_SCLK)
#define lcd5110_SCLK_L   GPIO_ResetBits(lcd5110_PORT,lcd5110_SCLK)
#define lcd5110_SCE_H     GPIO_SetBits(lcd5110_PORT,lcd5110_SCE)
#define lcd5110_SCE_L     GPIO_ResetBits(lcd5110_PORT,lcd5110_SCE)
#define lcd5110_LED_ON    GPIO_SetBits(lcd5110_PORT,lcd5110_LED)
#define lcd5110_LED_OFF    GPIO_ResetBits(lcd5110_PORT,lcd5110_LED)

//#define  lcd5110_write_dat		lcd5110_DC_L  //пишем данные
//#define  lcd5110_write_cmd		lcd15110_DC_H //пишем комманду
//#define  lcd5110_ON				lcd5110_SCE_L  //включим экран
//#define  lcd5110_OFF			lcd5110_SCE_H  //выключим

//************************************************************************//



#define PD_ON  		0x00       //вкл чип 0
#define PD_OFF 		0x04       //выкл чип 1
#define V_VER  		0x02       //адресса по вертикали 1
#define V_GOR  		0x00		//адресса по горизонтали  0
#define H_B    		0x00       //базовые комманды 0
#define H_H    		0x01       //расширенные команды 1

#define D_L    		0x00      //экран очищен
#define D_H    		0x04      //все точки вкл
#define E_L    		0x00	  //нет инверсии
#define E_H	   		0x01	  //инверси€ изобр
#define TC1_L  		0x00
#define TC1_H  		0x02
#define TC0_L  		0x00
#define TC0_H  		0x01


#define BIAS        0x10		//опорное напр€жение
#define TC			0x04      //температурный контроль
#define FSet		0x20      //направление авто перемещени€ курсора, по строке гор. или вертикатьно
#define VOP			0x80
#define D_blank		0x08|D_L|D_L    //очистить экран
#define D_normal	0x08|D_H|E_L		//норм состо€ние???
#define D_ALL		0x08|D_L|E_H		//все сигменты включены
#define D_INVERT    0x08|D_H|E_H		//инвертировать изображение


#define SetY        0x40                //установка курсора по Y (значени€ от 0-5)
#define SetX 		0x80				//установка курсора по X (0 to 83 (53H).)
#define NOP         lcd_write_cmd(0)//нет операции

//***********************************************************************//
u8 oscl[85];//дл€ бегущей строки
u8 x_oscl;//текущие координаты бег строки
u8 i_oscl;//дл€ массива
//*************************************************************************//

void gpio_spi_Init();
void lcd_wbyte ( uint8_t data);
//void lcd_rbyte ();
void lcd_write_cmd (uint8_t data);
void lcd_write_dat (uint8_t data);
void lcd_clear();
void init_lcd_5110 ();
void lcd_set_pos (uint8_t  y, uint8_t  x);
void lcd_set_sector(unsigned char y_pos,unsigned char  x_pos, unsigned char  y_s, unsigned char  x_s, unsigned char *data,_Bool invert);
//void lcd_set_char_5x8 (unsigned char y_pos,unsigned char  x_pos, unsigned char data ,_Bool invert);
//void lcd_set_str_5x8 (unsigned char y_pos,unsigned char  x_pos, unsigned char *data ,_Bool invert);
void lcd_set_char (unsigned char y_pos,unsigned char  x_pos, /*unsigned*/ char data ,_Bool invert);
//void lcd_set_st (unsigned char y_pos,unsigned char  x_pos, unsigned char  y_s, unsigned char  x_s, unsigned char *data ,_Bool invert);
void lcd_set_strs (unsigned char y_pos,unsigned char  x_pos, unsigned char R ,/*unsigned*/ char *data ,_Bool invert);
void lcd_set_rect (unsigned char y, unsigned char x ,unsigned char l,_Bool invert);//рисуем пр€моугольник висотой 8 пикселей
void Init_Timer2 ();
void TIM2_IRQHandler ();
u8  oscl_tr (u32 r, u16 d);
void oscl_set (u8 y);

