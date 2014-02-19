// main_Lab5DAC.c
// Runs on LM3S1968
// EE445L - Spring 2014 Lab5
// DAC and SSI
// Kevin Gilbert, Gilberto Rodriguez
// February 19, 2014
#include "timer0A.h"
#include "system.h"
#include "lm3s1968.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

void UserTask(void){
  GPIO_PORTC_DATA_R ^= 0x20;
}

int main(void){ 
  System_Init();                   // initialize system
  Timer0A_Init(&UserTask,50);      // initialize timer0A (20,000 Hz)
  while(1){

  }
}
