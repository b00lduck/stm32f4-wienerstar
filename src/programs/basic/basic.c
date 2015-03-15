#include "main.h"
#include "keyboard/scancodes.h"
#include "expression.h"
#include "effects/textTools.h"

#include "tools/reset.h"

/**
 *
 */
void prgBasicInit() {

	videoMode(VT40x30);

	textToolsDrawBox(0,0,40,7,15);
	textToolsDrawText(5,2,"Welcome to b\x01\x01lduck STM32play",14);
	textToolsDrawText(1,4,"Type \"PROGS\" to see a list of programs",12);

	videoInstance.textCursor = 8*40;
	textToolsPrintLn("Ready.");

}

/**
 *
 */
void prgBasicPreParseLine(char *lineOut) {

	uint8_t lineNum = videoInstance.textCursor / videoInstance.textCols;
	char *linePointer = tram + lineNum * videoInstance.textCols;

	uint8_t srcOffset = 0;
	uint8_t targetOffset = 0;

	uint8_t quoted = 0;

	while ( srcOffset < 40 ) {
		uint8_t c = *(linePointer+srcOffset);

		if ((c == 0) || (c == ' ')) {
			// whitespace
			if (quoted) {
				lineOut[targetOffset++] = *(linePointer+srcOffset);
			}
		} else if (c == '"') {
			// ": switch quotation flag
			lineOut[targetOffset++] = *(linePointer+srcOffset);
			if (quoted) {
				quoted = 0;
			} else {
				quoted = 1;
			}
		} else if ((c>=97) && (c<=122)) {
			// a..z: make uppercase
			if (quoted) {
				lineOut[targetOffset++] = *(linePointer+srcOffset);
			} else {
				lineOut[targetOffset++] = *(linePointer+srcOffset) - 32;
			}
		} else if ((c>=65) && (c<=90)) {
			// A..Z: just copy
			lineOut[targetOffset++] = *(linePointer+srcOffset);
		} else if ((c >= '0') && (c <= '9')) {
			// 0..9: just copy
			lineOut[targetOffset++] = *(linePointer+srcOffset);
		} else if ((c == '+') || (c == '-') || (c == '/') || (c == '*') ||
				   (c == '=') || (c == '^') || (c == '.')) {
			// operators: just copy
			lineOut[targetOffset++] = *(linePointer+srcOffset);
		} else if ((c == '(') || (c == ')') || (c == '[') || (c == ']') ) {
			// parenthesis: just copy
			lineOut[targetOffset++] = *(linePointer+srcOffset);
		}

		srcOffset++;
	};

	lineOut[targetOffset] = 0;

}

#define NUMTOKENS 5
char *tokens[NUMTOKENS] = { "", "PRINT", "LOCATE", "CLS", "RESET" };
enum e_tokens { TOKEN_NULL = 0, TOKEN_PRINT, TOKEN_LOCATE, TOKEN_CLS, TOKEN_RESET };

/**
 * command must be zero terminated
 */
void prgBasicParseCommand(char *command, char *parsedCommand) {

	uint8_t tokenId = 0;
	uint8_t tokenLen = 0;

	uint8_t i;
	for(i=1;i<NUMTOKENS;i++) {
		tokenLen = strlen(tokens[i]);
		if (memcmp(tokens[i],command,tokenLen) == 0) {
			*(parsedCommand) = i;
			tokenId = i;
			break;
		}
	}

	if (tokenId == 0) {
		textToolsPrint("Parse error: No command found.");
		textToolsNewline();
		return;
	}

	strcpy(parsedCommand + 1, command + strlen(tokens[tokenId]));

	//textToolsPrintLn("Parsed line:");
	//textToolsPrintLn(parsedCommand);

}

/**
 *
 */
void prgBasicExecuteParsedCommand(char *parsedCommand) {

	float f;

	switch(*parsedCommand++) {
		case TOKEN_PRINT:

			f = expressionEvaluateExpression(parsedCommand);

			char x[20];

			sprintf(x,"%d",(int)f);

			textToolsPrintLn(x);

			return;

		case TOKEN_LOCATE:
			  textToolsPrintLn("Error: LOCATE known but unimplemented.");
			  break;

		case TOKEN_RESET:
			  textToolsPrintLn("Rebooting system...");
			  reset();
			  return;

		case TOKEN_CLS:
			videoClearTextmem();
			videoInstance.textCursor = 0;
			return;

	}

	textToolsPrintLn("Error: Unknown token.");
	return;

}

/**
 *
 */
void prgBasicInterpreteLine() {

	char line[41];
	char parsedLine[41];

	prgBasicPreParseLine(line);

	textToolsNewline();

	if ((line[0] >= '0') && (line[0] <= '9')) {
		// program mode
		textToolsPrintLn("Error: Program mode not yet implemented.");
	} else {
		// direct mode

		prgBasicParseCommand(line,parsedLine);
		prgBasicExecuteParsedCommand(parsedLine);

		textToolsPrintLn("Ready.");

	}

}

/**
 *
 */
void prgBasicDraw() {

	if (keyboardParserPollKeyboard()) {
		//keyboardParserVisualizeKeystroke();
		uint16_t keycode;
		uint8_t breaked;

		keyboardParserProcessKeystroke(&keycode, &breaked);
		keyboardParserClearKeystroke();

		if (!breaked) {
			if (keycode & 0xff00) {
				switch(keycode) {
					case KEY_RETURN:
						prgBasicInterpreteLine();
						break;

					case KEY_UP:
						if (videoInstance.textCursor >= videoInstance.textCols) {
							videoInstance.textCursor -= videoInstance.textCols;
						}
						break;

					case KEY_LEFT:
						if (videoInstance.textCursor >= 1) {
							videoInstance.textCursor -= 1;
						}
						break;

					case KEY_RIGHT:
						videoInstance.textCursor += 1;
						if (videoInstance.textCursor >= videoInstance.textScrollTrigger) {
							textToolsScrollUp();
							videoInstance.textCursor -= videoInstance.textCols;
						}
						break;

					case KEY_DOWN:
						videoInstance.textCursor += videoInstance.textCols;
						if (videoInstance.textCursor >= videoInstance.textScrollTrigger) {
							textToolsScrollUp();
							videoInstance.textCursor -= videoInstance.textCols;
						}
						break;

					case KEY_HOME:
						videoInstance.textCursor = 0;
						break;

				}

			} else {
				if ( keycode > 0 ) {
					textToolsPutCh(keycode);
				}
			}
		}
	}

}
