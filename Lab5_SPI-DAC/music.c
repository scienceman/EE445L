/************************* Music.c *************************
 * EE445L - Lab05 
 * Spring 2014
 * SSI - DAC
 * Kevin Gilbert, Gilberto Rodriguez
 * 19 February 2014
 ***********************************************************/

 #include "music.h"
 #include "../inc/hw_types.h"
 #include "system.h"
 #include "../inc/hw_types.h"
#include "../driverlib/gpio.h"
#include "../inc/hw_gpio.h"
#include "../inc/hw_memmap.h"
#include "../driverlib/sysctl.h"
#include "../driverlib/interrupt.h"
#include "../inc/hw_timer.h"
#include "../driverlib/timer.h"
#include "../inc/hw_ints.h"
 
 extern tBoolean pause;
 extern int noteIndex2;
 extern int noteIndex;

 void Rewind(void){
	 Stop();
	 noteIndex=0;
	 noteIndex2=0;
 }
 void Play(unsigned short note){
	 pause = false;
	 IntEnable(INT_TIMER0A);
	 IntEnable(INT_TIMER1A);
}
	 	 
 void Stop(void){
	 pause = true;
	 IntDisable(INT_TIMER0A);
	 IntDisable(INT_TIMER1A);	 
 }
 
