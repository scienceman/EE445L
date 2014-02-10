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
#include "../inc/hw_types.h"
#include "../driverlib/gpio.h"
#include "../driverlib/pwm.h"
#include "../driverlib/sysctl.h"
#include "../driverlib/interrupt.h"
#include "../inc/hw_memmap.h" 
#include <stdlib.h>

#define PI 3.14159265

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

tBoolean alarm = false;
void DisplayTimeNumeric(int hour, int min);
void DisplayTimeClock(int hour, int min);

int main(void){
	// Initializations
	int HOUR;
	extern unsigned int count, hours, minutes;
	unsigned char output=0;
	//Switch_Init();
    SysTick_IE_Init(50000); //50000000 = 1 sec
	//Switch_Init();
  	Output_Init();
	Output_Color(15);

//	 for(HOUR=-6;HOUR<6;HOUR++) {
//	   RIT128x96x4_Line(66,46,(66+(15*(cos(HOUR*(PI/6))))),46+(sin(HOUR*(PI/6))*15),15);
//	  // printf("Hand: %lf,%lf\r",15*(cos(HOUR*(PI/6)))),46+(sin(HOUR*(PI/6))*15);
//	   RIT128x96x4_ShowImage();
//	   SysTick_Wait10ms(50);
//	   RIT128x96x4_Line(66,46,(66+(15*(cos(HOUR*(PI/6))))),46+(sin(HOUR*(PI/6))*15),0);
//	 }
//	 for(HOUR=-30;HOUR<30;HOUR++) {
//		RIT128x96x4_Line(66,46,(66+(25*(cos(HOUR*(PI/30))))),46+(sin(HOUR*(PI/30))*25),15);
//		RIT128x96x4_ShowImage();
//		SysTick_Wait10ms(10);
//		RIT128x96x4_Line(66,46,(66+(25*(cos(HOUR*(PI/30))))),46+(sin(HOUR*(PI/30))*25),0);
//	 }
	 EnableInterrupts();	  
//	 printf("(1) Set Time\r");
//     printf("(2) Set Alarm\r");
//     printf("(3) Turn on/off Alarm\r");
//     printf("(4) Display Mode\r");
	
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
	    GPIOPinTypeGPIOOutput(GPIO_PORTH_BASE, GPIO_PIN_0);
		alarm=false;

	while(1){  	
		//DisplayTimeNumeric(hours, minutes);
		DisplayTimeClock(hours, minutes);
		if(alarm){
		 	GPIOPinWrite(GPIO_PORTH_BASE,GPIO_PIN_0,output);
			SysTick_Wait10ms(1);
			output = ~GPIOPinRead(GPIO_PORTH_BASE,GPIO_PIN_0);
		}
		//SysTick_Wait10ms(50);       
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


