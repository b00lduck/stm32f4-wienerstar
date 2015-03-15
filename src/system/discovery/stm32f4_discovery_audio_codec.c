#include "config.h"

#ifdef MUSIC_ENABLED
#include "stm32f4_discovery_audio_codec.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_rcc.h"

int16_t buffer[BUFFERSIZE] __attribute__ ((section (".sram2")));;

/* Mask for the bit EN of the I2S CFGR register */
#define I2S_ENABLE_MASK               0x0400

#define CODEC_RESET_DELAY             0x4FFF
#define CODEC_ADDRESS                 0x94
#define CODEC_STANDARD                0x04

DMA_InitTypeDef DMA_InitStructure; 
DMA_InitTypeDef AUDIO_MAL_DMA_InitStructure;

__IO uint32_t CODECTimeout = CODEC_LONG_TIMEOUT;
__IO uint8_t OutputDev = 0;

static void Audio_MAL_IRQHandler(void);

/* High Layer codec functions */
static uint32_t Codec_Init(uint16_t OutputDevice, uint8_t Volume);
static uint32_t Codec_VolumeCtrl(uint8_t Volume);

/* Low layer codec functions */
static void     Codec_CtrlInterface_Init(void);
static void     Codec_AudioInterface_Init();
static void     Codec_Reset(void);
static uint32_t Codec_WriteRegister(uint8_t RegisterAddr, uint8_t RegisterValue);
static uint32_t Codec_ReadRegister(uint8_t RegisterAddr);
static void     Codec_GPIO_Init(void);
static void     Delay(__IO uint32_t nCount);

/* Peripherals configuration functions */
static void     Audio_MAL_Init(void);

uint32_t EVAL_AUDIO_Init(uint16_t OutputDevice, uint8_t Volume) {
  if (Codec_Init(OutputDevice, VOLUME_CONVERT(Volume)) != 0) {
    return 1;                
  } else {
    Audio_MAL_Init();
    return 0;
  }
}

/**
  * @brief  This function handles main Media layer interrupt. 
  * @param  None
  * @retval 0 if correct communication, else wrong communication
  */
static void Audio_MAL_IRQHandler(void) {

#ifdef AUDIO_MAL_DMA_IT_TC_EN
  /* Transfer complete interrupt */
  if (DMA_GetFlagStatus(AUDIO_I2S_DMA_STREAM, AUDIO_I2S_DMA_FLAG_TC) != RESET) {
    EVAL_AUDIO_TransferComplete_CallBack();
    DMA_ClearFlag(AUDIO_I2S_DMA_STREAM, AUDIO_I2S_DMA_FLAG_TC);
  }
#endif /* AUDIO_MAL_DMA_IT_TC_EN */

#ifdef AUDIO_MAL_DMA_IT_HT_EN  
  /* Half Transfer complete interrupt */
  if (DMA_GetFlagStatus(AUDIO_I2S_DMA_STREAM, AUDIO_I2S_DMA_FLAG_HT) != RESET) {
    EVAL_AUDIO_HalfTransfer_CallBack();
    DMA_ClearFlag(AUDIO_I2S_DMA_STREAM, AUDIO_I2S_DMA_FLAG_HT);
  }
#endif /* AUDIO_MAL_DMA_IT_HT_EN */
  
#ifdef AUDIO_MAL_DMA_IT_TE_EN  
  /* FIFO Error interrupt */

  if (DMA_GetFlagStatus(AUDIO_I2S_DMA_STREAM, AUDIO_I2S_DMA_FLAG_TE) != RESET) {
	EVAL_AUDIO_Error_CallBack((uint32_t*)&pAddr);
    DMA_ClearFlag(AUDIO_I2S_DMA_STREAM, AUDIO_I2S_DMA_FLAG_TE);
  }

  if (DMA_GetFlagStatus(AUDIO_I2S_DMA_STREAM, AUDIO_I2S_DMA_FLAG_FE) != RESET) {
	EVAL_AUDIO_Error_CallBack((uint32_t*)&pAddr);
    DMA_ClearFlag(AUDIO_I2S_DMA_STREAM, AUDIO_I2S_DMA_FLAG_FE);
  }

  if (DMA_GetFlagStatus(AUDIO_I2S_DMA_STREAM, AUDIO_I2S_DMA_FLAG_DME) != RESET) {
	EVAL_AUDIO_Error_CallBack((uint32_t*)&pAddr);
    DMA_ClearFlag(AUDIO_I2S_DMA_STREAM, AUDIO_I2S_DMA_FLAG_DME);
  }  

#endif /* AUDIO_MAL_DMA_IT_TE_EN */
}

