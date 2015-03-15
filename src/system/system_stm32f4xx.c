#include "config.h"
#include "stm32f4xx.h"

uint32_t SystemCoreClock = 168000000;

static void SetSysClock(void);

void SystemInit(void) {

  // FPU
  SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));

  /*
   * Enable additional exceptions, so they don't escalate to
   * HardFault immediately.
   *
  SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk |
      SCB_SHCSR_BUSFAULTENA_Msk |
      SCB_SHCSR_MEMFAULTENA_Msk;
      */

  RCC->CR |= (uint32_t)0x00000001;
  RCC->CFGR = 0x00000000;

  // Reset HSEON, CSSON and PLLON bits
  RCC->CR &= (uint32_t)0xFEF6FFFF;

  // Reset PLLCFGR register
  RCC->PLLCFGR = 0x24003010;

  // Reset HSEBYP bit
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  // Disable all interrupts
  RCC->CIR = 0x00000000;
         
  // Configure the System clock source, PLL Multiplier and Divider factors,
  // AHB/APBx prescalers and Flash settings
  SetSysClock();

  // Configure the Vector Table location add offset address
  SCB->VTOR = FLASH_BASE; // Vector Table Relocation in Internal FLASH
}

static void SetSysClock(void) {

  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  
  // Enable HSE
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
  // Wait till HSE is ready and if Time out is reached exit
  do {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET) {
    HSEStatus = (uint32_t)0x01;
  } else {
    HSEStatus = (uint32_t)0x00;
  }

  if (HSEStatus == (uint32_t)0x01) {
    // Select regulator voltage output Scale 1 mode, System frequency up to 168 MHz
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;

    // HCLK = SYSCLK / 1
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
      
    // PCLK2 = HCLK / 2
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
    
    // PCLK1 = HCLK / 4
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;

    // PLLI2S clock used as I2S clock source
    RCC->CFGR &= ~RCC_CFGR_I2SSRC;

    // Configure the main PLL
    RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24);

    // Enable the main PLL
    RCC->CR |= RCC_CR_PLLON;

    // Wait till the main PLL is ready
    while((RCC->CR & RCC_CR_PLLRDY) == 0) { }
   
    // Configure Flash prefetch, Instruction cache, Data cache and wait state
    FLASH->ACR = FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;

    // Select the main PLL as system clock source
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    // Wait till the main PLL is used as system clock source
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL); { }
  } else {
	  // If HSE fails to start-up, the application will have wrong clock
	  // configuration. User can add here some code to deal with this error
  }

  #ifdef MUSIC_ENABLED

	  // I2S clock configuration
	  // Configure PLLI2S
	  RCC->PLLI2SCFGR = (PLLI2S_N << 6) | (PLLI2S_R << 28);

	  // Enable PLLI2S
	  RCC->CR |= ((uint32_t)RCC_CR_PLLI2SON);

	  // Wait till PLLI2S is ready
	  while((RCC->CR & RCC_CR_PLLI2SRDY) == 0) { }
  #endif

}
