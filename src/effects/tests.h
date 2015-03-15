
void drawTestGraphic(uint8_t *target);
void drawVerticals(uint8_t *target);
void drawVerticalsNumLines(uint8_t *target, uint16_t lines);
void drawVerticalsNumLinesRows(uint8_t *target, uint16_t lines, uint16_t rows);
void drawVerticals2(uint8_t *target, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void drawColorbars(uint8_t *target, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void drawColorbarsInverted(uint8_t *target, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void drawGradient(uint8_t *target, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t channel);
void drawGradients(uint8_t *target, uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void drawAllColors4x1(uint8_t *target, uint16_t x, uint16_t y, uint8_t scale);
void drawAllColors2x2(uint8_t *target, uint16_t x, uint16_t y, uint8_t scale);
