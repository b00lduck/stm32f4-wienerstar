#pragma once

#ifdef MUSIC_ENABLED

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"

extern int16_t buffer[BUFFERSIZE] __attribute__ ((section (".sram2")));;

#define AUDIO_MAL_MODE_CIRCULAR

//#define AUDIO_MAL_DMA_IT_TC_EN
//#define AUDIO_MAL_DMA_IT_HT_EN
// #define AUDIO_MAL_DMA_IT_TE_EN

#define EVAL_AUDIO_IRQ_PREPRIO           10   /* Select the preemption priority level(0 is the highest) */
#define EVAL_AUDIO_IRQ_SUBRIO            10   /* Select the sub-priority level (0 is the highest) */

#define I2C_SPEED                        100000
#define I2S_STANDARD_PHILLIPS

#define VERIFY_WRITTENDATA 

/* Audio Reset Pin definition */
#define AUDIO_RESET_GPIO_CLK           RCC_AHB1Periph_GPIOD  
#define AUDIO_RESET_PIN                GPIO_Pin_4    
#define AUDIO_RESET_GPIO               GPIOD 
                 
/* I2S peripheral configuration defines */
#define CODEC_I2S                      SPI3
#define CODEC_I2S_CLK                  RCC_APB1Periph_SPI3
#define CODEC_I2S_ADDRESS              0x40003C0C
#define CODEC_I2S_GPIO_AF              GPIO_AF_SPI3
#define CODEC_I2S_IRQ                  SPI3_IRQn
#define CODEC_I2S_GPIO_CLOCK           (RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOA)
#define CODEC_I2S_WS_PIN               GPIO_Pin_4
#define CODEC_I2S_SCK_PIN              GPIO_Pin_10
#define CODEC_I2S_SD_PIN               GPIO_Pin_12
#define CODEC_I2S_MCK_PIN              GPIO_Pin_7
#define CODEC_I2S_WS_PINSRC            GPIO_PinSource4
#define CODEC_I2S_SCK_PINSRC           GPIO_PinSource10
#define CODEC_I2S_SD_PINSRC            GPIO_PinSource12
#define CODEC_I2S_MCK_PINSRC           GPIO_PinSource7
#define CODEC_I2S_GPIO                 GPIOC
#define CODEC_I2S_WS_GPIO              GPIOA
#define CODEC_I2S_MCK_GPIO             GPIOC
#define Audio_I2S_IRQHandler           SPI3_IRQHandler

#define AUDIO_MAL_DMA_PERIPH_DATA_SIZE DMA_PeripheralDataSize_HalfWord
#define AUDIO_MAL_DMA_MEM_DATA_SIZE    DMA_MemoryDataSize_HalfWord
#define DMA_MAX_SZE                    0xFFFF

/* I2S DMA Stream definitions */
#define AUDIO_I2S_DMA_CLOCK            RCC_AHB1Periph_DMA1
#define AUDIO_I2S_DMA_STREAM           DMA1_Stream7
#define AUDIO_I2S_DMA_DREG             CODEC_I2S_ADDRESS
#define AUDIO_I2S_DMA_CHANNEL          DMA_Channel_0
#define AUDIO_I2S_DMA_IRQ              DMA1_Stream7_IRQn
#define AUDIO_I2S_DMA_FLAG_TC          DMA_FLAG_TCIF7
#define AUDIO_I2S_DMA_FLAG_HT          DMA_FLAG_HTIF7
#define AUDIO_I2S_DMA_FLAG_FE          DMA_FLAG_FEIF7
#define AUDIO_I2S_DMA_FLAG_TE          DMA_FLAG_TEIF7
#define AUDIO_I2S_DMA_FLAG_DME         DMA_FLAG_DMEIF7
#define Audio_MAL_I2S_IRQHandler       DMA1_Stream7_IRQHandler

/* I2C peripheral configuration defines (control interface of the audio codec) */
#define CODEC_I2C                      I2C1
#define CODEC_I2C_CLK                  RCC_APB1Periph_I2C1
#define CODEC_I2C_GPIO_CLOCK           RCC_AHB1Periph_GPIOB
#define CODEC_I2C_GPIO_AF              GPIO_AF_I2C1
#define CODEC_I2C_GPIO                 GPIOB
#define CODEC_I2C_SCL_PIN              GPIO_Pin_6
#define CODEC_I2C_SDA_PIN              GPIO_Pin_9
#define CODEC_I2S_SCL_PINSRC           GPIO_PinSource6
#define CODEC_I2S_SDA_PINSRC           GPIO_PinSource9

/* Maximum Timeout values for flags and events waiting loops. These timeouts are
   not based on accurate values, they just guarantee that the application will 
   not remain stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define CODEC_FLAG_TIMEOUT             ((uint32_t)0x1000)
#define CODEC_LONG_TIMEOUT             ((uint32_t)(300 * CODEC_FLAG_TIMEOUT))

/* Codec output DEVICE */
#define OUTPUT_DEVICE_SPEAKER         1
#define OUTPUT_DEVICE_HEADPHONE       2
#define OUTPUT_DEVICE_BOTH            3
#define OUTPUT_DEVICE_AUTO            4

/* Volume Levels values */
#define DEFAULT_VOLMIN                0x00
#define DEFAULT_VOLMAX                0xFF
#define DEFAULT_VOLSTEP               0x04

#define AUDIO_PAUSE                   0
#define AUDIO_RESUME                  1

/* Codec POWER DOWN modes */
#define CODEC_PDWN_HW                 1
#define CODEC_PDWN_SW                 2

#define VOLUME_CONVERT(x)    ((Volume > 100)? 100:((uint8_t)((Volume * 255) / 100)))
#define DMA_MAX(x)           (((x) <= DMA_MAX_SZE)? (x):DMA_MAX_SZE)

uint32_t EVAL_AUDIO_Init(uint16_t OutputDevice, uint8_t Volume);
uint32_t EVAL_AUDIO_Play(uint16_t* pBuffer, uint32_t Size);
uint32_t EVAL_AUDIO_VolumeCtl(uint8_t Volume);
void Audio_MAL_Play();
void Audio_MAL_Stop();

uint16_t EVAL_AUDIO_GetSampleCallBack(void);
uint32_t Codec_TIMEOUT_UserCallback(void);

#endif
