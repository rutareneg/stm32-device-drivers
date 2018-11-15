
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include<stm32f10x_rcc.h>
#include "stm32f10x_conf.h"
#include "delay.h"
#include "ds18b20.h"
#include "stm32f10x_usart.h"
#include "main.h"

//#include "lcd_44780.h"

/****************************************************
//ПРЯМО ЧЕРЕЗ ПОРТ
 * ***************************************************/
void PORT_DS18B20_IN()
{  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
   GPIO_InitTypeDef PORT_DS18B20_I;
   PORT_DS18B20_I.GPIO_Pin = DS18B20;
   PORT_DS18B20_I.GPIO_Speed = GPIO_Speed_50MHz;
   PORT_DS18B20_I.GPIO_Mode = GPIO_Mode_IN_FLOATING;
   GPIO_Init(PORT_DS18B20, &PORT_DS18B20_I);
}

void PORT_DS18B20_OUT()
{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
   GPIO_InitTypeDef PORT_DS18B20_O;
   PORT_DS18B20_O.GPIO_Pin = DS18B20;
   PORT_DS18B20_O.GPIO_Speed = GPIO_Speed_50MHz;
   PORT_DS18B20_O.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_Init(PORT_DS18B20, &PORT_DS18B20_O);
}

void RESET_DS18B20()
{
	PORT_DS18B20_OUT();
	GPIO_ResetBits(PORT_DS18B20,DS18B20);
	delay_us(750);
	GPIO_SetBits(PORT_DS18B20,DS18B20);
	PORT_DS18B20_IN();
	delay_us(15);
}

_Bool CHEK_DS18B20 (void)
{	 _Bool r = 0;
	u8 i = 0;
//	PORT_DS18B20_IN();
	while (GPIO_ReadInputDataBit(PORT_DS18B20,DS18B20)&& i < 240)
	{	i++;
		delay_us(1);}
	if (i>=200)	return r = 1;
	else i = 0;

	while (!GPIO_ReadInputDataBit(PORT_DS18B20,DS18B20)&& i < 240)
	{	i++;
		delay_us(1);}
	if (i>=240)	return r = 1;
	else return r = 0;
	}

void WRITE_BYTE_DS18B20 (u8 date)
{	u8 i,temp;
		PORT_DS18B20_OUT();
		 for (i=1;i<=8;i++)
			{
		        temp=date&0x01;
		        date=date>>1;
		        if (temp)
		        {
		        	GPIO_ResetBits(PORT_DS18B20,DS18B20);// Write 1
		            delay_us(2);
		            GPIO_SetBits(PORT_DS18B20,DS18B20);
		            delay_us(60);
		        }
		        else
		        {
		        	GPIO_ResetBits(PORT_DS18B20,DS18B20);// Write 0
		            delay_us(60);
		            GPIO_SetBits(PORT_DS18B20,DS18B20);
		            delay_us(2);
		        }
		    }
}


_Bool READ_BIT_DS18B20 (void)
{	_Bool r = 0;
	PORT_DS18B20_OUT();
		GPIO_ResetBits(PORT_DS18B20,DS18B20);
		delay_us(2);
		GPIO_SetBits(PORT_DS18B20,DS18B20);
		PORT_DS18B20_IN();
		delay_us(12);
		if(GPIO_ReadInputDataBit(PORT_DS18B20,DS18B20))  r = 1;
		else r=0;
		delay_us(50);
		return r;
	}

 u8 READ_BYTE_DS18B20 (void)
 {
	u8 i,date = 0;
	for (i=1;i<=8;i++)
	{
		if( READ_BIT_DS18B20()) date=((date>>1)|0x80 );
		else date = date>>1;
	}
	return date;
 }

 void START_DS18B20(void)// ds1820 start convert
 {
	 RESET_DS18B20();
	 CHEK_DS18B20();
     WRITE_BYTE_DS18B20(0xcc);// skip rom
     WRITE_BYTE_DS18B20(0x44);// convert
 }

 short GET_TEMP_DS18B20(void)
 {
     u8 temp;
     u8 TL,TH;
 	short tem;
    START_DS18B20 ();                    // ds1820 start convert
     RESET_DS18B20();
     CHEK_DS18B20();
     WRITE_BYTE_DS18B20(0xcc);// skip rom
     WRITE_BYTE_DS18B20(0xbe);// отдать темпер
     TL = READ_BYTE_DS18B20(); // LSB
     TH = READ_BYTE_DS18B20(); // MSB


     if(TH>7)
      {   TH=~TH;
         TL=~TL;
         temp = 0;
      }
     else temp=1;
     tem=TH;
     tem<<=8;
     tem+=TL;
     tem =(float)tem*0.625;
 	if(temp)return tem;
 	else return -tem;
 }


