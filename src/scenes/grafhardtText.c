#include "main.h"
#include "grafhardtText.h"
#include "programs/basic/basic.h"


/**
 *
 */
void sceneGrafhardtTextInit() {

	prgBasicInit();

}


/*uint16_t i;
for(i=0;i<256;i++) {
	tram[i] = i;
	tram_fx[i] = i % 16;
}

for(i=0;i<16;i++) {
	sprintf(tram+280+i*20,"Hallo, Welt!");
	memset(tram_fx+280+i*20,i % 16,20);
}
*/

/**
 *
 */
void sceneGrafhardtTextDraw(uint16_t timeGone) {

	prgBasicDraw();

}

