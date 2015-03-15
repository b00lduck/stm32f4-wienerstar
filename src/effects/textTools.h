#pragma once

// direct
void textToolsDrawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t c);
void textToolsDrawText(uint8_t x, uint8_t y, char* text, uint8_t fx);

// TTY style
void textToolsPrint(char* text);
void textToolsPrintLn(char* text);
void textToolsPutCh(char c);
void textToolsSetFx(uint8_t fx);
void textToolsNewline();
void textToolsScrollUp();
