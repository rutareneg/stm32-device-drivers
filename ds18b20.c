
#include "ds18b20.h"

#include <main.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_usart.h>

#include "../delay/delay.h"


//#include "lcd_44780.h"

/****************************************************
//����� ����� ����
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
     WRITE_BYTE_DS18B20(0xbe);// ������ ������
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
 //**************����� USART***********************************************/
/******************************************************************************/



//��������� ����� � ������
 void Init_USART_GPIO_DS18B20()
 {
         GPIO_InitTypeDef GPIO_InitStruct;
      //   USART_InitTypeDef USART_InitStructure;


                 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
//����� ��� USART2
                 GPIO_InitStruct.GPIO_Pin =  Tx_USART_DS18B20;
                 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
                 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
                 GPIO_Init(PORT_USART_DS18B20, &GPIO_InitStruct);
                 GPIO_InitStruct.GPIO_Pin = Rx_USART_DS18B20;
                 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
                 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
                 GPIO_Init(PORT_USART_DS18B20, &GPIO_InitStruct);

                 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
                 USART_ClearFlag(USART_DS18B20, USART_FLAG_TC);//����� ����� ���������� ��������

         USART_Cmd(USART_DS18B20, ENABLE);

 }

_Bool Reset_USART_DS18B20()//���� 1 ���� ������ �������
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
        	 {return 1;}
         return 0;
 }

_Bool Control_circut ()
{	GPIO_SetBits(PORT_USART_DS18B20, Rx_USART_DS18B20);
	if(GPIO_ReadInputDataBit(PORT_USART_DS18B20, Tx_USART_DS18B20)){return 1;}
	else {return 0;}
	}

void Write_bit_USART_DS18B20(u8 cmd)//����� 1 ���
{
		if (cmd)USART_SendData(USART_DS18B20,0xFF);
		else USART_SendData(USART_DS18B20,0);
		while (USART_GetFlagStatus(USART_DS18B20, USART_FLAG_TC) == RESET);
		USART_ClearFlag(USART_DS18B20, USART_FLAG_TC);
}

void Write_USART_DS18B20(u8 cmd)//����� 8 ���
{   for(u8 i=0;i<8;i++)
	{
		if (cmd & 0x01)USART_SendData(USART_DS18B20,0xFF);
		else USART_SendData(USART_DS18B20,0x0);
		cmd = cmd >> 1;
		while (USART_GetFlagStatus(USART_DS18B20, USART_FLAG_TC) == RESET);
		USART_ClearFlag(USART_DS18B20, USART_FLAG_TC);

	}
}

void prog_rw_byt(u8 cmd)//����� 8 ���
{

	cmd = ~cmd;

	for(u8 i=0;i<8;i++)
	{
		if (cmd & 0x01)USART_SendData(USART_DS18B20,0xFF);
		else USART_SendData(USART_DS18B20,0x0);
		cmd = cmd >> 1;
		while (USART_GetFlagStatus(USART_DS18B20, USART_FLAG_TC) == RESET);
		USART_ClearFlag(USART_DS18B20, USART_FLAG_TC);
		delay_ms(10);
	}
}

void prog_USART_byts(u8 *cmd)//����� R ���

	{
		for(u8 i=0;i<8;i++)
			{
			prog_rw_byt(*cmd++);
			}
	}


void Write_byts_USART_1WIRE(u8 *cmd, u8 R)//����� R ���

{u8 i;
	for( i=0;i<R;i++)
		{
	Write_USART_DS18B20(*cmd++);
		}
}


u8 Read_byt_USART_DS18B20()
{	u8 dd = 0,i=0;
USART_ClearFlag(USART_DS18B20, USART_FLAG_TC);
	USART_ReceiveData (USART_DS18B20);//������ ��������
		for(i=0;i<8;i++)
	{		USART_SendData(USART_DS18B20,0xff);
			while (USART_GetFlagStatus(USART_DS18B20, USART_FLAG_TC) == RESET);
			USART_ClearFlag(USART_DS18B20, USART_FLAG_TC);
			if ( USART_ReceiveData (USART_DS18B20) > 0xFC)
				dd = ((dd>>1)|0x80);
			else dd = dd>>1;
	}
		return dd;
}
//***************************************************************************************************************************//
u8 Read_bits_USART_DS18B20(u8 R)//����������� ��� ��� ������
{
	u8 dd = 0,i=0;
	USART_ClearFlag(USART_DS18B20, USART_FLAG_TC);
	USART_ReceiveData (USART_DS18B20);//������ ��������
			for(i=0;i<R;i++)
		{		USART_SendData(USART_DS18B20,0xff);
				while (USART_GetFlagStatus(USART_DS18B20, USART_FLAG_TC) == RESET);
				USART_ClearFlag(USART_DS18B20, USART_FLAG_TC);
				if ( USART_ReceiveData (USART_DS18B20) > 0xFC)
					dd = ((dd>>1)|0x80);
				else dd = dd>>1;
		}
			return dd;
}