/****************************************************************************/
 //**************ЧЕРЕЗ USART***********************************************/
/******************************************************************************/



//настройка порта и усарта
 void Init_USART_GPIO_DS18B20()
 {
         GPIO_InitTypeDef GPIO_InitStruct;
      //   USART_InitTypeDef USART_InitStructure;


                 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
//Порты для USART2
                 GPIO_InitStruct.GPIO_Pin =  Tx_USART_DS18B20;
                 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
                 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
                 GPIO_Init(PORT_USART_DS18B20, &GPIO_InitStruct);
                 GPIO_InitStruct.GPIO_Pin = Rx_USART_DS18B20;
                 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
                 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
                 GPIO_Init(PORT_USART_DS18B20, &GPIO_InitStruct);

                 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
                 USART_ClearFlag(USART_DS18B20, USART_FLAG_TC);//сброс флага завершения передачи

         USART_Cmd(USART_DS18B20, ENABLE);

 }

_Bool Reset_USART_DS18B20()//возв 1 если ошибка датчика
 {
        u8 status;

         USART_InitTypeDef USART_InitStructure;
         USART_InitStructure.USART_BaudRate = 9600;
         USART_InitStructure.USART_WordLength = USART_WordLength_8b;
                 USART_InitStructure.USART_StopBits = USART_StopBits_1;
                 USART_InitStructure.USART_Parity = USART_Parity_No;
                 USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
                 USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
         USART_Init(USART_DS18B20, &USART_InitStructure);

         USART_SendData(USART_DS18B20, 0xF0);
         while (USART_GetFlagStatus(USART_DS18B20, USART_FLAG_TC) == RESET);
         	 	 	 USART_ClearFlag(USART_DS18B20, USART_FLAG_TC);
       status = USART_ReceiveData(USART_DS18B20);

       	   	   USART_InitStructure.USART_BaudRate = 115200;
              /*  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
                USART_InitStructure.USART_StopBits = USART_StopBits_1;
                USART_InitStructure.USART_Parity = USART_Parity_No;
                USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None ;
                USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx ;*/
                USART_Init(USART_DS18B20, &USART_InitStructure);

         if (0x80<=status && status <=0xE0 )
        	 return 1;
         return 0;
 }

_Bool Control_circut ()
{	GPIO_SetBits(PORT_USART_DS18B20, Rx_USART_DS18B20);
	if(GPIO_ReadInputDataBit(PORT_USART_DS18B20, Tx_USART_DS18B20)){return 1;}
	else {return 0;}
	}

void Write_USART_DS18B20(u8 cmd)
{	u8 i=0;
	for(i=0;i<8;i++)

	{	//if (cmd & 0x01){dd = 0xff;}
		//	else{dd = 0;}
		if (cmd & 0x01)USART_SendData(USART_DS18B20,0xFF);
		else USART_SendData(USART_DS18B20,0);
		cmd = cmd >> 1;
		while (USART_GetFlagStatus(USART_DS18B20, USART_FLAG_TC) == RESET);
		USART_ClearFlag(USART_DS18B20, USART_FLAG_TC);
	}
}

