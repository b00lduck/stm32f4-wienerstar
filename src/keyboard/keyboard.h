#pragma once
#include "config.h"
#ifdef ENABLE_KEYBOARD
	void keyboardInit(void);
	uint8_t keyboardFifoGet();
#endif


