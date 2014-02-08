#include <stdio.h>
#include "OLED.h"
#include "line.h"
#include "SysTick.h"
#include "sound.h"
#include "Output.h"

// delay function for testing from sysctl.c
// which delays 3*ulCount cycles
#ifdef __TI_COMPILER_VERSION__
	//Code Composer Studio Code
	void Delay(unsigned long ulCount){
	__asm (	"    subs    r0, #1\n"
			"    bne     Delay\n"
			"    bx      lr\n");
}

#else
	//Keil uVision Code
	__asm void
	Delay(unsigned long ulCount)
	{
    subs    r0, #1
    bne     Delay
    bx      lr
	}

#endif

int main(void){
	// Initializations
	int i;
	Output_Init();
	Output_Color(15);
	 // Hardcoded clock example
	 RIT128x96x4_Line(32, 15, 32, 25, 15); 	//23 pixels from center, y coordinates divided by two
	 RIT128x96x4_Line(32, 71, 32, 81, 15);
	 RIT128x96x4_Line(13, 48, 20, 48, 15);  //24 pixels from center
	 RIT128x96x4_Line(44, 48, 51, 48, 15);

	 //Hour hand
	 RIT128x96x4_Line(32,48,42,68,15);
	 RIT128x96x4_Line(33,49,43,69,15);
	 RIT128x96x4_Line(31,47,41,67,15);
	 //Minute hand
	 RIT128x96x4_Line(32,48,15,36,15);
	 RIT128x96x4_ShowImage();


	// 
	while(1){
		//while(!update);	 // Wait for systick
		//reposition in RAM
		//redraw image on OLED
	}
	
}


