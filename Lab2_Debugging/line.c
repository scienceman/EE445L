#include "line.h"
#include "rit128x96x4.h"
#include "math.h"
#include "stdlib.h"

//*************OLED Buffer*************************************************
// 4-bit color pixels, each char 2 pixels. Width = 128/2
unsigned char imageBuff[(128/2)*96] = {0};

void RIT128x96x4_ShowImage(void) {
    RIT128x96x4ImageDraw(imageBuff,0,0,128,96);
}

void RIT128x96x4_Line(int x1, int y1, int x2, int y2, unsigned char color) {
	int dx,dy,index,sx,sy,err,e2;
	dx = abs(x2 - x1);
	dy = abs(y2 - y1);
	index = x1+y1*(128/2);
	if(x1 < x2) {
		sx = 1;	 	
	} else {
	 	sx = -1;
	}
	if(y1 < y2) {
	 	sy = 1;
	} else {
	 	sy = -1;
	}
	err = dx-dy;

	while(1) {
	 	imageBuff[index] = color;
		if(x1 == x2 && y1==y2){
			return;
		}
		e2=err*2;
		if(e2 > -dy) {
			err -= dy;
			x1 += sx;
		}
		if(x1 == x2 && y1 == y2) {
			imageBuff[x1+y1*(128/2)] = color;
			return;
		}
		if(e2 < dx) {
			err += dx;
			y1 += sy;
		}
		imageBuff[x1+y1*(128/2)] = color;
	}				 	
}

void RIT128x96x4_ClearImage(void) {
	int i,size;
	size = (128/2)*96;
	for(i=0;i<size;i++) {
	    imageBuff[i]=0;
	}
	RIT128x96x4ImageDraw(imageBuff,0,0,128,96);
}
