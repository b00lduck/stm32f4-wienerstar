#include "main.h"
#include "video.h"
#include "dma.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "hsync.h"
#include "effects/fixedFont.h"

/**
 * Activate Clock and GPIO for VSYNC
 */
void HSYNC_GPIO_Config(void) {

	GPIO_InitTypeDef GPIO_InitStructure;

	// TIM1 clock enable
	RCC_APB2PeriphClockCmd(HSYNC_TIMER_CLOCK, ENABLE);

	// GPIO clock enable
	RCC_AHB1PeriphClockCmd(HSYNC_PORT_CLOCK, ENABLE);

	// GPIO configuration
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(HSYNC_PORT, &GPIO_InitStructure);

	// Connect TIM1 pins to alternate function
	GPIO_PinAFConfig(HSYNC_PORT, HSYNC_PINSOURCE, HSYNC_PIN_AF);
}

void VideoSwitch_GPIO_Config(void) {

	GPIO_InitTypeDef GPIO_InitStructure;

	// GPIO clock enable
	RCC_AHB1PeriphClockCmd(VIDEO_SWITCH_PORT_CLOCK, ENABLE);

	// GPIO configuration
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(VIDEO_SWITCH_PORT, &GPIO_InitStructure);


}

/**
 * Configure the Timer for the HSYNC
 *
 * Use PWM to generate HSYNC-impulse
 * Use CC2 to signal start of visible line
 *
 * @param pixelDmaStartOffset	 start of pixel DMA in timer ticks from start of line
 * @param hsyncTimerPeriod		 length of one timer period in ticks
 * @param hsyncSyncLength	     length of the sync impulse in timer ticks
 * @param syncPolarity 			 NEGATIVE | POSITIVE
 *
 */
void HSYNC_TIMER_Config(uint16_t pixelDmaStartOffset,
						uint16_t hsyncTimerPeriod,
						uint16_t hsyncSyncLength,
						enum e_videoSyncPolarity syncPolarity) {

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// Time base configuration
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = hsyncTimerPeriod;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(HSYNC_TIMER, &TIM_TimeBaseStructure);

	// PWM1 Mode configuration: Channel1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse = hsyncSyncLength;
	if (syncPolarity == NEGATIVE) {
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	} else {
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	}
	TIM_OC1Init(HSYNC_TIMER, &TIM_OCInitStructure);

	//
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse = HSYNC_TIMER_CC2;
	TIM_OC2Init(HSYNC_TIMER, &TIM_OCInitStructure);

	// Master Mode for TIM1 (PIXEL DMA TIMER)
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = pixelDmaStartOffset;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OC4Init(HSYNC_TIMER, &TIM_OCInitStructure);
    TIM_SelectOutputTrigger(HSYNC_TIMER,  TIM_TRGOSource_OC4Ref);

	// Be a slave of TIM3
	TIM_SelectInputTrigger(HSYNC_TIMER, TIM_TS_ITR2);
    TIM_SelectSlaveMode(HSYNC_TIMER, TIM_SlaveMode_Trigger);

    // Enable MS Mode
    TIM_SelectMasterSlaveMode(HSYNC_TIMER, TIM_MasterSlaveMode_Enable);

	// Automatic Output enable, Break, dead time and lock configuration
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
	TIM_BDTRInitStructure.TIM_DeadTime = 0;
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
    TIM_BDTRConfig(HSYNC_TIMER, &TIM_BDTRInitStructure);

	// Enable the global Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = HSYNC_TIMER_CC_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Interrupts enable
	TIM_ITConfig(HSYNC_TIMER, TIM_IT_CC2, ENABLE);

	// enable the counter
	TIM_Cmd(HSYNC_TIMER, DISABLE);

	// Main Output Enable
	TIM_CtrlPWMOutputs(HSYNC_TIMER, ENABLE);
}

/**
 * prepare the DMA for one line
 */