u8 Read_byt_USART_DS18B20()
{	u8 dd = 0,i=0;
USART_ClearFlag(USART_DS18B20, USART_FLAG_TC);
		for(i=0;i<8;i++)
	{		USART_ReceiveData (USART_DS18B20);
			USART_SendData(USART_DS18B20,0xff);
			while (USART_GetFlagStatus(USART_DS18B20, USART_FLAG_TC) == RESET);
			USART_ClearFlag(USART_DS18B20, USART_FLAG_TC);
			if ( USART_ReceiveData (USART_DS18B20) > 0xFC)
				dd = ((dd>>1)|0x80);
			else dd = dd>>1;
	}
		return dd;
}


void Start_USART_DS18B20()
{	Reset_USART_DS18B20();
	Write_USART_DS18B20(0xcc);//делать всем
	Write_USART_DS18B20(0x44);//измерить темпер

}

void Get_Rom_USART_DS18B20 (u8 *rom)
{	u8 i=0;
	Reset_USART_DS18B20();
	Write_USART_DS18B20(READ_ROM);// дай всесь ром
	USART_ReceiveData (USART_DS18B20);//чистим входящий регистр
	for( i=0;i < 8;i++)//читаем все 8 байт
	{(*rom++) = Read_byt_USART_DS18B20();}
}

void Set_Rom_USART_DS18B20 (u8 *rom) // обращается конкретно, по коду устройства, эй ты, вася!!!!
{	u8 i=0;
	Reset_USART_DS18B20();
	Write_USART_DS18B20(MATCH_ROM);//эй ты,
	for( i=0;i<8;i++)//пишим все 8 байт
	{ Write_USART_DS18B20 (*rom++);}
	//delay_us(100);
}

void Set_CFG_USART_DS18B20 (u8 *rom,u8 H_,u8 L_,u8 CFG)
{
	if (!rom){Reset_USART_DS18B20(); Write_USART_DS18B20(0xcc);}// делать всем
	 else  {Set_Rom_USART_DS18B20 (rom);}	// делай вася(отправляем код железки код железки)
		Write_USART_DS18B20(WRITE_SCRATCHPAD);//запиши в оперативу конфигурацию
		Write_USART_DS18B20(H_);//пишим верхний придел
		Write_USART_DS18B20(L_);//пишим нижний придел
		Write_USART_DS18B20(CFG);//пишим онфигурацию.
	if (!rom){Reset_USART_DS18B20(); Write_USART_DS18B20(0xcc);}// делать всем
	 else { Set_Rom_USART_DS18B20 (rom);}	// делай вася(отправляем код железки код железки
		Write_USART_DS18B20(COPY_SCRATCHPAD);//сохраняем данные в памяти датчика
		Reset_USART_DS18B20();//сброс обязателен т.к. пропускаем этап
		//проверки записанной информации
	}

void Get_CFG_USART_DS18B20 (u8 *rom, u8 *dat)
{	u8 i;

if (!rom){Reset_USART_DS18B20(); Write_USART_DS18B20(0xcc);}// делать всем
	 else { Set_Rom_USART_DS18B20 (rom);}	// делай вася(отправляем код железки код железки)
	Write_USART_DS18B20(REC_SCRATCHPAD);//епром в оперативы
	while(!Read_byt_USART_DS18B20()){};
	if (!rom){Reset_USART_DS18B20(); Write_USART_DS18B20(0xcc);}// делать всем
	 else { Set_Rom_USART_DS18B20 (rom);}	// делай вася(отправляем код железки код железки)
	Write_USART_DS18B20(READ_SCRATCHPAD);// отдать оперативу
	 	for( i=0;i < 9;i++)//читаем все 9 байт
	 		{(*dat++) = Read_byt_USART_DS18B20();}

	}
