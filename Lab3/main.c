#include <stdio.h>
#include "OLED.h"
#include "line.h"
#include "SysTick.h"									 
#include "sound.h"
#include "Output.h"
#include "switch.h"
#include "SysTickInts.h"
#include "lm3s1968.h"
#include "math.h"

#define PI 3.14159265

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
	//int i = 0;
	int HOUR;
	Switch_Init();
    SysTick_IE_Init(50000000);
  	Output_Init();
	Output_Color(15);
	 // Hardcoded clock example
    //printf("%lf\r",cos(PI/30)*15);	

	 //DisplayTimeNumeric();
	 for(HOUR=-6;HOUR<6;HOUR++) {
	   RIT128x96x4_Line(66,46,(66+(15*(cos(HOUR*(PI/6))))),46+(sin(HOUR*(PI/6))*15),15);
	  // printf("Hand: %lf,%lf\r",15*(cos(HOUR*(PI/6)))),46+(sin(HOUR*(PI/6))*15);
	   RIT128x96x4_ShowImage();
	   SysTick_Wait10ms(50);
	   RIT128x96x4_Line(66,46,(66+(15*(cos(HOUR*(PI/6))))),46+(sin(HOUR*(PI/6))*15),0);
	 }
	 for(HOUR=-100;HOUR<100;HOUR++) {
		RIT128x96x4_Line(66,46,(66+(25*(cos(HOUR*(PI/30))))),46+(sin(HOUR*(PI/30))*25),15);
		RIT128x96x4_ShowImage();
		SysTick_Wait10ms(10);
		RIT128x96x4_Line(66,46,(66+(25*(cos(HOUR*(PI/30))))),46+(sin(HOUR*(PI/30))*25),0);
	 }
	 //Hour hand
	 //RIT128x96x4_Line(64,48,72,60,15);
	 //RIT128x96x4_Line(66,46,(66+(15*(cos(HOUR*(PI/6))))),46+(HOUR*sin(PI/6)*15),15);
	 //RIT128x96x4_Line(66,49,86,69,15);
	 //RIT128x96x4_Line(62,47,82,67,15);
	 //Minute hand
	 //RIT128x96x4_Line(66,46,66,21,15);
	 //RIT128x96x4_Line(66,46,(66+(25*(cos(HOUR*(PI/30))))),46+(HOUR*sin(PI/30)*25),15);
	 //RIT128x96x4_ShowImage();
	 EnableInterrupts();	  
//	 printf("(1) Set Time\r");
//     printf("(2) Set Alarm\r");
//     printf("(3) Turn on/off Alarm\r");
//     printf("(4) Display Mode\r");
	// 
	while(1){         
//		while((GPIO_PORTG_DATA_R&0xF0)>= 0xF0);   // Do nothing, wait for button press
//		// Determine which button was pressed
//		switch(GPIO_PORTG_DATA_R&0xF0) {
//			case(0xE0):   //1110 (button button)	
//				//printf("%x\r",GPIO_PORTG_DATA_R);
//				
//				break;
//			case(0xD0):   //1101 (left button)
//				//printf("%x\r",GPIO_PORTG_DATA_R);
//				break;
//			case(0xB0):	  //1011 (right button)
//				//printf("%x\r",GPIO_PORTG_DATA_R);
//				break;
//			case(0x70):    //0111 (select)
//				//printf("%x\r",GPIO_PORTG_DATA_R); 
//				//ToggleDisplayMode();
//				Draw_Clock();
//				break; 
//			default:
//				printf("Hit Default\r");
//				break;
//		} 
		//redraw image on OLED
    }
	
}


