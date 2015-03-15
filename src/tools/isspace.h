#pragma once

static inline uint8_t isspace8(char c) {
	if (c == 32) return 1;
	if (c == 0) return 1;
	return 0;
}
