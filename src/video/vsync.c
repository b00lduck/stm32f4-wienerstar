#include "main.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "video.h"
#include "hsync.h"
#include "vsync.h"

/**
 * Activate Clock and GPIO for VSYNC
 */
void VSYNC_GPIO_Config(void) {

	GPIO_InitTypeDef GPIO_InitStructure;

	// Timer clock enable
	RCC_APB1PeriphClockCmd(VSYNC_TIMER_CLOCK, ENABLE);

	// GPIO clock enable
	RCC_AHB1PeriphClockCmd(VSYNC_PORT_CLOCK, ENABLE);

	// GPIO configuration
	GPIO_InitStructure.GPIO_Pin = VSYNC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(VSYNC_PORT, &GPIO_InitStructure);

	GPIO_PinAFConfig(VSYNC_PORT, VSYNC_PINSOURCE, VSYNC_PIN_AF);
}

/**
 * Configure the Timer for the VSYNC
 *
 * Use PWM on CC1 to generate sync-impulse
 * Use UPDATE interrupt to enable CC2 INT of HSYNC timer
 *
 * @param vsyncTimerPeriod		 length of one timer period in ticks
 * @param vsyncTimerPrescaler	 timer prescaler
 * @param hsyncSyncLength	     length of the sync impulse in timer ticks
 * @param syncPolarity 			 NEGATIVE | POSITIVE
 *
 */
void VSYNC_TIMER_Config(uint16_t vsyncTimerPeriod,
		                uint16_t vsyncTimerPrescaler,
		                uint16_t vsyncSyncLength,
		                enum e_videoSyncPolarity syncPolarity) {

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// Time base configuration
	TIM_TimeBaseStructure.TIM_Period = vsyncTimerPeriod;
	TIM_TimeBaseStructure.TIM_Prescaler = vsyncTimerPrescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(VSYNC_TIMER, &TIM_TimeBaseStructure);

	// PWM1 Mode configuration: Channel1
	// End of Sync impulse
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = vsyncSyncLength;
	if (syncPolarity == NEGATIVE) {
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	} else {
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	}
	TIM_OC1Init(VSYNC_TIMER, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(VSYNC_TIMER, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(VSYNC_TIMER, ENABLE);

    TIM_SelectOutputTrigger(VSYNC_TIMER,  TIM_TRGOSource_Update);

    // Enable MS Mode
    TIM_SelectMasterSlaveMode(HSYNC_TIMER, TIM_MasterSlaveMode_Enable);

	// Enable the global interrupts
	NVIC_InitStructure.NVIC_IRQChannel = VSYNC_TIMER_CC_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 15;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// enable interrupts
	TIM_ITConfig(VSYNC_TIMER, TIM_IT_Update, ENABLE);
}

/**
 * IRQ Handler for TIM3
 *
 * TIM_IT_Update only
 */
void TIM3_IRQHandler(void) {

	HSYNC_TIMER->CNT = 0;
	HSYNC_TIMER->DIER |= TIM_IT_CC2; 	// enable CC2

	TIM_ClearITPendingBit(VSYNC_TIMER, TIM_IT_Update);

	// Update: Start of frame
	videoInstance.current_y = -35; // 2 sync + 25 back porch + 8 top border

	// increment frame counter
	videoInstance.current_frame++;

	// timecode for cool display
	videoInstance.tc_frame++;
	if (videoInstance.tc_frame > 60) {
		videoInstance.tc_frame = 0;
		videoInstance.tc_sec++;
		if (videoInstance.tc_sec > 60) {
			videoInstance.tc_sec = 0;
			videoInstance.tc_min++;
		}
	}

}
