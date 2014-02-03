#include "line.h"
#include "rit128x96x4.h"

//*************OLED Buffer*************************************************
// 4-bit color pixels, each char 2 pixels. Width = 128/2
unsigned char imageBuff[(128/2)*96] = {0};

void RIT128x96x4_ShowImage(void) {
    RIT128x96x4ImageDraw(imageBuff,0,0,128,96);
}

void RIT128x96x4_Line(int x1, int y1, int x2, int y2, unsigned char color) {

}

void RIT128x96x4_ClearImage(void) {
	int i,size;
	size = (128/2)*96;
	for(i=0;i<size;i++) {
	    imageBuff[i]=0;
	}
	RIT128x96x4ImageDraw(imageBuff,0,0,128,96);
}
