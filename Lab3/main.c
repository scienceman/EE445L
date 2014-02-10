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

tBoolean alarmdicks = false;

int main(void){
	// Initializations
	extern unsigned int count, hours, minutes;
	extern int displayClock;
	unsigned char output=0;
	//SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
	Switch_Init();
    SysTick_IE_Init(500000); //50000000 = 1 sec  500000000000
  	Output_Init();
	Output_Color(15);

	EnableInterrupts();	  
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
	GPIOPinTypeGPIOOutput(GPIO_PORTH_BASE, GPIO_PIN_0);
	alarmdicks=false;

	while(1){  	
		if(displayClock) {
		    DisplayTimeClock(hours, minutes, count);
		} else {
			DisplayTimeNumeric(hours, minutes, count);
		}
		//GPIOPortG_Wait();
		if(alarmdicks){
		 	GPIOPinWrite(GPIO_PORTH_BASE,GPIO_PIN_0,output);
			//SysTick_Wait10ms(1);
			output = ~GPIOPinRead(GPIO_PORTH_BASE,GPIO_PIN_0);
		}
		SysTick_Wait10ms(1);       
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


