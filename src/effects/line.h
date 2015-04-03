#pragma once

#include "inttypes.h"

void drawLine(uint8_t *target, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color);
void drawLineAA(uint8_t *target, int16_t x0, int16_t y0, int16_t x1, int16_t y1);
void drawWuLine (uint8_t *target, int16_t X0, int16_t Y0, int16_t X1, int16_t Y1);
void drawWuLineColor (uint8_t *target, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t r, uint8_t g, uint8_t b);