/**
  * @brief  This function handles main I2S interrupt.
  * @param  None
  * @retval 0 if correct communication, else wrong communication
  */
void Audio_MAL_I2S_IRQHandler(void) {
  Audio_MAL_IRQHandler();
}

/*
  * @brief  I2S interrupt management
  * @param  None
  * @retval None
  *
void Audio_I2S_IRQHandler(void) {

  if (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) != RESET) {
    SPI_I2S_SendData(CODEC_I2S, EVAL_AUDIO_GetSampleCallBack()); 
  }
}
*/
/*========================

                CS43L22 Audio Codec Control Functions
                                                ==============================*/
/**
  * @brief  Initializes the audio codec and all related interfaces (control 
  *         interface: I2C and audio interface: I2S)
  * @param  OutputDevice: can be OUTPUT_DEVICE_SPEAKER, OUTPUT_DEVICE_HEADPHONE,
  *                       OUTPUT_DEVICE_BOTH or OUTPUT_DEVICE_AUTO .
  * @param  Volume: Initial volume level (from 0 (Mute) to 100 (Max))
  * @param  AudioFreq: Audio frequency used to play the audio stream.
  * @retval 0 if correct communication, else wrong communication
  */
static uint32_t Codec_Init(uint16_t OutputDevice, uint8_t Volume) {
  uint32_t counter = 0; 

  /* Configure the Codec related IOs */
  Codec_GPIO_Init();   
  
  /* Reset the Codec Registers */
  Codec_Reset();

  /* Initialize the Control interface of the Audio Codec */
  Codec_CtrlInterface_Init();     
  
  /* Keep Codec powered OFF */
  counter += Codec_WriteRegister(0x02, 0x01);  
      
  counter += Codec_WriteRegister(0x04, 0xAF); /* SPK always OFF & HP always ON */
  OutputDev = 0xAF;
  
  /* Clock configuration: Auto detection */  
  counter += Codec_WriteRegister(0x05, 0x81);
  
  /* Set the Slave Mode and the audio Standard */  
  counter += Codec_WriteRegister(0x06, CODEC_STANDARD);
      
  /* Set the Master volume */
  Codec_VolumeCtrl(Volume);
  
  /* Power on the Codec */
  counter += Codec_WriteRegister(0x02, 0x9E);  
  
  /* Additional configuration for the CODEC. These configurations are done to reduce
      the time needed for the Codec to power off. If these configurations are removed, 
      then a long delay should be added between powering off the Codec and switching 
      off the I2S peripheral MCLK clock (which is the operating clock for Codec).
      If this delay is not inserted, then the codec will not shut down properly and
      it results in high noise after shut down. */
  
  /* Disable the analog soft ramp */
  counter += Codec_WriteRegister(0x0A, 0x00);
  /* Disable the digital soft ramp */
  counter += Codec_WriteRegister(0x0E, 0x04);
  /* Disable the limiter attack level */
  counter += Codec_WriteRegister(0x27, 0x00);
  /* Adjust Bass and Treble levels */
  counter += Codec_WriteRegister(0x1F, 0x0F);
  /* Adjust PCM volume level */
  counter += Codec_WriteRegister(0x1A, 0x0A);
  counter += Codec_WriteRegister(0x1B, 0x0A);

  /* Configure the I2S peripheral */
  Codec_AudioInterface_Init();
  
  /* Return communication control value */
  return counter;  
}

/**
  * @brief  Sets higher or lower the codec volume level.
  * @param  Volume: a byte value from 0 to 255 (refer to codec registers
  *         description for more details).
  * @retval 0 if correct communication, else wrong communication
  */
static uint32_t Codec_VolumeCtrl(uint8_t Volume)
{
  uint32_t counter = 0;

  if (Volume > 0xE6)
  {
    /* Set the Master volume */
    counter += Codec_WriteRegister(0x20, Volume - 0xE7);
    counter += Codec_WriteRegister(0x21, Volume - 0xE7);
  }
  else
  {
    /* Set the Master volume */
    counter += Codec_WriteRegister(0x20, Volume + 0x19);
    counter += Codec_WriteRegister(0x21, Volume + 0x19);
  }

  return counter;
}

/**
  * @brief  Resets the audio codec. It restores the default configuration of the 
  *         codec (this function shall be called before initializing the codec).
  * @note   This function calls an external driver function: The IO Expander driver.
  * @param  None
  * @retval None
  */
