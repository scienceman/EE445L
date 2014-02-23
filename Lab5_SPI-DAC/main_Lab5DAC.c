// main_Lab5DAC.c
// Runs on LM3S1968
// EE445L - Spring 2014 Lab5
// DAC and SSI
// Kevin Gilbert, Gilberto Rodriguez
// February 19, 2014
 #include "system.h"
 #include "music.h"
 #include "lm3s1968.h"
 #include "Timer.h"
 #include "../inc/hw_types.h"
 #include "../driverlib/interrupt.h"
 #include "../inc/hw_ints.h"
 #include "../driverlib/timer.h"
 #include "../inc/hw_timer.h"
 #include "../driverlib/sysctl.h"
 #include "../inc/hw_memmap.h"
 #include "../driverlib/debug.h"
 #include "../driverlib/gpio.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

void UserTask(void){
  //GPIO_PORTC_DATA_R ^= 0x20;
  	GPIO_PORTG_DATA_R ^= 0x04;
}

/*void Timer0B_Handler(void) {
	TimerIntClear(TIMER0_BASE, TIMER_TIMB_TIMEOUT);	// acknowledge
	GPIO_PORTG_DATA_R ^= 0x04;
}*/

int main(void){ 
  System_Init();                   // initialize system
  //Timer0A_Init(&UserTask,50);      // initialize timer0A (20,000 Hz)
  Timer0_Init(&UserTask, 500);
  EnableInterrupts();
  while(1){

  }
}
