

#include "stm32f10x.h"
#include "delay.h"
#include  "stm32f10x_usart.h"

#define config_ds18b20 0xff
/***************************************/
//В порт на прямую
/***************************************/
#define PORT_DS18B20  GPIOB
#define DS18B20       GPIO_Pin_11

void PORT_DS18B20_IN();
void PORT_DS18B20_OUT();
void RESET_DS18B20();
_Bool CHEK_DS18B20 (void);
void WRITE_BYTE_DS18B20 (u8 date);
_Bool READ_BIT_DS18B20 (void);
u8 READ_BYTE_DS18B20 (void);
void START_DS18B20(void);
short GET_TEMP_DS18B20(void);

/**************************************/
//через Usart
/*************************************/
/*
 * #define USART_DS18B20   USART3
#define Rx_USART_DS18B20 GPIO_Pin_11
#define Tx_USART_DS18B20 GPIO_Pin_10
#define PORT_USART_DS18B20 GPIOB

*/

#define SEARCH_ROM   0xF0 // идентифицировать коды ROM всех ведомых устройств на шине.
#define READ_ROM     0x33 // один ведомый. Она позволяет мастеру шины читать 64-разрядный код ROM ведомого
#define MATCH_ROM    0x55  //сопровождаемая 64-разрядным кодом ROM, позволяет мастеру шины обращаться к определенному ведомому на многоточечной или одноточечной шине
#define SKIP_ROM     0xCC // для одновременного обращения ко всем устройствам на шине, не передавая информацию о коде ROM
#define ALARM_SEARCH 0xEC // ответят только ведомые с установленным флагом тревоги
#define CONVERT_T    0x44 // инициализирует одно преобразование температуры
#define WRITE_SCRATCHPAD 0x4E //Записывать 3 байта данных оперативную память DS18B20
#define READ_SCRATCHPAD 0xBE //читать содержимое оперативной памят
#define COPY_SCRATCHPAD 0x48 //копирует содержимое оперативных регистров TH, TL и конфигурации (байты 2, 3 и 4 в EEPROM.
#define REC_SCRATCHPAD    0xB8 //копирует значения памяти  и конфигурации из EEPROM  в регистры 2, 3, и 4 оперативной памяти, соответственно
#define READ_POWER_SUPPLY 0xB4 //присутствуют ли на шине DS18B20 с паразитным питанием.
unsigned char code[8];//сюды номер ром код
unsigned char data[9];//всякие данные
_Bool   cont_circut; //для определения типа подключения датчика

#define  COF_P  0.0625 //коф приращения датчика  0.5°C, 0.25°C, 0.125°C, и 0.0625°C
#define  COF_AP  0.125	//коф приращения  для расчёта тревоги
#define	TL_  data[0]//темпер
#define	TH_  data[1]//темпер
#define	ATH_  data[2]//контрольная темпер верхнего придела
#define	ATL_  data[3]//контрольная температура нижнего придела
#define CFG_  data[4]//байт конфигурации
#define RCRC  data[8]//контрольная сумма
#define	AFTH_  data[2]&0x80//флаг верхнего придела
#define	AFTL_  data[3]&0x80//флаг нижнего придела






void Init_USART_GPIO_DS18B20();
_Bool Reset_USART_DS18B20();
void Write_USART_DS18B20(u8 cmd);
u8 Read_byt_USART_DS18B20();
void Start_USART_DS18B20();
void Get_Rom_USART_DS18B20 (u8 *rom);
void Set_Rom_USART_DS18B20 (u8 *rom);
void GET_RAM_USART_DS18B20(u8 rom[],u8 *dat);
unsigned char iButtonCRC( unsigned char *code);
void Set_CFG_USART_DS18B20 (u8 *rom,u8 H_,u8 L_,u8 CFG);
_Bool CHEK_TEMP_DS18B20 (u8 TH, u8 TL);
float CONV_TEMP_DS18B20 (float C, u8 TH, u8 TL);//ПЕРЕСЧЁТ В ГРАДУСЫ
void TEMP_CONF_DS18B20 (u16 T,float C,u8 *R);
void COF_DS18B20 (float COF);//определяем приращение по конфигурации
void Get_CFG_USART_DS18B20 (u8 *rom, u8 *dat);
_Bool Control_circut ();
unsigned char crc(char *code);
_Bool crc_check(unsigned char *code);

