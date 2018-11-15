//**********************************//

//********************************///
//1 - GND
//2 - VCC
//3 - NC
//4  - /RST
//5  - /CS1
//6  - RS
//7  - RW_WR
//8  - E_RD
//9-14  DB0-DB5
//15-DB6 - CLK
//16-DB7 - SID
//17 - led+
//18 - led-

/*
#define lcd12864_RS       GPIO_Pin_9//  D6 rs   GPIO_Pin_10
#define lcd12864_RST      GPIO_Pin_8//  D4 reset GPIO_Pin_11
#define lcd12864_RW		  GPIO_Pin_7//  RW 7
#define lcd12864_SID      GPIO_Pin_15//MOSI--D16
#define lcd12864_SCLK     GPIO_Pin_13//SCK--D15

#define lcd12864_CS       GPIO_Pin_14//MICO
#define lcd12864_BLK      GPIO_Pin_12
#define lcd12864_PORT     GPIOB

#define SPI_lcd  SPI2
//#define SPIx_lcd  SPI1


 */
//****************************************************************************

//****************************************************************************

//*****************************************************************************

//*****************************************************************************
#define lcd12864_RS_H     GPIO_SetBits(lcd12864_PORT,lcd12864_RS)
#define lcd12864_RS_L     GPIO_ResetBits(lcd12864_PORT,lcd12864_RS)
#define lcd12864_RST_H    GPIO_SetBits(lcd12864_PORT,lcd12864_RST)
#define lcd12864_RST_L    GPIO_ResetBits(lcd12864_PORT,lcd12864_RST)
#define lcd12864_RW_H    GPIO_SetBits(lcd12864_PORT,lcd12864_RW)
#define lcd12864_RW_L    GPIO_ResetBits(lcd12864_PORT,lcd12864_RW)
#define lcd12864_SID_H    GPIO_SetBits(lcd12864_PORT,lcd12864_SID)
#define lcd12864_SID_L    GPIO_ResetBits(lcd12864_PORT,lcd12864_SID)
#define lcd12864_SCLK_H   GPIO_SetBits(lcd12864_PORT,lcd12864_SCLK)
#define lcd12864_SCLK_L   GPIO_ResetBits(lcd12864_PORT,lcd12864_SCLK)
#define lcd12864_CS_H     GPIO_SetBits(lcd12864_PORT,lcd12864_CS)
#define lcd12864_CS_L     GPIO_ResetBits(lcd12864_PORT,lcd12864_CS)
#define lcd12864_BLK_H    GPIO_SetBits(lcd12864_PORT,lcd12864_BLK)
#define lcd12864_BLK_L    GPIO_ResetBits(lcd12864_PORT,lcd12864_BLK)

#define  lcd12864_write_dat		lcd12864_RS_H;lcd12864_RW_L
#define  lcd12864_write_cmd
#define  lcd12864_read_stat

//************************************************************************
//#define cmd 0//0b11111000
//#define dat 1//0b11111010

#define RESETL 0xE2 //брос адрессов отображения
#define DISPOFF 0xAE // включить изображение
#define DISPON 0xAF // выключить изображение
#define LINE 0x40 // установить линию 0 первая +1 вторая до 63
#define RMODE 0x81 // set reference mode
#define PAG 0xB0 // выбор номера страници +1 первая, +2 вторая

#define MSB 0x10 // установка вертикального столбца
#define LSB 0x00 // установка вертикального столбца до 0,,,131

#define PGLP 0xA0 // страници с лева на право
#define PGPL 0xA1
#define COLUP  0xC0 // пишем сверху в низ
#define COLDOW 0xC8 //пишем снизу в верх

#define REVDISPOFF 0xA6
#define REVDISP 0xA7 //реверс изображения

#define ENTEROFF 0xA4//весь экран белые точки
#define ENTERON 0xA5//весь экран чёрные точк

#define STATICON 0xAD//изображение не меняется при изменении данных рег
#define STATICOFF 0xAC//изображение меняется вместе с изменением регистра

#define STATICLCD   lcd_write_cmd(STATICON);lcd_write_cmd(0x03)
#define NOSTATICLCD lcd_write_cmd(STATICOFF);lcd_write_cmd(0x00)

#define LCD bias 0xA2 //ыбор соотношеня напряжения ХХхх
#define	SetModifyRead	0xE0 //остановка приращения адресса но адрес увел
#define	ResetModifyRead 0xEE // отмена отмнены приращения


#define POWERC			0x0028// Power control 0000101CRF * control power circuite operation
#define RES			    0x0020 // /* Regulator resistor select 0000100CBA Regulator resistor select


#define LCD_W 128
#define LCD_H 64
#define invert_off 0
#define invert_on  1


u16 lcdstat;
//***********************************************************************//
/*unsigned char M_ [16] =
{0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00
};*/
//*************************************************************************//
void gpio_spi_Init();
void lcd_wbyte (uint16_t data);
void lcd_rbyte ();
void lcd_rstatus ();
void lcd_write_cmd (uint16_t data);
void lcd_write_dat (uint16_t data);
void lcd_clear();
void init_lcd_12864 ();
void lcd_set_pos (unsigned char  x, unsigned char  y);
void lcd_set_sector(unsigned char x_pos,unsigned char  y_pos, unsigned char  x_s, unsigned char  y_s, unsigned char *data,_Bool invert);
void lcd_set_char (unsigned char x_pos,unsigned char  y_pos, unsigned char  x_s, unsigned char  y_s, unsigned char data ,_Bool invert);
void lcd_set_st (unsigned char x_pos,unsigned char  y_pos, unsigned char  x_s, unsigned char  y_s, unsigned char *data ,_Bool invert);
void lcd_set_strs (unsigned char x_pos,unsigned char  y_pos, unsigned char R ,unsigned char *data ,_Bool invert);
void lcd_set_rect (u8 x, u8 y ,u8 l,_Bool invert);//рисуем прямоугольник висотой 8 пикселей
void rest_lcd_12864 ();
