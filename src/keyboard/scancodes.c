#include "config.h"

#ifdef ENABLE_KEYBOARD
#include "main.h"
#include "scancodes.h"

const uint16_t scancodes[0x84] = {

		0, KEY_F9, 0, KEY_F5, KEY_F3, KEY_F1, KEY_F2, KEY_F12,  	// 00..07
		0, KEY_F10, KEY_F8, KEY_F6, KEY_F4, KEY_TAB, '^', 0,		// 08..0f
		0, KEY_ALTL, KEY_SHIFTL,   0,   KEY_CTRLL, 'q', '1',   0,   // 10..17
		0,   0, 'y', 's', 'a', 'w', '2', 0,							// 18..1f
		0, 'c', 'x', 'd', 'e', '4', '3', 0, 		  				// 20..27
		0, ' ', 'v', 'f', 't', 'r', '5', 0,   						// 28..2f
		0, 'n', 'b', 'h', 'g', 'z', '6', 0, 						// 30..37
		0,   0, 'm', 'j', 'u', '7', '8', 0,   						// 38..3f
		0, ',', 'k', 'i', 'o', '0', '9', 0,
		0, '.', '-', 'l', CHAR_OUML, 'p', CHAR_SZLIG, 0,   						// 48..4f
		0, 0, CHAR_AUML, 0, CHAR_UUML, 0, 0, 0,
		KEY_CAPSLOCK, KEY_SHIFTR,  KEY_RETURN, '+', 0, '#', 0, 0,   // 58..5f
		0,   '<',   0,   0,   0,   0, KEY_BACKSPACE, 0,
		0, 0,  0,   0, 0, 0, 0, 0,   								// 68..6f
		0,   0,   0,   0,   0,   0,   0, 0,
		KEY_F11, 0, 0, 0, 0, 0, 0, 0,   							// 78..7f
		0, 0, 0, KEY_F7 									   		// 80..83
};

const uint16_t scancodes_e0[0x84] = {

		0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,
		0,KEY_ALTR,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,

		0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,    0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,    0,0,0,KEY_LEFT,KEY_HOME,0,0,0,
		0,0,KEY_DOWN,0,KEY_RIGHT,KEY_UP,0,0,    0,0,0,0,0,0,0,0,

		0,0,0,0
};


const uint16_t scancodes_shift[0x84] = {

		0, KEY_SHIFT_F9, 0, KEY_SHIFT_F5, KEY_SHIFT_F3, KEY_SHIFT_F1, KEY_SHIFT_F2, KEY_SHIFT_F12,  	// 00..07
		0, KEY_SHIFT_F10, KEY_SHIFT_F8, KEY_SHIFT_F6, KEY_SHIFT_F4, KEY_SHIFT_TAB, '^', 0,		// 08..0f
		0, KEY_ALTL, KEY_SHIFTL,   0,   KEY_CTRLL, 'Q', '!',   0,   // 10..17
		0,   0, 'Y', 'S', 'A', 'W', '"', 0,							// 18..1f
		0, 'C', 'X', 'D', 'E', '$', '§', 0,   						// 20..27
		0, ' ', 'V', 'F', 'T', 'R', '%', 0,   						// 28..2f
		0, 'N', 'B', 'H', 'G', 'Z', '&', 0, 						// 30..37
		0,   0, 'M', 'J', 'U', '/', '(', 0,   						// 38..3f
		0, ';', 'K', 'I', 'O', '=', ')', 0,
		0, ':', '_', 'L', CHAR_OUMLC, 'P', '?', 0,   						// 48..4f
		0,   0, CHAR_AUMLC,   0, CHAR_UUMLC, '`',   0, 0,
		KEY_CAPSLOCK, KEY_SHIFTR, KEY_SHIFT_RETURN, '*', 0, '\'', 0, 0,   // 58..5f
		0, '>', 0, 0, 0, 0, KEY_SHIFT_BACKSPACE, 0,
		0, 0, 0, 0, 0, 0, 0, 0,   									// 68..6f
		0, 0, 0, 0, 0, 0, 0, 0,
		KEY_SHIFT_F11, 0, 0, 0, 0, 0, 0, 0,   						// 78..7f
		0, 0, 0, KEY_SHIFT_F7 									   	// 80..83
};

