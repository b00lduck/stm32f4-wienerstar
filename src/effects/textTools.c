#include "main.h"
#include "textTools.h"

uint16_t textToolsOffset = 0;

void textToolsSetOffset(uint8_t x, uint8_t y) {
	textToolsOffset = x+y*videoInstance.textCols;
}

void textToolsPutChar(uint8_t x, uint8_t y, uint8_t c, uint8_t fx) {
	uint16_t ofs = textToolsOffset + x + y * videoInstance.textCols;
	tram[ofs] = c;
	tram_fx[ofs] = fx;
}

void textToolsDrawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t fx) {

	uint8_t i;
	textToolsSetOffset(x,y);

	textToolsPutChar(0,0,201,fx);
	textToolsPutChar(w-1,0,187,fx);
	textToolsPutChar(0,h-1,200,fx);
	textToolsPutChar(w-1,h-1,188,fx);

    for(i = 1; i < (w-1); i++) {
    	textToolsPutChar(i,0,205,fx);
    	textToolsPutChar(i,h-1,205,fx);
    }
    for(i = 1; i < (h-1); i++) {
    	textToolsPutChar(0,i,186,fx);
    	textToolsPutChar(w-1,i,186,fx);
    }

}

void textToolsDrawText(uint8_t x, uint8_t y, char* text, uint8_t fx) {

	textToolsSetOffset(x,y);

	strcpy(tram + textToolsOffset, text);
	memset(tram_fx + textToolsOffset, fx, strlen(text));

}

/**
 * scrolls the textscreen one line up
 * line 1 is discarded
 */
void textToolsScrollUp() {

	memcpy(tram,tram + videoInstance.textCols, videoInstance.textScrollTrigger - videoInstance.textCols);
	memset(tram + videoInstance.textScrollTrigger - videoInstance.textCols, 0, videoInstance.textCols);

	memcpy(tram_fx,tram_fx + videoInstance.textCols, videoInstance.textScrollTrigger - videoInstance.textCols);
	memset(tram_fx + videoInstance.textScrollTrigger - videoInstance.textCols, 7, videoInstance.textCols);
}

/**
 * print to cursor
 */
void textToolsPrint(char* text) {
	uint16_t len = strlen(text);

	while ((len + videoInstance.textCursor) >= videoInstance.textScrollTrigger) {
		textToolsScrollUp();
		videoInstance.textCursor -= videoInstance.textCols;
	}
	memset(tram_fx + videoInstance.textCursor,videoInstance.textFx,len);
	strcpy(tram + videoInstance.textCursor, text);
	videoInstance.textCursor += len;
}

/**
 * print to cursor
 */
void textToolsPrintLn(char* text) {
	textToolsPrint(text);
	textToolsNewline();
}

/**
 * print to cursor
 */
void textToolsPutCh(char c) {
	if ((videoInstance.textCursor + 1) >= videoInstance.textScrollTrigger) {
		textToolsScrollUp();
		videoInstance.textCursor -= videoInstance.textCols;
	}
	*(tram_fx + videoInstance.textCursor) = videoInstance.textFx;
	*(tram + videoInstance.textCursor) = c;
	videoInstance.textCursor += 1;
}

/**
 * set fx flag
 */
void textToolsSetFx(uint8_t fx) {
	videoInstance.textFx = fx;
}

/**
 * CR+LF
 */
void textToolsNewline() {

	// carriage return
	videoInstance.textCursor -= (videoInstance.textCursor % videoInstance.textCols);

	// linefeed
	videoInstance.textCursor += videoInstance.textCols;

	if(videoInstance.textCursor >= videoInstance.textScrollTrigger) {
		textToolsScrollUp();
		videoInstance.textCursor -= videoInstance.textCols;
	}

}
