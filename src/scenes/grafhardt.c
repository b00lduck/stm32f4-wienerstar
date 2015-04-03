#include "main.h"
#include "grafhardt.h"
#include "effects/scroller.h"
#include "effects/analyzer.h"
#include "effects/fftAnalyzer.h"
#include "effects/simple.h"
#include "effects/fixedFont.h"

#include "../data/images/280x400/img280x400_spacetits.h"

struct t_scrollerInstance scrollerInstance;

uint8_t sceneGrafHardtInited = 0;

/**
 *
 */
void sceneGrafHardtInit() {

	videoMode(V280x400x8V);

	scrollerInit(&scrollerInstance, &fontInstanceXenon, "WELCOME TO REVISION 2015");

	#ifdef FFT_ENABLED
	fftAnalyzerCalc();
	#endif

	sceneGrafHardtInited = 1;
}

uint16_t maxPerf = 0;

void sceneGrafHardtUninit() {
	sceneGrafHardtInited = 0;
}

void sceneGrafHardtDraw(uint16_t timeGone) {

	if (!sceneGrafHardtInited) sceneGrafHardtInit();

	blankScreen(videoInstance.vramTarget);

	//showImage(videoInstance.vramTarget, img280x400_spacetits);

	#ifdef MUSIC_ENABLED
	  //analyzerRender(videoInstance.vramTarget);
	#endif

	scrollerRender(&scrollerInstance, videoInstance.vramTarget, 0, 100);

	#ifdef FFT_ENABLED
	fftAnalyzerRenderC(videoInstance.vramTarget, 140, 330);
	//fftAnalyzerRenderB(videoInstance.vramTarget, 10, 330);

	if (videoInstance.current_frame % 3 == 0)
		fftAnalyzerCalc();
	#endif

}

