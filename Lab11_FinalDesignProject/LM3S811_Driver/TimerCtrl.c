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

tBoolean rising_x = true;
tBoolean rising_y = true;
unsigned long t_rise1_y=2, t_rise2_y=0, t_fall_y=1; 
unsigned long t_rise1_x=2, t_rise2_x=0, t_fall_x=1;
extern tSonarModule left_sonar, right_sonar;
extern unsigned int x_axis_accel, y_axis_accel;

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
	TimerConfigure(TIMER0_BASE, TIMER_CFG_A_CAP_TIME | TIMER_CFG_B_CAP_TIME | TIMER_CFG_SPLIT_PAIR);
	//TIMER0_TAILR_R = 0x0000FFFF;
	TimerLoadSet(TIMER0_BASE, TIMER_A, 10000);
	TimerLoadSet(TIMER0_BASE, TIMER_B, 10000);
	TimerControlEvent(TIMER0_BASE, TIMER_A, TIMER_EVENT_POS_EDGE);
	TimerControlEvent(TIMER0_BASE, TIMER_B, TIMER_EVENT_POS_EDGE);
	TimerEnable(TIMER0_BASE, TIMER_A); 
	TimerEnable(TIMER0_BASE, TIMER_B);
	TimerIntEnable(TIMER0_BASE, TIMER_CAPA_EVENT);
	TimerIntEnable(TIMER0_BASE, TIMER_CAPB_EVENT);
	IntEnable(INT_TIMER0A);
	IntEnable(INT_TIMER0B);
}

void Timer1_CaptureInit(void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	TimerConfigure(TIMER1_BASE, TIMER_CFG_A_CAP_TIME | TIMER_CFG_B_PERIODIC | TIMER_CFG_SPLIT_PAIR);
	TIMER1_TAILR_R = 0x0000FFFF;
	TimerLoadSet(TIMER1_BASE, TIMER_A, 10000);
	TimerLoadSet(TIMER1_BASE, TIMER_B, 10000);
	TimerControlEvent(TIMER1_BASE, TIMER_A, TIMER_EVENT_POS_EDGE);
	TimerEnable(TIMER1_BASE, TIMER_A); 
	TimerIntEnable(TIMER1_BASE, TIMER_CAPA_EVENT);
	IntEnable(INT_TIMER1A);
}

void Timer2_CaptureInit(void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
	TimerConfigure(TIMER2_BASE, TIMER_CFG_A_PERIODIC | TIMER_CFG_B_CAP_TIME | TIMER_CFG_SPLIT_PAIR);
	TimerLoadSet(TIMER2_BASE, TIMER_A, 10000);
	TimerLoadSet(TIMER2_BASE, TIMER_B, 10000);
	TimerControlEvent(TIMER2_BASE, TIMER_B, TIMER_EVENT_POS_EDGE);
	TimerEnable(TIMER2_BASE, TIMER_B); 
	TimerIntEnable(TIMER2_BASE, TIMER_CAPB_EVENT);
	IntEnable(INT_TIMER1B);
}
									 
//Interrupt period is 50000000/32/440 = 3551 counts = 71ƒÊs
/**
* 	Right Sonar Distance Measure
*/
void Timer0A_Handler(void){
	TimerIntClear(TIMER0_BASE, TIMER_CAPA_EVENT);	// acknowledge
	right_sonar.echoTime = TIMER0_TAR_R;
	right_sonar.distance = (right_sonar.triggerTime - right_sonar.echoTime) / 58; 
	if(right_sonar.distance > 999) {
		right_sonar.distance = 999;
	}
}

/**
* 	Accelerometer Y-axis Handler
*/														   
void Timer0B_Handler(void) {
	TimerIntClear(TIMER0_BASE, TIMER_CAPB_EVENT);	// acknowledge
	if(rising_y) {
		TimerControlEvent(TIMER0_BASE, TIMER_B, TIMER_EVENT_NEG_EDGE);
		rising_y = false;
	  	//(t2-t1)/(t3-t1) -> count down timer (t1-t2)/(t1-t3)
		t_rise2_y = TIMER0_TBR_R;
		y_axis_accel = (t_rise1_y-t_fall_y)/(t_rise1_y-t_rise2_y);
		t_rise1_y = t_rise2_y; 
	} else {					 
	   	TimerControlEvent(TIMER0_BASE, TIMER_B, TIMER_EVENT_POS_EDGE);
		rising_y = true;
		t_fall_y = TIMER0_TBR_R;
	} 
}

/**
* 	Left Sonar Distance Measure
*/
void Timer1A_Handler(void){
	TimerIntClear(TIMER1_BASE, TIMER_CAPA_EVENT);	// acknowledge
	left_sonar.echoTime = TIMER1_TAR_R;
	left_sonar.distance = (left_sonar.triggerTime - left_sonar.echoTime) / 58; 
	if(left_sonar.distance > 999) {
	 	left_sonar.distance = 999;
	}
}
														   
void Timer1B_Handler(void) {
	TimerIntClear(TIMER1_BASE, TIMER_TIMB_TIMEOUT);	// acknowledge
}

void Timer2A_Handler(void) {
	TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
}

/**
* 	Accelerometer X-axis Handler
*/
void Timer2B_Handler(void) {
	TimerIntClear(TIMER2_BASE, TIMER_CAPB_EVENT);
	 if(rising_x) {
		TimerControlEvent(TIMER2_BASE, TIMER_B, TIMER_EVENT_NEG_EDGE);
		rising_x = false;
	  	//(t2-t1)/(t3-t1) -> count down timer (t1-t2)/(t1-t3)
		t_rise2_x = TIMER2_TBR_R;
		x_axis_accel = (t_rise1_x-t_fall_x)/(t_rise1_x-t_rise2_x);
		t_rise1_x = t_rise2_x; 
	} else {
	   	TimerControlEvent(TIMER2_BASE, TIMER_B, TIMER_EVENT_POS_EDGE);
		rising_x = true;
		t_fall_x = TIMER2_TBR_R;
	}
}