static void Codec_Reset(void)
{
  /* Power Down the codec */
  GPIO_WriteBit(AUDIO_RESET_GPIO, AUDIO_RESET_PIN, Bit_RESET);

  /* wait for a delay to insure registers erasing */
  Delay(CODEC_RESET_DELAY); 
  
  /* Power on the codec */
  GPIO_WriteBit(AUDIO_RESET_GPIO, AUDIO_RESET_PIN, Bit_SET);
}

/**
  * @brief  Writes a Byte to a given register into the audio codec through the 
            control interface (I2C)
  * @param  RegisterAddr: The address (location) of the register to be written.
  * @param  RegisterValue: the Byte value to be written into destination register.
  * @retval 0 if correct communication, else wrong communication
  */
static uint32_t Codec_WriteRegister(uint8_t RegisterAddr, uint8_t RegisterValue)
{
  uint32_t result = 0;

  /*!< While the bus is busy */
  CODECTimeout = CODEC_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_BUSY))
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }
  
  /* Start the config sequence */
  I2C_GenerateSTART(CODEC_I2C, ENABLE);

  /* Test on EV5 and clear it */
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }
  
  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress(CODEC_I2C, CODEC_ADDRESS, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }

  /* Transmit the first address for write operation */
  I2C_SendData(CODEC_I2C, RegisterAddr);

  /* Test on EV8 and clear it */
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }
  
  /* Prepare the register value to be sent */
  I2C_SendData(CODEC_I2C, RegisterValue);
  
  /*!< Wait till all data have been physically transferred on the bus */
  CODECTimeout = CODEC_LONG_TIMEOUT;
  while(!I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_BTF))
  {
    if((CODECTimeout--) == 0) {
    	Codec_TIMEOUT_UserCallback();
    }
  }
  
  /* End the configuration sequence */
  I2C_GenerateSTOP(CODEC_I2C, ENABLE);  
  
#ifdef VERIFY_WRITTENDATA
  /* Verify that the data has been correctly written */  
  result = (Codec_ReadRegister(RegisterAddr) == RegisterValue)? 0:1;
#endif /* VERIFY_WRITTENDATA */

  /* Return the verifying value: 0 (Passed) or 1 (Failed) */
  return result;  
}

/**
  * @brief  Reads and returns the value of an audio codec register through the
  *         control interface (I2C).
  * @param  RegisterAddr: Address of the register to be read.
  * @retval Value of the register to be read or dummy value if the communication
  *         fails.
  */
static uint32_t Codec_ReadRegister(uint8_t RegisterAddr)
{
  uint32_t result = 0;

  /*!< While the bus is busy */
  CODECTimeout = CODEC_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_BUSY))
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }
  
  /* Start the config sequence */
  I2C_GenerateSTART(CODEC_I2C, ENABLE);

  /* Test on EV5 and clear it */
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }
  
  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress(CODEC_I2C, CODEC_ADDRESS, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }

  /* Transmit the register address to be read */
  I2C_SendData(CODEC_I2C, RegisterAddr);

  /* Test on EV8 and clear it */
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while (I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_BTF) == RESET)
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }
  
  /*!< Send START condition a second time */  
  I2C_GenerateSTART(CODEC_I2C, ENABLE);
  
  /*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  } 
  
  /*!< Send Codec address for read */
  I2C_Send7bitAddress(CODEC_I2C, CODEC_ADDRESS, I2C_Direction_Receiver);  
  
  /* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while(I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_ADDR) == RESET)
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }     
  
  /*!< Disable Acknowledgment */
  I2C_AcknowledgeConfig(CODEC_I2C, DISABLE);   
  
  /* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
  (void)CODEC_I2C->SR2;
  
  /*!< Send STOP Condition */
  I2C_GenerateSTOP(CODEC_I2C, ENABLE);
  
  /* Wait for the byte to be received */
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while(I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_RXNE) == RESET)
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }
  
  /*!< Read the byte received from the Codec */
  result = I2C_ReceiveData(CODEC_I2C);
  
  /* Wait to make sure that STOP flag has been cleared */
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while(CODEC_I2C->CR1 & I2C_CR1_STOP)
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }  
  
  /*!< Re-Enable Acknowledgment to be ready for another reception */
  I2C_AcknowledgeConfig(CODEC_I2C, ENABLE);  
  
  /* Clear AF flag for next communication */
  I2C_ClearFlag(CODEC_I2C, I2C_FLAG_AF); 
  
  /* Return the byte read from Codec */
  return result;
}

