
#define PIXELDMA					DMA2

#define PIXELDMA_CHANNEL        	DMA_Channel_7
#define PIXELDMA_STREAM         	DMA2_Stream1
#define PIXELDMA_STREAM_CLOCK 		RCC_AHB1Periph_DMA2

#define PIXELDMA_STREAM_IRQ      	DMA2_Stream1_IRQn

#define PIXELDMA_TIMER				TIM8
#define PIXELDMA_TIMER_CLOCK		RCC_APB2Periph_TIM8

void RGB_GPIO_Config(void);
void DACLOCK_GPIO_Config(void);
void Pixel_DMA_Config(uint16_t pixelDmaLength, uint8_t pixelDmaTimerPeriod);