//******************************************************************************************************************************//

//*****************************************************************************************************************************//
void Start_USART_DS18B20()
{	Reset_USART_DS18B20();
	Write_USART_DS18B20(0xcc);//������ ����
	Write_USART_DS18B20(0x44);//�������� ������

}

void Get_Rom_USART_DS18B20 (u8 *rom)
{	u8 i=0;
	Reset_USART_DS18B20();
	Write_USART_DS18B20(READ_ROM);// ��� ����� ���
	for( i=0;i < 8;i++)//������ ��� 8 ����
	{(*rom++) = Read_byt_USART_DS18B20();}
}

void Set_Rom_USART_DS18B20 (u8 *rom) // ���������� ���������, �� ���� ����������, �� ��, ����!!!!
{	u8 i=0;
	Reset_USART_DS18B20();
	Write_USART_DS18B20(MATCH_ROM);//�� ��,
	for( i=0;i<8;i++)//����� ��� 8 ����
	{ Write_USART_DS18B20 (*rom++);}
	//delay_us(100);
}

void Set_CFG_USART_DS18B20 (u8 *rom,u8 H_,u8 L_,u8 CFG)
{
	if (!rom){Reset_USART_DS18B20(); Write_USART_DS18B20(0xcc);}// ������ ����
	 else  {Set_Rom_USART_DS18B20 (rom);}	// ����� ����(���������� ��� ������� ��� �������)
		Write_USART_DS18B20(WRITE_SCRATCHPAD);//������ � ��������� ������������
		Write_USART_DS18B20(H_);//����� ������� ������
		Write_USART_DS18B20(L_);//����� ������ ������
		Write_USART_DS18B20(CFG);//����� �����������.
	if (!rom){Reset_USART_DS18B20(); Write_USART_DS18B20(0xcc);}// ������ ����
	 else { Set_Rom_USART_DS18B20 (rom);}	// ����� ����(���������� ��� ������� ��� �������
		Write_USART_DS18B20(COPY_SCRATCHPAD);//��������� ������ � ������ �������
		Reset_USART_DS18B20();//����� ���������� �.�. ���������� ����
		//�������� ���������� ����������
	}

