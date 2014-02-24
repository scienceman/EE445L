// TimerCtrl.c
// Runs on LM3S1968
// Use Timer0 in periodic mode to request interrupts at a particular
// period. Contains Timer0 init to two 16-bit timers, handlers included in this file.
// Kevin Gilbert, Gilberto Rodriguez
// February 23, 2014
 
 #include "system.h"
 #include "music.h"
 #include "TimerCtrl.h"

 #include "../inc/hw_types.h"
 #include "../driverlib/interrupt.h"
 #include "../inc/hw_ints.h"
 #include "../driverlib/timer.h"
 #include "../inc/hw_timer.h"
 #include "../driverlib/sysctl.h"
 #include "../inc/hw_memmap.h"
 #include "../driverlib/debug.h"
 #include "../driverlib/gpio.h"
 #include <stdio.h>

const unsigned short Wave[32] = {  
  125,143,159,175,189,200,208,213,215,213,208,
  200,189,175,159,143,125,107,91,75,61,50,
  42,37,35,37,42,50,61,75,91,107
};  

const unsigned short Wave2[32]= {
2048,2438,2813,3159,3462,3711,3896,4010,4048,4010,3896,
3711,3462,3159,2813,2438,2048,1658,1283,937,634,385,
200,86,48,86,200,385,634,937,1283,1658};

const unsigned short Wave3[32] = {  
  1250,1430,1590,1750,1890,2000,2080,2130,2150,2130,2080,
  2000,1890,1750,1590,1430,1250,1070,910,750,610,500,
  420,370,350,370,420,500,610,750,910,1070
};  

unsigned short I;
unsigned short Volume = 9;

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

void Timer0_Init(unsigned short period) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	// 16 bits Timer, | TIMER0_CGF_R to not clobber timerA.
  	TimerConfigure(TIMER0_BASE, TIMER_CFG_B_PERIODIC | TIMER_CFG_A_PERIODIC | TIMER_CFG_SPLIT_PAIR);  
	TimerLoadSet(TIMER0_BASE, TIMER_A, period-1);
	TimerLoadSet(TIMER0_BASE, TIMER_B, 65000);
	TimerEnable(TIMER0_BASE, TIMER_B);
	TimerEnable(TIMER0_BASE, TIMER_A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMB_TIMEOUT);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	IntEnable(INT_TIMER0B);
	IntEnable(INT_TIMER0A);
}

#define SONGLEN 24
#define TEMPO 100

unsigned int noteIndex = 0;
unsigned int changeNote = 0;
unsigned int intCounter = 0;
unsigned int notes[SONGLEN] = {B,B,B,0,A,A,A,A,G,G,G,G,B,B,B,B,A,A,A,A,G,G,G,G};

//Interrupt period is 50000000/32/440 = 3551 counts = 71É s
void Timer0A_Handler(void){
	long critSection;
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);	// acknowledge
	GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2,!(GPIOPinRead(GPIO_PORTG_BASE, GPIO_PIN_2)));
    critSection = StartCritical();
	I = (I+1)%32; // 0 to 31
	EndCritical(critSection);
	DAC_Out(Wave[I]*Volume);
	if(changeNote) {
	 	TimerLoadSet(TIMER0_BASE, TIMER_A, notes[noteIndex]);
		noteIndex = (noteIndex + 1) % SONGLEN;
		changeNote = 0;
	}
}
														   
void Timer0B_Handler(void) {
	TimerIntClear(TIMER0_BASE, TIMER_TIMB_TIMEOUT);	// acknowledge
	intCounter = (intCounter + 1) % TEMPO+1;
	if(intCounter == TEMPO) {
		changeNote = 1;		
	}
}

