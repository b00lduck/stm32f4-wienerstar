#include "main.h"
#include "video.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_dma.h"
#include "dma.h"

/**
 * RGB output GPIO config
 */
void RGB_GPIO_Config(void) {

	GPIO_InitTypeDef GPIO_InitStructure;

	// GPIO clock
	RCC_AHB1PeriphClockCmd(VIDEO_PORT_CLOCK, ENABLE);

	// GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(VIDEO_PORT, &GPIO_InitStructure);

	// blank out
	VIDEO_PORT->ODR = 0x00;

}

/**
 * External DA clock GPIO config
 */
void DACLOCK_GPIO_Config(void) {

	GPIO_InitTypeDef GPIO_InitStructure;

	// GPIO clock enable
	RCC_AHB1PeriphClockCmd(DACLOCK_PORT_CLOCK, ENABLE);

	// GPIO configuration
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(DACLOCK_PORT, &GPIO_InitStructure);

	// Connect TIM8 pin to alternate function
	GPIO_PinAFConfig(DACLOCK_PORT, DACLOCK_PINSOURCE, DACLOCK_PIN_AF);
}

/**
 * configure timer and DMA for pixel output
 */
void Pixel_DMA_Config(uint16_t pixelDmaLength, uint8_t pixelDmaTimerPeriod) {

	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	// Enable Timer Clock
	RCC_APB2PeriphClockCmd(PIXELDMA_TIMER_CLOCK, ENABLE);

	// Enable DMA Clock
	RCC_AHB1PeriphClockCmd(PIXELDMA_STREAM_CLOCK, ENABLE);

	// Time base configuration
	TIM_TimeBaseStructure.TIM_Prescaler = TIM_ICPSC_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = pixelDmaTimerPeriod;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(PIXELDMA_TIMER, &TIM_TimeBaseStructure);

	// Be a slave of TIM1
	TIM_SelectInputTrigger(PIXELDMA_TIMER, TIM_TS_ITR0);
    TIM_SelectSlaveMode(PIXELDMA_TIMER, TIM_SlaveMode_Trigger);
    TIM_SelectMasterSlaveMode(PIXELDMA_TIMER, TIM_MasterSlaveMode_Enable);


    // Clock output for external D/A converter
	// PWM1 Mode configuration: Channel2
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 15;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OC2Init(PIXELDMA_TIMER, &TIM_OCInitStructure);

	// use timer update trigger for pixel dma
	TIM_DMACmd(PIXELDMA_TIMER, TIM_DMA_Update, ENABLE);

	// Configure DMA Stream
	DMA_InitStructure.DMA_Channel = PIXELDMA_CHANNEL;
	DMA_InitStructure.DMA_PeripheralBaseAddr = ((uint32_t)&VIDEO_PORT->ODR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)videoInstance.vramDisplay;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = pixelDmaLength;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(PIXELDMA_STREAM, &DMA_InitStructure);

    DMA_Cmd(PIXELDMA_STREAM, DISABLE);

	// Enable DMA Stream Transfer Complete interrupt
	DMA_ITConfig(PIXELDMA_STREAM, DMA_IT_TCIF1, ENABLE);

	// Enable the global Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = PIXELDMA_STREAM_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 15;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

/**
 * end of transfer interrupt for DMA1
 * this is the end of the line
 */
void DMA2_Stream1_IRQHandler(void) {

	// Clear interrupt flag.
	DMA2->LIFCR |= DMA_LIFCR_CTCIF1;

	// blank out
	VIDEO_PORT->ODR = 0x00;

	// disable the timer
	PIXELDMA_TIMER->CR1 &= ~TIM_CR1_CEN;

}
