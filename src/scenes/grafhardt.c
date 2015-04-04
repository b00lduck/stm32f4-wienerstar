#include "main.h"
#include "grafhardt.h"
#include "effects/scroller.h"
#include "effects/analyzer.h"
#include "effects/fftAnalyzer.h"
#include "effects/simple.h"
#include "effects/fixedFont.h"

struct t_scrollerInstance scrollerInstance;

uint8_t sceneGrafHardtInited = 0;

/**
 *
 */
void sceneGrafHardtInit() {

	videoMode(V280x200x8VD);

	scrollerInit(&scrollerInstance, &fontInstanceBlazingStar, "GREETINGS FLY OUT TO FRIEDI AND MR.GROISS (THX FOR THE MICROPHONE!), BOCKY THE ITALIAN STALLION, PRALUCEO AND MR.BRASS...");

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

	#ifdef MUSIC_ENABLED
	  //analyzerRender(videoInstance.vramTarget);
	#endif

	scrollerRender(&scrollerInstance, videoInstance.vramTarget, 0, 100);

	#ifdef FFT_ENABLED
	//fftAnalyzerRenderC(videoInstance.vramTarget, 140, 330);
	//fftAnalyzerRenderB(videoInstance.vramTarget, 10, 330);

	if (videoInstance.current_frame % 3 == 0)
		fftAnalyzerCalc();
	#endif

}

