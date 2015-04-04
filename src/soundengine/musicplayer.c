#include "main.h"
#include "musicplayer.h"

#ifdef MODPLAYER_ENABLED
#include "../../data/music/mod/music_mikrofon.h"
#endif

#ifdef XMPLAYER_ENABLED
#include "../../data/music/xm/music_hybridsong.h"
#endif

void musicInit() {

	#ifdef MODPLAYER_ENABLED
		modplayerInit();
		modplayerLoadModule(music, music_size);
	#endif

	#ifdef SOFTSYNTH_ENABLED
		// This sucker needs no init
	#endif

	#ifdef XMPLAYER_ENABLED
		xmplayerInit();
		xmplayerLoadModule(music, music_size);
	#endif

}
