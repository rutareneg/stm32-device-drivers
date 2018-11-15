
#include "stm32f10x_flash.h"
#include "flash.h"
#include "stm32f10x.h"

#define FLASH_PAGE_SIZE    ((uint16_t)0x400) //If a page is 1K
#define WRITE_START_ADDR   ((uint32_t)0x08008000)//The starting address is written
#define WRITE_END_ADDR     ((uint32_t)0x0800C000)//The end address
uint32_t EraseCounter = 0x00, Address = 0x00;//Erase count, write address
uint32_t Data = 0x3210ABCD;//The data to be written
uint32_t NbrOfPage = 0x00;//Recording to erase the pages

volatile FLASH_Status FLASHStatus;// = FLASH_COMPLETE;/*FLASH complete erasure marks*/



/**********************************************************************/
void FLASH_WriteHalfDate(uint32_t addr, uint16_t *p, u8 R)
{	FLASH_Unlock();
     FLASH_ClearFlag (FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);//—á–∏—Å—Ç–∏–º —Ñ–ª–≥–∏
       FLASHStatus = FLASH_ErasePage (addr);

    while  (R-- )
    {
    	FLASHStatus = FLASH_ProgramHalfWord(addr, *p++); addr += 2;

   }
    FLASH_Lock();
}

/**************************************************************************/
void FLASH_ReadHalfData(uint32_t addr , uint16_t *p, u8 R)
{
	 while(R--)
	    {
	     *(p++)=*((uint16_t*)addr); addr+=2;
	  }
}
//**********************************************************************/
void FLASH_WriteHalfDate8(uint32_t addr, uint8_t *p, u8 R)
{	FLASH_Unlock();
     FLASH_ClearFlag (FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);//—á–∏—Å—Ç–∏–º —Ñ–ª–≥–∏
       FLASHStatus = FLASH_ErasePage (addr);//‡·ÓÚ‡ÂÚ ÔË ÓÚÍÎ˛˜ÂÌÌÓÈ ÒÚË‡ÎÍË

    while  (R--)
    {FLASHStatus = FLASH_ProgramHalfWord(addr, *p++); addr +=2;

    }
    FLASH_Lock();
}
/***********************************************************************/

void FLASH_ReadHalfData8(uint32_t addr , uint8_t *p, u8 R)
{
	 while(R--)
	    {
	     *(p++)=*((uint8_t*)addr); addr++;
	   }
}




//*********************************************************************//
void Write__All()
{
  /*Unlock FLASH*/
  FLASH_Unlock();
  /*Calculate the number of FLASH pages need to erase */
  NbrOfPage = (WRITE_END_ADDR - WRITE_START_ADDR) / FLASH_PAGE_SIZE;
  /* Remove all hang flags */
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
  /* Erase the FLASH page*/
  for(EraseCounter = 0; (EraseCounter <NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
    {
      FLASHStatus = FLASH_ErasePage(WRITE_START_ADDR + (FLASH_PAGE_SIZE * EraseCounter));
    }
  /* Write FLASH  */
  Address = WRITE_START_ADDR;
  while((Address <WRITE_END_ADDR) && (FLASHStatus == FLASH_COMPLETE))
    {
      FLASHStatus = FLASH_ProgramWord(Address, Data);
      Address = Address + 4;
    }
  /* FLASH lock  */
  FLASH_Lock();
}
/*
Function: to write data to the specified address
Explanation of parameters: addr write FLASH pages first
          P is written to the address of the variable (in the array must be of type uint8_t, the number of elements must be an even number)
          Byte_Num is written to a variable number of bytes (must be even)
*/
  void FLASH_WriteByte(uint32_t addr, uint8_t *p, uint16_t Byte_Num)
  {
 	uint32_t HalfWord;
 	Byte_Num = Byte_Num/2;
 	FLASH_Unlock();
 	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
 	FLASH_ErasePage(addr);
 	while(Byte_Num --)
 	{
 		HalfWord=*(p++);
 		HalfWord|=*(p++)<<8;
 		FLASH_ProgramHalfWord(addr, HalfWord);
 		addr += 2;
 	}
 	FLASH_Lock();
 }
//„ÄÄ„ÄÄAn example:
//„ÄÄ„ÄÄuint8_t data[100];
//„ÄÄ„ÄÄFLASH_WriteByte(0x8000000 , data , 100);/*An array of data data is written to the FLASH*/
/*
Function: to read data from the specified address
Explanation of parameters: addr read from the FLASH address
          P reads to deposit the address of the variable (must be an array of type uint8_t)
          Byte_Num to the number of bytes read
*/
 void FLASH_ReadByte(uint32_t addr , uint8_t *p , uint16_t Byte_Num)
  {
    while(Byte_Num--)
    {
     *(p++)=*((uint8_t*)addr++);
   }
 }
 // An example:
 //  uint8_t data[101];
 //  FLASH_ReadByte(0x8000001 , data , 101);/*The data in the FLASH is read into the array data*/


