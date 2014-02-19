// PeriodicTimer0AInts.c
// Runs on LM3S1968
// Use Timer0A in periodic mode to request interrupts at a particular
// period.
// Daniel Valvano
// September 14, 2011

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to the Arm Cortex M3",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2011
  Program 7.5, example 7.6

 Copyright 2011 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// oscilloscope connected to PC5 for period measurement
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "timer0A.h"
#include "system.h"

#define GPIO_PORTC_DATA_R       (*((volatile unsigned long *)0x400063FC))
#define GPIO_PORTC_DIR_R        (*((volatile unsigned long *)0x40006400))
#define GPIO_PORTC_DEN_R        (*((volatile unsigned long *)0x4000651C))
#define GPIO_PORTC4             (*((volatile unsigned long *)0x40006040))
#define GPIO_PORTC5             (*((volatile unsigned long *)0x40006080))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOC      0x00000004  // port C Clock Gating Control

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

void UserTask(void){
  GPIO_PORTC5 ^= 0x20;
}

//debug code
int main(void){ volatile unsigned long delay;
  SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                 SYSCTL_XTAL_8MHZ);
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOC;  // activate port C
  delay = SYSCTL_RCGC2_R;          // allow time to finish activating
  GPIO_PORTC_DIR_R |= 0x30;        // make PC5 PC4 output profiling
  GPIO_PORTC_DEN_R |= 0x30;        // enable digital I/O on PC4 PC5
  Timer0A_Init(&UserTask,50);      // initialize timer0A (20,000 Hz)
  while(1){
    GPIO_PORTC4 ^= 0x10;
  }
}
