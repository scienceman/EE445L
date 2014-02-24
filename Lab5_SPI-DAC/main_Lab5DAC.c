// main_Lab5DAC.c
// Runs on LM3S1968
// EE445L - Spring 2014 Lab5
// DAC and SSI
// Kevin Gilbert, Gilberto Rodriguez
// February 19, 2014
 #include "system.h"
 #include "music.h"
 #include "lm3s1968.h"
 #include "TimerCtrl.h"
 #include "../inc/hw_types.h"
 #include "../driverlib/interrupt.h"
 #include "../inc/hw_ints.h"
 #include "../driverlib/timer.h"
 #include "../driverlib/sysctl.h"
 #include "../inc/hw_memmap.h"
 #include "../driverlib/debug.h"
 #include "../driverlib/gpio.h"
 #include "SysTickInts.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

int main(void){ 
  System_Init();                   // initialize system
  Timer0_Init(D,65000);  // A = 440Hz = 2273 uSec period
  SysTick_IE_Init(D);
  EnableInterrupts();
  while(1);
}