//*************************************************************************************************************//
	unsigned char iButtonCRC( unsigned char *code)
		{
			unsigned char j, i, Data, CRC_ = 0 ;
				_Bool out[8]={0,0,0,0,0,0,0,0};
					_Bool Cout=0;
					for (j = 0; j < 8; j++)
					{
						Data = *code++;
						for (i = 0; i < 8; i++)
						{
							Cout = out[7]^(Data & 0x01);Data>>=1;
							out[7]=out[6];
							out[6]=out[5];
							out[5]=(out[4]^Cout);
							out[4]=(out[3]^Cout);
							out[3]=out[2];
							out[2]=out[1];
							out[1]=out[0];
							out[0]= Cout;
						}

					}
					for (i = 0; i < 8; i++)
						if (out[i])CRC_ = (CRC_ << 1)|0x01;else CRC_ <<=1;
					return CRC_;
		}
//**************************************************************************************************************//
	unsigned char crc(char *code)
	{
	unsigned char CRC_,i,j,data;
	for(j=0;j<8;j++)
	{
	data = *code++;
	for(i=0;i<8;i++)
		{
		if((CRC_&0x01)^(data&0x01))
			{
			CRC_>>=1;
			CRC_^=0x8c;
			}
		else
			{
			CRC_>>=1;
			}
		data>>=1;
		}
	}
	return CRC_;
	}

//*************************************************************************************************************//

	_Bool crc_check(unsigned char *code)
		{
		unsigned char CRC_,i,j,data;
		for(j=0;j<9;j++)
		{
		data = *code++;
		for(i=0;i<8;i++)
			{
			if((CRC_&0x01)^(data&0x01))
				{
				CRC_>>=1;
				CRC_^=0x8c;
				}
			else
				{
				CRC_>>=1;
				}
			data>>=1;
			}
		}
		if(CRC_ == 0)return 0;
		else  return 1;
		}
//*************************************************************************************************************//




//************************************************************************************************************//
void GET_RAM_USART_DS18B20(u8 rom[],u8 *dat)// rom-код устройства..dat- туда складываются данные с устройства
 {	u8 i=0;
    Reset_USART_DS18B20();
 	Start_USART_DS18B20();         //Измерить всем
 	//delay_ms(800);			//ждём пока измерит
 if (!rom){Reset_USART_DS18B20(); Write_USART_DS18B20(0xcc);}// делать всем
 else  {Reset_USART_DS18B20(); Set_Rom_USART_DS18B20 (rom);}	// делай вася(отправляем код железки код железки)
 	Write_USART_DS18B20(READ_SCRATCHPAD);// отдать оперативу
 	for( i=0;i < 9;i++)//читаем все 9 байт
 		{(*dat++) = Read_byt_USART_DS18B20();}
 }

_Bool CHEK_TEMP_DS18B20 (u8 TH, u8 TL)//старший байт младший байт из data возвращает 1 при отрицательной температуре
{			if(TH >= 7)
	      {   TH = ~TH;
	         TL = ~TL;
	         return 1;
	      }
			return 0;
}

float CONV_TEMP_DS18B20 (float C, u8 TH, u8 TL)//ПЕРЕСЧЁТ В ГРАДУСЫ,С кофф,
{	float T=0;
	u16	R=0;
	_Bool m = 0;
	//m = CHEK_TEMP_DS18B20 (TH, TL);//определяем отрицательное значение

		R = TH;
		R <<=8;
		R += TL;
		if(R > 0x07D0 ){R =(~ R);R++; m = 1;}
		T = (float)R * C;

	if(m)return -T;
		return T;
	}


//перреводим градусы в байты
void TEMP_CONF_DS18B20 (u16 T,float C,u8 *R)//темпер,коф приращения,рег для загрузки

{	u16 TR = T/C;
	*R++;*R++;
	*R++ = TR>>8;

	*R++ = TR & 0xFF;

	}
//	определяес коф приращения от настройки датчика
void COF_DS18B20 (float COF)
{	u8 C = COF *100;
	switch(C)
        {
        case(50):
		CFG_ = 0x1F ;
        break;
        case(25):
		CFG_ = 0x3F;
        break;
        case(12):
		CFG_ = 0x5F;
        break;
        case(6):
		CFG_ = 0x7F;
        break;

        }
	}
///*************************************************************//


//**************************************************************//
