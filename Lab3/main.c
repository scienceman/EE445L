#include <stdio.h>
#include "OLED.h"
#include "line.h"
#include "SysTick.h"
#include "sound.h"
#include "Output.h"
#include "switch.h"
#include "lm3s1968.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

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
	int i = 0;
	Switch_Init();
    SysTick_IE_Init(250000);
    Output_Init();
	Output_Color(15);
	 // Hardcoded clock example
	 //Draw_Clock();

	 //Hour hand
	 RIT128x96x4_Line(64,48,72,60,15);
	 //RIT128x96x4_Line(66,49,86,69,15);
	 //RIT128x96x4_Line(62,47,82,67,15);
	 //Minute hand
	 RIT128x96x4_Line(64,48,35,40,15);
	 RIT128x96x4_ShowImage();
	 EnableInterrupts();	  


	// 
	while(1){
		//while(!update);	 // Wait for systick
		//reposition in RAM
		//redraw image on OLED
	    if (i == 500000){
            //GPIO_PORTG_DATA_R ^= 0x04; //heartbeat
            i = 0;
        }
        //printf("(1) Set Time\r");
        //printf("(2) Set Alarm\r");
        //printf("(3) Turn on/off Alarm\r");
        //printf("(4) Display Mode\r");
        i += 1;
    }
	
}


