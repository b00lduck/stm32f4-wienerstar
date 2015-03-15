#define HSYNC_PORT			GPIOA
#define HSYNC_PORT_CLOCK    RCC_AHB1Periph_GPIOA
#define HSYNC_PIN			GPIO_Pin8
#define HSYNC_PINSOURCE		GPIO_PinSource8
#define HSYNC_PIN_AF		GPIO_AF_TIM1
#define HSYNC_TIMER			TIM1
#define HSYNC_TIMER_CLOCK	RCC_APB2Periph_TIM1
#define HSYNC_TIMER_CC_IRQ  TIM1_CC_IRQn

#define HSYNC_TIMER_CC2		50 // start of line interrupt

void HSYNC_GPIO_Config(void);
void HSYNC_TIMER_Config(uint16_t pixelDmaStartOffset, uint16_t hsyncTimerPeriod, uint16_t hsyncSyncLength, enum e_videoSyncPolarity syncPolarity);