/**
  * @brief  Initializes the Audio Codec control interface (I2C).
  * @param  None
  * @retval None
  */
static void Codec_CtrlInterface_Init(void)
{
  I2C_InitTypeDef I2C_InitStructure;
  
  /* Enable the CODEC_I2C peripheral clock */
  RCC_APB1PeriphClockCmd(CODEC_I2C_CLK, ENABLE);
  
  /* CODEC_I2C peripheral configuration */
  I2C_DeInit(CODEC_I2C);
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x33;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
  /* Enable the I2C peripheral */
  I2C_Cmd(CODEC_I2C, ENABLE);  
  I2C_Init(CODEC_I2C, &I2C_InitStructure);
}

/**
  * @brief  Initializes the Audio Codec audio interface (I2S)
  * @note   This function assumes that the I2S input clock (through PLL_R in 
  *         Devices RevA/Z and through dedicated PLLI2S_R in Devices RevB/Y)
  *         is already configured and ready to be used.    
  * @param  None
  * @retval None
  */
static void Codec_AudioInterface_Init() {

  /* Enable the CODEC_I2S peripheral clock */
  RCC_APB1PeriphClockCmd(CODEC_I2S_CLK, ENABLE);
  
  SPI_I2S_DeInit(CODEC_I2S);

  // Clear I2SMOD, I2SE, I2SCFG, PCMSYNC, I2SSTD, CKPOL, DATLEN and CHLEN bits
  CODEC_I2S->I2SCFGR &= (uint16_t)0xF040;
  CODEC_I2S->I2SPR = 0x0002;

  // Write to SPIx I2SPR register
  CODEC_I2S->I2SPR = (uint16_t)I2S_DIV | (uint16_t)(I2S_ODD<<8) | I2S_MCLKOutput_Enable;

  // Configure the I2S
  CODEC_I2S->I2SCFGR |= SPI_I2SCFGR_I2SMOD | I2S_Mode_MasterTx;
}

/**
  * @brief Initializes IOs used by the Audio Codec (on the control and audio 
  *        interfaces).
  * @param  None
  * @retval None
  */
static void Codec_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  
  // Audio reset pin configuration
  RCC_AHB1PeriphClockCmd(AUDIO_RESET_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin = AUDIO_RESET_PIN; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(AUDIO_RESET_GPIO, &GPIO_InitStructure);    
  
  // CODEC_I2C SCL and SDA pins configuration
  RCC_AHB1PeriphClockCmd(CODEC_I2C_GPIO_CLOCK | CODEC_I2S_GPIO_CLOCK, ENABLE);
  GPIO_InitStructure.GPIO_Pin = CODEC_I2C_SCL_PIN | CODEC_I2C_SDA_PIN; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;

  GPIO_Init(CODEC_I2C_GPIO, &GPIO_InitStructure);     
  GPIO_PinAFConfig(CODEC_I2C_GPIO, CODEC_I2S_SCL_PINSRC, CODEC_I2C_GPIO_AF);  
  GPIO_PinAFConfig(CODEC_I2C_GPIO, CODEC_I2S_SDA_PINSRC, CODEC_I2C_GPIO_AF);  

  // CODEC_I2S pins configuration: SCK and SD pins
  GPIO_InitStructure.GPIO_Pin = CODEC_I2S_SCK_PIN | CODEC_I2S_SD_PIN; 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(CODEC_I2S_GPIO, &GPIO_InitStructure);
  GPIO_PinAFConfig(CODEC_I2S_WS_GPIO, CODEC_I2S_WS_PINSRC, CODEC_I2S_GPIO_AF);  
  GPIO_PinAFConfig(CODEC_I2S_GPIO, CODEC_I2S_SCK_PINSRC, CODEC_I2S_GPIO_AF);

  // CODEC_I2S pins configuration: WS pin
  GPIO_InitStructure.GPIO_Pin = CODEC_I2S_WS_PIN;
  GPIO_Init(CODEC_I2S_WS_GPIO, &GPIO_InitStructure);
  GPIO_PinAFConfig(CODEC_I2S_GPIO, CODEC_I2S_SD_PINSRC, CODEC_I2S_GPIO_AF);

  // CODEC_I2S pins configuration: MCK pin (MASTER CLOCK)
  GPIO_InitStructure.GPIO_Pin = CODEC_I2S_MCK_PIN; 
  GPIO_Init(CODEC_I2S_MCK_GPIO, &GPIO_InitStructure);   
  GPIO_PinAFConfig(CODEC_I2S_MCK_GPIO, CODEC_I2S_MCK_PINSRC, CODEC_I2S_GPIO_AF); 
}

