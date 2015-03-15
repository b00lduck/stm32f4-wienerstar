#include "stm32f4xx_iwdg.h"

void reset(void) {
	 IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	 IWDG_SetPrescaler(IWDG_Prescaler_8);
	 IWDG_SetReload(0x0FFF);
	 IWDG_ReloadCounter();
	 IWDG_Enable();

	 while (1) {
		 // auf wiedersehen (this triggers the watchdog)
	 }
}
