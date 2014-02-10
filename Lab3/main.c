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
#include "rit128x96x4.h"
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

tBoolean alarmflag = false;

int main(void){
	// Initializations
	extern unsigned int count, hours, minutes;
	extern int displayClock, main_menu, set_time, button, set_alarm, alarm_hours, alarm_minutes;
	unsigned char output=0;
	//SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
	Switch_Init();
    SysTick_IE_Init(50000000); //50000000 = 1 sec  500000000000
  	Output_Init();
	Output_Color(15);

	EnableInterrupts();	  
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
	GPIOPinTypeGPIOOutput(GPIO_PORTH_BASE, GPIO_PIN_0);
	alarmflag=false;

	while(1){
		if(main_menu){
			RIT128x96x4Clear();
			printf("(1) Display Mode\r");
			printf("(2) Set Time\r");
			printf("(3) Set Alarm\r");
			printf("(4) Turn On/Off Alarm\n");
			while(!button);
			RIT128x96x4Clear();
			button = 0;
		}
		if(set_time){
			DisplayTimeNumeric(hours, minutes, count);
		} else if(set_alarm){
			DisplayAlarmNumeric(alarm_hours, alarm_minutes);
		}else if(displayClock) {
		    DisplayTimeClock(hours, minutes, count);
		} else {
			DisplayTimeNumeric(hours, minutes, count);
		}
		//GPIOPortG_Wait();
		if(alarmflag && (hours == alarm_hours) && (minutes == alarm_minutes)){
		 	GPIOPinWrite(GPIO_PORTH_BASE,GPIO_PIN_0,output);
			//SysTick_Wait10ms(1);
			output = ~GPIOPinRead(GPIO_PORTH_BASE,GPIO_PIN_0);
		}
		SysTick_Wait10ms(1);       
    }	
}


