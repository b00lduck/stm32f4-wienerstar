#pragma once

#include "config.h"

#ifdef ENABLE_KEYBOARD
	struct t_keystroke {
		uint8_t code;
		uint8_t esc;
		uint8_t brk;
	};

	void keyboardParserClearKeystroke();
	void keyboardParserInit();

	void keyboardParserParseScancode(uint8_t scancode);
	void keyboardParserVisualizeKeystroke();
	void keyboardParserProcessKeystroke(uint16_t *keycode, uint8_t* breaked);
	uint8_t keyboardParserPollKeyboard();
#endif
