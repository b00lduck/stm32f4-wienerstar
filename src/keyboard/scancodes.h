#include "config.h"

#ifdef ENABLE_KEYBOARD
enum {
	KEY_ESCAPE = 0x0100,

	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,

	KEY_SHIFT_F1,
	KEY_SHIFT_F2,
	KEY_SHIFT_F3,
	KEY_SHIFT_F4,
	KEY_SHIFT_F5,
	KEY_SHIFT_F6,
	KEY_SHIFT_F7,
	KEY_SHIFT_F8,
	KEY_SHIFT_F9,
	KEY_SHIFT_F10,
	KEY_SHIFT_F11,
	KEY_SHIFT_F12,

	KEY_ALT_F1,
	KEY_ALT_F2,
	KEY_ALT_F3,
	KEY_ALT_F4,
	KEY_ALT_F5,
	KEY_ALT_F6,
	KEY_ALT_F7,
	KEY_ALT_F8,
	KEY_ALT_F9,
	KEY_ALT_F10,
	KEY_ALT_F11,
	KEY_ALT_F12,

	KEY_BACKSPACE,
	KEY_TAB,
	KEY_RETURN,

	KEY_ALT_TAB,
	KEY_ALT_RETURN,
	KEY_ALT_BACKSPACE,

	KEY_SHIFT_TAB,
	KEY_SHIFT_RETURN,
	KEY_SHIFT_BACKSPACE,

	KEY_CAPSLOCK,
	KEY_SHIFTL,
	KEY_SHIFTR,
	KEY_CTRLL,
	KEY_ALTL,
	KEY_ALTR,

	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_HOME

};

#define CHAR_UUML			129
#define CHAR_AUML			132
#define CHAR_AUMLC			142
#define CHAR_OUML			148
#define CHAR_OUMLC			153
#define CHAR_UUMLC			154
#define CHAR_SZLIG			225

extern const uint16_t scancodes[0x84];
extern const uint16_t scancodes_e0[0x84];
extern const uint16_t scancodes_shift[0x84];
extern const uint16_t scancodes_altl[0x84];
extern const uint16_t scancodes_altr[0x84];

#endif
