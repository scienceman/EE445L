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
#include "lm3s1968.h"
 
 extern tBoolean pause;
 extern int noteIndex2;
 extern int noteIndex;
Music song;

 void Rewind(void){
	 Stop();
	 //song.current = song.begin;
	 noteIndex=0;
	 noteIndex2=0;
 }
 void Play(unsigned short note){
	 pause = false;
	 //DAC_Out(note);
	 IntEnable(INT_TIMER0A);
	 NVIC_ST_CTRL_R |= NVIC_ST_CTRL_INTEN;
}
	 	 
 void Stop(void){
	 pause = true;
	 IntDisable(INT_TIMER0A);
	 NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_INTEN;	 	 
 }
 
