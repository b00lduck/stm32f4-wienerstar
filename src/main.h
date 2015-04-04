#include <string.h>
#include <stdio.h>
#include "config.h"
#include "macros.h"

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "stm32f4_discovery_audio_codec.h"
#include "stm32f4xx_it.h"

#include "video/video.h"
#include "keyboard/keyboard.h"
#include "keyboard/keyboardParser.h"

extern struct t_fontInstance fontInstanceXenon;
extern struct t_fontInstance fontInstanceBlazingStar;
extern uint32_t frameCount;

extern const char img280x200_title_bw[];
