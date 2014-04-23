// TimerCtrl.c
// Runs on LM3S1968
// Use Timer0 in periodic mode to request interrupts at a particular
// period. Contains Timer0 init to two 16-bit timers, handlers included in this file.
// Kevin Gilbert, Gilberto Rodriguez
// February 23, 2014
 #include "TimerCtrl.h"
 #include "Sonar.h"
 #include "lm3s1968.h"

 #include "../inc/hw_types.h"
 #include "../driverlib/interrupt.h"
 #include "../inc/hw_ints.h"
 #include "../driverlib/timer.h"
 #include "../inc/hw_timer.h"
 #include "../driverlib/sysctl.h"
 #include "../inc/hw_memmap.h"
 #include "../driverlib/debug.h"
 #include "../driverlib/gpio.h"
 #include "../driverlib/pin_map.h"
 #include <stdio.h>  

extern tSonarModule sonar;
tBoolean newSonar = false;

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
	//TimerLoadSet(TIMER0_BASE, TIMER_B, periodB-1);
	//TimerEnable(TIMER0_BASE, TIMER_B);
	TimerEnable(TIMER0_BASE, TIMER_A);
	//TimerIntEnable(TIMER0_BASE, TIMER_TIMB_TIMEOUT);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	//IntEnable(INT_TIMER0B);
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

void Timer1_CaptureInit(void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	TimerConfigure(TIMER1_BASE, TIMER_CFG_A_CAP_TIME | TIMER_CFG_B_PERIODIC | TIMER_CFG_SPLIT_PAIR);
	TimerControlEvent(TIMER1_BASE, TIMER_A, TIMER_EVENT_POS_EDGE);
	TimerEnable(TIMER1_BASE, TIMER_A); 
	TimerIntEnable(TIMER1_BASE, TIMER_CAPA_EVENT);
	IntEnable(INT_TIMER1A);
}
					 
//Interrupt period is 50000000/32/440 = 3551 counts = 71É s
void Timer0A_Handler(void){	  
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);	// acknowledge
	Sonar_Trigger(&sonar);
}

void Timer0B_Handler(void) {
   TimerIntClear(TIMER0_BASE, TIMER_TIMB_TIMEOUT);
}
														   
void Timer1A_Handler(void) {
	TimerIntClear(TIMER1_BASE, TIMER_CAPA_EVENT);	// acknowledge
	GPIO_PORTG_DATA_R ^= 0x04;
	sonar.echoTime = TIMER1_TAR_R;
	sonar.distance = (sonar.echoTime - sonar.triggerTime) / 58; 
	newSonar = true;
}
