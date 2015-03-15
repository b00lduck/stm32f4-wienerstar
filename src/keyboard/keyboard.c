#include "config.h"

#ifdef ENABLE_KEYBOARD

#include "main.h"
#include "keyboard.h"

#define KEYBOARD_FIFOLEN 64

uint8_t keyboardPos = 0;
uint8_t keyboardData[64];
uint8_t keyboardScanned = 0;

uint8_t keyboardFifoBuffer[KEYBOARD_FIFOLEN];
uint8_t keyboardFifoBufferWrite = 0;
uint8_t keyboardFifoBufferRead = 0;
uint8_t keyboardFifoBufferSpace = KEYBOARD_FIFOLEN;

/**
 *
 */
static void keyboardGPIOInit(void) {

	GPIO_InitTypeDef GPIO_InitStructure;

	// Enable GPIOA clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	// Configure PA0,PA1 pin as input pullup
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

/**
 *
 */
static void keyboardExtintInit(void) {

	  NVIC_InitTypeDef NVIC_InitStructure;
	  EXTI_InitTypeDef EXTI_InitStructure;

	  // Enable SYSCFG clock
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	  // Connect EXTI Line2 to PA2 pin
	  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource2);

	  // Configure EXTI Line2
	  EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	  EXTI_Init(&EXTI_InitStructure);

	  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 15;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);

}

/**
 *
 */
static inline void keyboardReset() {
	memset(keyboardData,0,8);
	keyboardPos = 0;
}

/**
 *
 */
void keyboardInit(void) {
	keyboardGPIOInit();
	keyboardExtintInit();
	keyboardReset();
}

/**
 *
 */
static inline void keyboardFifoPut(uint8_t scancode) {

	// no space left
	if (keyboardFifoBufferSpace == 0) {
		return;
	}

	keyboardFifoBuffer[keyboardFifoBufferWrite++] = scancode;
	keyboardFifoBufferSpace--;

	if (keyboardFifoBufferWrite == KEYBOARD_FIFOLEN) {
		keyboardFifoBufferWrite = 0;
	}
}

/**
 *
 */
uint8_t keyboardFifoGet() {

	// nothing here
	if (keyboardFifoBufferSpace == KEYBOARD_FIFOLEN) {
		return 0;
	}

	uint8_t ret = keyboardFifoBuffer[keyboardFifoBufferRead++];
	keyboardFifoBufferSpace++;

	if (keyboardFifoBufferRead == KEYBOARD_FIFOLEN) {
		keyboardFifoBufferRead = 0;
	}

	return ret;
}

/**
 *
 */
static inline void keyboardProcessScancode(uint8_t scancode) {
	keyboardFifoPut(scancode);
}

/**
 *
 */
void EXTI2_IRQHandler(void) {

	// TODO: implement timeout

	EXTI_ClearITPendingBit(EXTI_Line2);

	uint8_t error = 0;

	uint8_t val = (GPIOA->IDR & 2) >> 1;

	if (keyboardPos == 0) {
		// start bit
		error = (val != 0);
	} else if (keyboardPos == 9) {
		// parity bit
		// TODO: implement parity check
	} else if (keyboardPos == 10) {
		// stop bit
		error = (val != 1);
	} else {
		// data bit
		keyboardData[keyboardPos-1] = val;
	}

	if ((error) || (keyboardPos > 10)) {
		keyboardReset();
		return;
	}

	if (keyboardPos == 10) {

		uint8_t i;
		for(i=0;i<8;i++) {
			keyboardScanned += keyboardData[i] << i;
		}

		keyboardProcessScancode(keyboardScanned);

		keyboardReset();
		keyboardScanned = 0;

	} else {
		keyboardPos++;
	}
}
#endif
