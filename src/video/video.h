#pragma once

extern struct t_fixedFontInstance fixedFontInstanceVga;

enum e_videoMode {
	V280x200x8D,
	V280x200x8VD,
	V320x240x8,
	V320x240x8V,
	V280x400x8,
	V280x400x8V,
	V400LINE_FREERUN,
	VT40x30
};

#define VSYNC_TIMER_PERIOD 13344

enum e_videoSyncPolarity { NEGATIVE, POSITIVE };

#define MAX_SWITCHLIST_SIZE 10

#define VIDEO_SWITCH_PORT GPIOB
#define VIDEO_SWITCH_PORT_CLOCK RCC_AHB1Periph_GPIOB

struct t_videoInstance {

	uint16_t resx;
	uint16_t resy;
	uint32_t singleVramSize;
	uint8_t	 doubleBuffer;
	uint8_t  doubleScanline;

	// textmode options
	uint8_t  textMode;
	uint8_t  textRows;
	uint8_t  textCols;
	uint8_t  textLinesPerBuffer;
	uint16_t textCursor;
	uint8_t  textFx;
	uint16_t textScrollTrigger;

	// pixeldma
	uint8_t  pixelClock;

	// hsync
	uint16_t hsyncTimerPeriod;
	uint16_t hsyncPixelDmaStartOffset;
	uint16_t hsyncSyncLength;
	enum e_videoSyncPolarity hsyncPolarity;

	// vsync
	uint16_t vsyncTimerPeriod;
	uint16_t vsyncTimerPrescaler;
	uint16_t vsyncSyncLength;
	enum e_videoSyncPolarity vsyncPolarity;

	uint16_t lowerEndOfFrame;

	// precalculated for performance gain
	uint16_t hardwareResY;

	enum e_videoMode mode;

	// data buffer pointers
	uint8_t *vramTarget;
	uint8_t *vramDisplay;

	// status
	int16_t current_y;
	int32_t current_frame;

	// timecode
	uint8_t tc_frame;
	uint8_t tc_sec;
	uint8_t tc_min;

	// sync to vblank
	uint8_t vblank_flag;
	uint8_t vblank;

	// callback for freerun mode
	void (*freerunCallback)(uint16_t x, uint16_t y);

	uint8_t switchToBwAtLine[MAX_SWITCHLIST_SIZE];
	uint8_t switchToBwAtLineSize;
	uint8_t switchToColorAtLine[MAX_SWITCHLIST_SIZE];
	uint8_t switchToColorAtLineSize;


};

extern struct t_videoInstance videoInstance;

void videoInit(void);
void videoMode(enum e_videoMode videoMode);
void videoFlip(void);

void videoClearVidmem();
void videoClearTextmem();

#define VIDEORAMSIZE 112000
#define TEXTRAMSIZE (40*30)

#define VIDEO_PORT			GPIOE
#define VIDEO_PORT_CLOCK	RCC_AHB1Periph_GPIOE

#define DACLOCK_PORT		GPIOC
#define DACLOCK_PORT_CLOCK	RCC_AHB1Periph_GPIOC
#define DACLOCK_PIN			GPIO_Pin_7
#define DACLOCK_PINSOURCE	GPIO_PinSource7
#define DACLOCK_PIN_AF		GPIO_AF_TIM8


extern uint8_t vram[VIDEORAMSIZE] __attribute__ ((section (".sram1")));
extern char tram[TEXTRAMSIZE] __attribute__ ((section (".ccmram")));
extern uint8_t tram_fx[TEXTRAMSIZE] __attribute__ ((section (".ccmram")));