const uint16_t scancodes_altl[0x84] = {

		0, KEY_ALT_F9, 0, KEY_ALT_F5, KEY_ALT_F3, KEY_ALT_F1, KEY_ALT_F2, KEY_ALT_F12,  	// 00..07
		0, KEY_ALT_F10, KEY_ALT_F8, KEY_ALT_F6, KEY_ALT_F4, KEY_ALT_TAB,   0, 0,		// 08..0f
		0, KEY_ALTL, KEY_SHIFTL,   0,   KEY_CTRLL,   0,   0,   0,   // 10..17
		0,   0,   0,   0,   0,   0,   0, 0,							// 18..1f
		0,   0,   0,   0,   0,   0,   0, 0,   						// 20..27
		0,   0,   0,   0,   0,   0,   0, 0,   						// 28..2f
		0,   0,   0,   0,   0,   0,   0, 0, 						// 30..37
		0,   0,   0,   0,   0,   0,   0, 0,   						// 38..3f
		0,   0,   0,   0,   0,   0,   0, 0,
		0,   0,   0,   0,   0,   0,   0, 0,   						// 48..4f
		0,   0,   0,   0,   0,   0,   0, 0,
		KEY_CAPSLOCK, KEY_SHIFTR, KEY_ALT_RETURN, 0, 0, 0, 0, 0,   // 58..5f
		0, 0, 0, 0, 0, 0, KEY_ALT_BACKSPACE, 0,
		0, 0, 0, 0, 0, 0, 0, 0,   									// 68..6f
		0, 0, 0, 0, 0, 0, 0, 0,
		KEY_ALT_F11, 0, 0, 0, 0, 0, 0, 0,   						// 78..7f
		0, 0, 0, KEY_ALT_F7 									   	// 80..83
};

const uint16_t scancodes_altr[0x84] = {

		0, KEY_ALT_F9, 0, KEY_ALT_F5, KEY_ALT_F3, KEY_ALT_F1, KEY_ALT_F2, KEY_ALT_F12,  	// 00..07
		0, KEY_ALT_F10, KEY_ALT_F8, KEY_ALT_F6, KEY_ALT_F4, KEY_ALT_TAB, '^', 0,		// 08..0f
		0, KEY_ALTL, KEY_SHIFTL,   0,   KEY_CTRLL, '@',   0,   0,   // 10..17
		0,   0,   0,   0,   0,   0,   0, 0,							// 18..1f
		0,   0,   0,   0,   0,   0,   0, 0,   						// 20..27
		0,   0,   0,   0,   0,   0,   0, 0,   						// 28..2f
		0,   0,   0,   0,   0,   0,   0, 0, 						// 30..37
		0,   0,   0,   0,   0, '{', '[', 0,   						// 38..3f
		0,   0,   0,   0,   0, '}', ']', 0,
		0,   0,   0,   0,   0,   0,'\\', 0,   						// 48..4f
		0,   0,   0,   0,   0,   0,   0, 0,
		KEY_CAPSLOCK, KEY_SHIFTR, KEY_ALT_RETURN, '~', 0, 0, 0, 0,   // 58..5f
		0, '|', 0, 0, 0, 0, KEY_ALT_BACKSPACE, 0,
		0, 0, 0, 0, 0, 0, 0, 0,   									// 68..6f
		0, 0, 0, 0, 0, 0, 0, 0,
		KEY_ALT_F11, 0, 0, 0, 0, 0, 0, 0,   						// 78..7f
		0, 0, 0, KEY_ALT_F7 									   	// 80..83
};
#endif
