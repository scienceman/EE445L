// TimerCtrl.c
// Runs on LM3S1968
// Use Timer0 in periodic mode to request interrupts at a particular
// period. Contains Timer0 init to two 16-bit timers, handlers included in this file.
// Kevin Gilbert, Gilberto Rodriguez
// February 23, 2014
 #include "TimerCtrl.h"
 #include "Sonar.h"

 #include "../inc/hw_types.h"
 #include "../driverlib/interrupt.h"
 #include "../inc/hw_ints.h"
 #include "../driverlib/timer.h"
 #include "../inc/hw_timer.h"
 #include "../driverlib/sysctl.h"
 #include "../inc/hw_memmap.h"
 #include "../driverlib/debug.h"
 #include "../driverlib/gpio.h" 
 #include "lm3s811.h"


void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

extern tSonarModule left_sonar, right_sonar;

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

void Timer0_CaptureInit(void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_A_CAP_TIME | TIMER_CFG_B_PERIODIC | TIMER_CFG_SPLIT_PAIR);
	TimerLoadSet(TIMER0_BASE, TIMER_A, 10000);
	TimerControlEvent(TIMER0_BASE, TIMER_A, TIMER_EVENT_POS_EDGE);
	TimerEnable(TIMER0_BASE, TIMER_A); 
	TimerIntEnable(TIMER0_BASE, TIMER_CAPA_EVENT);
	IntEnable(INT_TIMER0A);
}

void Timer1_CaptureInit(void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	TimerConfigure(TIMER1_BASE, TIMER_CFG_A_CAP_TIME | TIMER_CFG_B_PERIODIC | TIMER_CFG_SPLIT_PAIR);
	TimerLoadSet(TIMER1_BASE, TIMER_A, 10000);
	TimerControlEvent(TIMER1_BASE, TIMER_A, TIMER_EVENT_POS_EDGE);
	TimerEnable(TIMER1_BASE, TIMER_A); 
	TimerIntEnable(TIMER1_BASE, TIMER_CAPA_EVENT);
	IntEnable(INT_TIMER1A);
}
									 
//Interrupt period is 50000000/32/440 = 3551 counts = 71ƒÊs
void Timer0A_Handler(void){
	TimerIntClear(TIMER0_BASE, TIMER_CAPA_EVENT);	// acknowledge
	right_sonar.echoTime = TIMER0_TAR_R;
	right_sonar.distance = (right_sonar.echoTime - right_sonar.triggerTime) / 58; 
	if(right_sonar.distance > 9999) {
		right_sonar.distance = 9999;
	}
}
														   
void Timer0B_Handler(void) {
	TimerIntClear(TIMER0_BASE, TIMER_TIMB_TIMEOUT);	// acknowledge
}

void Timer1A_Handler(void){
	TimerIntClear(TIMER1_BASE, TIMER_CAPA_EVENT);	// acknowledge
	left_sonar.echoTime = TIMER1_TAR_R;
	left_sonar.distance = (left_sonar.echoTime - left_sonar.triggerTime) / 58; 
	if(left_sonar.distance > 9999) {
	 	left_sonar.distance = 9999;
	}
}
														   
void Timer1B_Handler(void) {
	TimerIntClear(TIMER1_BASE, TIMER_TIMB_TIMEOUT);	// acknowledge
}