static inline void videoPrepLineDma(uint32_t addr) {

	DMA2_Stream1->M0AR = addr;
	DMA2_Stream1->NDTR = videoInstance.resx;

	DMA2_Stream1->CR |= DMA_Channel_7 | DMA_Priority_VeryHigh | DMA_MemoryDataSize_Word | DMA_PeripheralDataSize_Byte |
						DMA_MemoryInc_Enable | DMA_DIR_MemoryToPeripheral | DMA_SxCR_TCIE;

	DMA2_Stream1->CR |= DMA_SxCR_EN;

	// reset the timer
	PIXELDMA_TIMER->CNT = 0;

}

/**
 * graphics mode
 */
static inline void HsyncIsrPixelMode(void) {

	if (videoInstance.current_y >= 0) {

		// prepare DMA settings for pixel DMA
		// it is started later via timer sync
		// doubleScanline is 1 in case of half vertical resolution. Every line is displayed twice.
		if (videoInstance.current_y < videoInstance.hardwareResY) {
			uint32_t dma_start = (uint32_t)videoInstance.vramDisplay + ( ( videoInstance.current_y >> videoInstance.doubleScanline ) * videoInstance.resx );
			videoPrepLineDma(dma_start);
		} else if (videoInstance.current_y < videoInstance.lowerEndOfFrame) {  //  480 image + 8 bottom border
			VIDEO_PORT->ODR = 0x00;
			TIM_ITConfig(HSYNC_TIMER, TIM_IT_CC2, DISABLE); // no more DMA
			videoInstance.vblank_flag = 1;
		}
	}
}

char *textRowPointer;
uint8_t *textRowPointerFx;
uint16_t textBitmapOffset;

/**
 * text mode hsync routine
 * renders chars from char flash "ROM" to vram (Video RAM) according to chars from tram (Text RAM)
 */
static inline void HsyncIsrTextMode(void) {

	int16_t render_y = videoInstance.current_y + 1;
	int16_t display_y = videoInstance.current_y;

	if (render_y >= 0) {
		if (display_y >= 0) {
			if (display_y < videoInstance.hardwareResY) {
				videoPrepLineDma((uint32_t) videoInstance.vramDisplay);
			} else if (display_y < videoInstance.lowerEndOfFrame) {  //  480 image + 8 bottom border
				VIDEO_PORT->ODR = 0x00;
				TIM_ITConfig(HSYNC_TIMER, TIM_IT_CC2, DISABLE); // no more DMA
			}
		}

		if (render_y < videoInstance.hardwareResY) {

			uint8_t subLine = render_y & 0x0f;

			if (subLine == 0) {
				// begin of new text row, set text pointers
				uint32_t textRowOffset = (videoInstance.textCols * (render_y >> 4));
				textRowPointer = tram + textRowOffset;
				textRowPointerFx = tram_fx + textRowOffset;

				// set the source text bitmap offset to 0
				textBitmapOffset = 0;
			}

			// copy one pixel line of text into the backbuffer
			fixedFontDrawLine(&fixedFontInstanceVga, videoInstance.vramTarget, textRowPointer, textRowPointerFx, textBitmapOffset);

			// advance the source text bitmap offset (in the text bitmap) one line
			textBitmapOffset += fixedFontInstanceVga.stride;

			// swap buffers
			videoFlip();
		} else {
			videoInstance.vblank_flag = 1;
		}

	}

}


/**
 * HSYNC IRQ Handler
 *
 * branches to graphics or text mode ISR
 */
void TIM1_CC_IRQHandler(void) {

	if (videoInstance.textMode == 1) {
		HsyncIsrTextMode();
	} else {
		HsyncIsrPixelMode();
	}

	// reset interrupt flag
	HSYNC_TIMER->SR = (uint16_t)~TIM_IT_CC2;

	// increment line
	videoInstance.current_y++;

	for(uint8_t i=0; i < videoInstance.switchToBwAtLineSize; i++) {
		if(videoInstance.current_y == videoInstance.switchToBwAtLine[i]) {
			VIDEO_SWITCH_PORT->BSRRL = GPIO_Pin_0;
		}
	}

	for(uint8_t i=0; i < videoInstance.switchToColorAtLineSize; i++) {
		if(videoInstance.current_y == videoInstance.switchToColorAtLine[i]) {
			VIDEO_SWITCH_PORT->BSRRH = GPIO_Pin_0;
		}
	}

}

