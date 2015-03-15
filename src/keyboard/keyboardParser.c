#include "config.h"
#ifdef ENABLE_KEYBOARD
#include "main.h"
#include "keyboard.h"
#include "keyboardParser.h"
#include "scancodes.h"
#include "effects/textTools.h"

// status variables for scancode parsing
uint8_t keyboardBreak = 0;
uint8_t keyboardEscape = 0;
uint8_t keyboardEscape2 = 0;

struct t_keystroke keystroke;

//struct t_keystroke keystrokeBuffer[64];


uint8_t shiftl = 0;
uint8_t shiftr = 0;
uint8_t altl = 0;
uint8_t altr = 0;

void keyboardParserInit(void) {
	keyboardParserClearKeystroke();
}

void keyboardParserClearKeystroke() {
	keystroke.code = 0;
	keystroke.brk = 0;
	keystroke.esc = 0;
}

/**
 * parse next scancode to keystroke
 */
void keyboardParserParseScancode(uint8_t scancode) {

	if (scancode == 0) return;

	if (scancode & 0x80) {

		switch(scancode) {

			case 0x83:
				// F7
				break;

			case 0xf0:
				keyboardBreak = 1;
				return;

			case 0xaa:
				// "keyboard ok" message from keyboard, skip
				return;

			default:
				keyboardEscape = scancode;
				return;

		}
	}

	if ((keyboardEscape == 0xe1) && (!keyboardEscape2)) {
		keyboardEscape2 = scancode;
		return;
	}

	if (keyboardBreak) {
		keystroke.brk = 1;
		keyboardBreak = 0;
	} else {
		keystroke.brk = 0;
	}

	if (keyboardEscape) {
		keystroke.esc = keyboardEscape;
		keyboardEscape = 0;
	}

	if (keyboardEscape2) {
		keyboardEscape2 = 0;
	}

	keystroke.code = scancode;

}

/**
 * returns true if a keystroke is ready
 */
uint8_t keyboardParserPollKeyboard() {
	uint8_t c = keyboardFifoGet();
	keyboardParserParseScancode(c);
	return (keystroke.code != 0);
}

/**
 *
 */
void keyboardParserProcessKeystroke(uint16_t *keycode, uint8_t *breaked) {

	if (keystroke.esc == 0xe0) {
		*keycode = scancodes_e0[keystroke.code];
	} else if (shiftl || shiftr) {
		*keycode = scancodes_shift[keystroke.code];
	} else if (altl) {
		*keycode = scancodes_altl[keystroke.code];
	} else if (altr) {
		*keycode = scancodes_altr[keystroke.code];
	} else {
		*keycode = scancodes[keystroke.code];
	}

	*breaked = (keystroke.brk);

	switch(*keycode) {

		case KEY_SHIFTL:
			shiftl = (!keystroke.brk);
			break;

		case KEY_SHIFTR:
			shiftr = (!keystroke.brk);
			break;

		case KEY_ALTL:
			altl = (!keystroke.brk);
			break;

		case KEY_ALTR:
			altr = (!keystroke.brk);
			break;
	}




}

/**
 * visualizes the ready keystroke on the text screen
 */
void keyboardParserVisualizeKeystroke() {

	char x[64];

	if (keystroke.brk) {
		textToolsSetFx(12);
		textToolsPrint("Keyup:   ");
	} else {
		textToolsSetFx(11);
		textToolsPrint("Keydown: ");
	}

	if (keystroke.esc) {
		textToolsSetFx(13);
		sprintf(x,"%02x ", keystroke.esc);
		textToolsPrint(x);
	}

	sprintf(x,"%02x", keystroke.code);
	textToolsSetFx(14);
	textToolsPrint(x);

	textToolsNewline();

}
#endif
