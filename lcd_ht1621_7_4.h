


 //Defined HT1621's command

      #define  ComMode    0x52  //4COM,1/3bias  1000    010 1001  0
      #define  RCosc      0x30  //on-chip RC oscillator(Power-on default)1000 0011 0000
      #define  LCD_on     0x06  //Turn on LCD
      #define  LCD_off    0x04  //Turn off LCD
      #define  Sys_en     0x02  //Turn on system oscillator 1000   0000 0010
      #define  CTRl_cmd   0x80  //Write control cmd
      #define  Data_cmd   0xa0  //Write data cmd

      // //Define port    HT1621 data port
//	#define LCDPORT      GPIO_PORTA
//	#define DATA         GPIO_PIN_9
//	#define CS    		 GPIO_PIN_10
	//#define RD           GPIO_PIN_10
//	#define WR           GPIO_PIN_11
	//*************************************************************//
	#define  delay_bit	delay_us(500)

    #define CS_LOW   GPIO_ResetBits(LCDPORT, CS); delay_bit
	#define CS_HIGH  GPIO_SetBits(LCDPORT, CS); delay_bit

	#define WR_LOW   GPIO_ResetBits(LCDPORT, WR); delay_bit
	#define WR_HIGH  GPIO_SetBits(LCDPORT, WR); delay_bit

	#define DATA_LOW   GPIO_ResetBits(LCDPORT, DATA); delay_bit
	#define DATA_HIGH  GPIO_SetBits(LCDPORT, DATA); delay_bit

	//*************************************************************//
		  //режим данных
#define set_dat   CS_LOW; WR_LOW; DATA_HIGH; WR_HIGH; WR_LOW; DATA_LOW; WR_HIGH; WR_LOW;DATA_HIGH; WR_HIGH;

		//режим комманд

#define set_com	 CS_LOW; WR_LOW; DATA_HIGH; WR_HIGH; WR_LOW;DATA_LOW; WR_HIGH; WR_LOW; WR_HIGH;

	//******************************************************************//
	void LCDPORT_INI();
	void lcd_init(void);
	void lcd_clr();
	void set_lcd_com(unsigned char com);
	void set_lcd_data(unsigned short int address,unsigned short int data);
	void all_lcd_data(unsigned char temp[],u8 R, _Bool sek);