static void Delay( __IO uint32_t nCount) {
  for (; nCount != 0; nCount--);
}

static void Audio_MAL_Init(void) {
  
#ifdef I2S_INTERRUPT  
  NVIC_InitTypeDef   NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = SPI3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_TXE, ENABLE);

  I2S_Cmd(SPI3, ENABLE); 
#else  
	#if defined(AUDIO_MAL_DMA_IT_TC_EN) || defined(AUDIO_MAL_DMA_IT_HT_EN) || defined(AUDIO_MAL_DMA_IT_TE_EN)
	  NVIC_InitTypeDef NVIC_InitStructure;
	#endif

    /* Enable the DMA clock */
    RCC_AHB1PeriphClockCmd(AUDIO_I2S_DMA_CLOCK, ENABLE);
    
    /* Configure the DMA Stream */
    DMA_Cmd(AUDIO_I2S_DMA_STREAM, DISABLE);
    DMA_DeInit(AUDIO_I2S_DMA_STREAM);

    /* Set the parameters to be configured */
    DMA_InitStructure.DMA_Channel = AUDIO_I2S_DMA_CHANNEL;
    DMA_InitStructure.DMA_PeripheralBaseAddr = AUDIO_I2S_DMA_DREG;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)0;      /* This field will be configured in play function */
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize = (uint32_t)0xFFFE;      /* This field will be configured in play function */
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = AUDIO_MAL_DMA_PERIPH_DATA_SIZE;
    DMA_InitStructure.DMA_MemoryDataSize = AUDIO_MAL_DMA_MEM_DATA_SIZE; 
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = 0;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
    DMA_Init(AUDIO_I2S_DMA_STREAM, &DMA_InitStructure);
    
    // Enable the selected DMA interrupts (selected in "stm32f4_discovery_eval_audio_codec.h" defines)
#ifdef AUDIO_MAL_DMA_IT_TC_EN
    DMA_ITConfig(AUDIO_I2S_DMA_STREAM, DMA_IT_TC, ENABLE);
#endif /* AUDIO_MAL_DMA_IT_TC_EN */
#ifdef AUDIO_MAL_DMA_IT_HT_EN
    DMA_ITConfig(AUDIO_I2S_DMA_STREAM, DMA_IT_HT, ENABLE);
#endif /* AUDIO_MAL_DMA_IT_HT_EN */
#ifdef AUDIO_MAL_DMA_IT_TE_EN
    DMA_ITConfig(AUDIO_I2S_DMA_STREAM, DMA_IT_TE | DMA_IT_FE | DMA_IT_DME, ENABLE);
#endif /* AUDIO_MAL_DMA_IT_TE_EN */
    
#if defined(AUDIO_MAL_DMA_IT_TC_EN) || defined(AUDIO_MAL_DMA_IT_HT_EN) || defined(AUDIO_MAL_DMA_IT_TE_EN)
    /* I2S DMA IRQ Channel configuration */
    NVIC_InitStructure.NVIC_IRQChannel = AUDIO_I2S_DMA_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EVAL_AUDIO_IRQ_PREPRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = EVAL_AUDIO_IRQ_SUBRIO;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif     

  /* Enable the I2S DMA request */
  SPI_I2S_DMACmd(CODEC_I2S, SPI_I2S_DMAReq_Tx, ENABLE);
    
}
#endif

/**
 * Audio_MAL_Play
 *
 * Starts playing audio stream
 */
void Audio_MAL_Play() {

  // Configure the buffer address and size
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)buffer;
  DMA_InitStructure.DMA_BufferSize = (uint32_t)BUFFERSIZE;

  // Configure the DMA Stream with the new parameters
  DMA_Init(AUDIO_I2S_DMA_STREAM, &DMA_InitStructure);

  // Enable the I2S DMA Stream
  DMA_Cmd(AUDIO_I2S_DMA_STREAM, ENABLE);
  
  // If the I2S peripheral is still not enabled, enable it
  if ((CODEC_I2S->I2SCFGR & I2S_ENABLE_MASK) == 0)  {
    I2S_Cmd(CODEC_I2S, ENABLE);
  }
}

void Audio_MAL_Stop() {
  DMA_Cmd(AUDIO_I2S_DMA_STREAM, DISABLE);
}

uint32_t Codec_TIMEOUT_UserCallback(void) {
	while(1);
}
#endif
