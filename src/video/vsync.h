#pragma once

#define VSYNC_PORT				GPIOC
#define VSYNC_PORT_CLOCK    	RCC_AHB1Periph_GPIOC
#define VSYNC_PIN				GPIO_Pin_6
#define VSYNC_PINSOURCE			GPIO_PinSource6
#define VSYNC_PIN_AF			GPIO_AF_TIM3
#define VSYNC_TIMER				TIM3
#define VSYNC_TIMER_CLOCK		RCC_APB1Periph_TIM3

#define VSYNC_TIMER_CC2 		1

#define VSYNC_TIMER_CC_IRQ  	TIM3_IRQn

void VSYNC_GPIO_Config(void);
void VSYNC_TIMER_Config(uint16_t vsyncTimerPeriod,
						uint16_t vsyncTimerPrescaler,
						uint16_t vsyncSyncLength,
						enum e_videoSyncPolarity syncPolarity);
