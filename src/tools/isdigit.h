#pragma once

static inline uint8_t isdigit8(char c) {
	if ((c >= '0') && (c <= '9')) return 1;
	return 0;
}