void Get_CFG_USART_DS18B20 (u8 *rom, u8 *dat)
{	u8 i;

if (!rom){Reset_USART_DS18B20(); Write_USART_DS18B20(0xcc);}// ������ ����
	 else { Set_Rom_USART_DS18B20 (rom);}	// ����� ����(���������� ��� ������� ��� �������)
	Write_USART_DS18B20(REC_SCRATCHPAD);//����� � ���������
	while(!Read_byt_USART_DS18B20()){};
	if (!rom){Reset_USART_DS18B20(); Write_USART_DS18B20(0xcc);}// ������ ����
	 else { Set_Rom_USART_DS18B20 (rom);}	// ����� ����(���������� ��� ������� ��� �������)
	Write_USART_DS18B20(READ_SCRATCHPAD);// ������ ���������
	 	for( i=0;i < 9;i++)//������ ��� 9 ����
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


//*************************************************************************************************************//
	unsigned char chek_crc( unsigned char *code, unsigned char R )
	{
	  unsigned char data,crc,d,byt;
	  byt=0; crc=0;
	  do{
	    data=code[byt];
	    for(u8 i=0; i<8; i++) {  // ������� ����� � �����
	      d = crc ^ data;
	      d &= 1;
	      crc >>= 1;
	      data >>= 1;
	      if( d == 1 ) crc ^= 0x8c; // �������
	    }
	    byt++;
	  } while( byt < R ); // ������� ������ � �������
	  return crc;
	}

//*************************************************************************************************************//





//************************************************************************************************************//
void GET_RAM_USART_DS18B20(u8 rom[],u8 *dat)// rom-��� ����������..dat- ���� ������������ ������ � ����������
 {	u8 i=0;

 	Start_USART_DS18B20();         //�������� ����

 if (!rom){/*Reset_USART_DS18B20();*/ Write_USART_DS18B20(0xcc);}// ������ ����
 else  {/*Reset_USART_DS18B20();*/ Set_Rom_USART_DS18B20 (rom);}	// ����� ����(���������� ��� ������� ��� �������)
 	Write_USART_DS18B20(READ_SCRATCHPAD);// ������ ���������
 	for( i=0;i < 9;i++)//������ ��� 9 ����
 		{(*dat++) = Read_byt_USART_DS18B20();}
 }

_Bool CHEK_TEMP_DS18B20 (u8 TH, u8 TL)//������� ���� ������� ���� �� data ���������� 1 ��� ������������� �����������
{			if(TH >= 7)
	      {   TH = ~TH;
	         TL = ~TL;
	         return 1;
	      }
			return 0;
}

float CONV_TEMP_DS18B20 (float C, u8 TH, u8 TL)//�����ר� � �������,� ����,
{	float T=0;
	u16	R=0;
	_Bool m = 0;
	//m = CHEK_TEMP_DS18B20 (TH, TL);//���������� ������������� ��������

		R = TH;
		R <<=8;
		R += TL;
		if(R > 0x07D0 ){R =(~ R);R++; m = 1;}
		T = (float)R * C;

	if(m)return -T;
		return T;
	}


//���������� ������� � �����
void TEMP_CONF_DS18B20 (u16 T,float C,u8 *R)//������,��� ����������,��� ��� ��������

{	u16 TR = T/C;
	*R++;*R++;
	*R++ = TR>>8;

	*R++ = TR & 0xFF;

	}
//	���������� ��� ���������� �� ��������� �������
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
//**************************************************************************************************************************************//
///*********************���� ���������**************************//
u8 Skan_1_wire()
{
u8 bits=0;//2 ���� ������
u8 C=0;//����� ���� ������
u8 R=0;//����� ������� �������
u8 M[]={0,0,0,0,0,0,0,0,0,0};//������ ��� ��������
u8 rm=0; //������� �� 64
u8 div=0;//����������� ���������

while(1){

				//if(!Reset_USART_DS18B20()){return 0;}//������ ��� ���������
				Reset_USART_DS18B20();
				Write_USART_DS18B20(SEARCH_ROM);//������� ������ ��� ����
while (rm<64)
	{
		bits = Read_bits_USART_DS18B20(2);// 2 ����������� ��� ��� ������, ������ 2 ����

		   if(bits < 0xC0)
		   {//���� ��� ������� ��������� 0x80 ��� �� ���� �� "0", 0�81 -��� ��  1
			if(bits>0) {Write_bit_USART_DS18B20(bits & 0x40); Load_bit_skan(bits & 0x40, C); C++; rm++;}
			else//���� 0 � 0 �� ������, ��������� ��� �� ������ � ���� ���
				{
					if(M[R]==0){ M[R]=0x80;
										Write_bit_USART_DS18B20(0); Load_bit_skan(0, C);}
					else//������� ��� ������
				  {
					if(M[R]==0x81){Write_bit_USART_DS18B20(1); Load_bit_skan(1, C);}//��� �� 1
					if(M[R]==0x80){Write_bit_USART_DS18B20(0); Load_bit_skan(0, C);}//
				  }
			       C++;  rm++;R++;
				}// ��� ������� ���������� �� ��� ����

		   } else {return 0;}//(192/C0) 1�1 (������) ��� ������ ���� �����������

	}
					div++;//��������� �����������
					rm = 0; // �������� ������� ����� ������ ���, ������� ���������.

						while(M[R-1]==0x81)
							{
							M[R-1]=0; R--;//������� ������

							}
						//if(M[R]==0x81) {M[R]= 0;}
						if(M[R-1]==0x80) {M[R-1]= 0x81;}//����������� ������ � 0 �� 1

						if(!R){return div;}//��� ������� ����������� �������, ���������� ����� ���������
						R=0;


				}

	}//��������� ����� ������� ������� � 1





//************************************************************************//

//************************************************************************//
void Load_bit_skan(u8 i, u8 C) //I ��������, R ����� ����
{
	u8 t=C/8;

	if(i){skan[t]>>=1;
		skan[t]|=0x80;}
	else skan[t]>>=1;
	//skan[t]=skan[t]>>1;
}
//************************************************************************//

