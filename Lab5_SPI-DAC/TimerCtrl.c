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

unsigned short I;
unsigned short Volume = 1;

unsigned short DACout=300;
unsigned short wave1;
unsigned int noteIndex = 0;
unsigned int changeNote = 0;
unsigned int intCounter = 0;
unsigned short wave2;
unsigned int I2=0;
int dur2 = 1;
int noteIndex2=0;

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

void Timer0_Init(unsigned short periodA, unsigned short periodB) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	// 16 bits Timer, | TIMER0_CGF_R to not clobber timerA.
  	TimerConfigure(TIMER0_BASE, TIMER_CFG_B_PERIODIC | TIMER_CFG_A_PERIODIC | TIMER_CFG_SPLIT_PAIR);  
	TimerLoadSet(TIMER0_BASE, TIMER_A, periodA-1);
	TimerLoadSet(TIMER0_BASE, TIMER_B, periodB-1);
	TimerEnable(TIMER0_BASE, TIMER_B);
	TimerEnable(TIMER0_BASE, TIMER_A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMB_TIMEOUT);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	IntEnable(INT_TIMER0B);
	IntEnable(INT_TIMER0A);
}

void Timer1_Init(unsigned short periodA, unsigned short periodB) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	// 16 bits Timer, | TIMER1_CGF_R to not clobber timerA.
  	TimerConfigure(TIMER1_BASE, TIMER_CFG_B_PERIODIC | TIMER_CFG_A_PERIODIC | TIMER_CFG_SPLIT_PAIR);  
	TimerLoadSet(TIMER1_BASE, TIMER_A, periodA-1);
	TimerLoadSet(TIMER1_BASE, TIMER_B, periodB-1);
	TimerEnable(TIMER1_BASE, TIMER_B);
	TimerEnable(TIMER1_BASE, TIMER_A);
	TimerIntEnable(TIMER1_BASE, TIMER_TIMB_TIMEOUT);
	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	IntEnable(INT_TIMER1B);
	IntEnable(INT_TIMER1A);
}

int dur = 1;									 
//Interrupt period is 50000000/32/440 = 3551 counts = 71ƒÊs
void Timer0A_Handler(void){
	long critSection;
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);	// acknowledge
	GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2,!(GPIOPinRead(GPIO_PORTG_BASE, GPIO_PIN_2))); 
	if(mario[noteIndex].frequency) {
		critSection = StartCritical();
		wave1 = Wave[I];
		EndCritical(critSection);
	}
	I = (I+1)%32; // 0 to 31

	if(changeNote) {
		if(mario[noteIndex].frequency == 0) {
			I--;
		} else {
		 	TimerLoadSet(TIMER0_BASE, TIMER_A, mario[noteIndex].frequency ? mario[noteIndex].frequency : 2);
		}
		if(dur > mario[noteIndex].duration) {
			noteIndex = (noteIndex + 1) % MARIOLEN;
			dur=1;
		} else { dur++; }
		changeNote = 0;
	}
}
														   
void Timer0B_Handler(void) {
	long critSection;
	TimerIntClear(TIMER0_BASE, TIMER_TIMB_TIMEOUT);	// acknowledge
	intCounter = (intCounter + 1) % TEMPO+1;
	if(intCounter == TEMPO) {
		critSection = StartCritical();
	    changeNote = 1;
		EndCritical(critSection);
	
		if(mario[noteIndex].frequency == 0) {
			I--;
			if(dur > mario[noteIndex].duration) {
				noteIndex = (noteIndex + 1) % MARIOLEN;
				dur = 1;
			} else {
			 	dur++;
			}
		} 	
	}
}

void Timer1A_Handler(void) {
   	long critSection;
	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);  // acknowledge
	#ifndef MAINSINEWAVE
	if(mario2[noteIndex2].frequency) {
		critSection = StartCritical();
		wave2 = Wave[I2];
		EndCritical(critSection);
	}
	I2 = (I2+1)%32; // 0 to 31
	 
	if(changeNote) {
		if(mario2[noteIndex2].frequency == 0) {
			I2--;
		} else {
		 	 TimerLoadSet(TIMER1_BASE, TIMER_A, mario2[noteIndex2].frequency);
		}
		if(dur2 > mario2[noteIndex2].duration) {
			noteIndex2 = (noteIndex2 + 1) % MARIOLEN2;
			dur2=1;
		} else { dur2++; }
		changeNote = 0;
	}
	#endif
}

void Timer1B_Handler(void) {
	TimerIntClear(TIMER1_BASE, TIMER_TIMB_TIMEOUT);  // acknowledge
	DAC_Out(((wave1 + wave2)/2)*Volume);
}

