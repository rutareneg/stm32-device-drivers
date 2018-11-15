
#include "stm32f10x_iwdg.h"
#include "wdt.h"
#include "stm32f10x.h"

void iwdt_init (u8 dl,u16 schet)
{
	IWDG_WriteAccessCmd(0x5555);//ключ доступа к таймеру
	IWDG_SetPrescaler(dl);//установка пред делителя
	IWDG_SetReload(schet);//устанавливаем значение счётчика тайсера
	IWDG_ReloadCounter(); //перезагрузка счетчика
	IWDG_Enable();//вкл сторожа
}
