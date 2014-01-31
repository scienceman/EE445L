// Performance.c
// Runs on LM3S1968
// Use the SysTick timer to measure approximately how long it takes to
// calculate a square root.  Also, toggle PG2 to use an oscilloscope
// or logic analyzer to measure the execution time as the high pulse
// time of the LED.
// Daniel Valvano
// March 6, 2012

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to the Arm Cortex M3",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2011
   Program 3.17

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

// oscilloscope connected to PG2

#include <math.h>
#include "SysTick.h"
#include "hw_types.h"
#include "sysctl.h"
#include "lm3s1968.h"
#define GPIO_PORTG2             (*((volatile unsigned long *)0x40026010))
#define NVIC_ST_CURRENT_R       (*((volatile unsigned long *)0xE000E018))

// Newton's method
// s is an integer
// sqrt(s) is an integer
/*unsigned long sqrt(unsigned long s){
unsigned long t;         // t*t will become s
int n;                   // loop counter to make sure it stops running
  t = s/16+1;            // initial guess 
  for(n = 16; n; --n){   // guaranteed to finish
    t = ((t*t+s)/t)/2;  
  }
  return t; 
}*/

volatile unsigned long before, elapsed, ss, tt;
int main(void){ volatile unsigned long delay;
  SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                 SYSCTL_XTAL_8MHZ);     // 50 MHz
  SysTick_Init(); // initialize SysTick timer, see SysTick.c
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG; // activate clock for Port G
  delay = SYSCTL_RCGC2_R;               // allow time to finish activating
  GPIO_PORTG_DIR_R |= 0x04;             // make PG2 output (PG2 built-in LED)
  GPIO_PORTG_AFSEL_R &= ~0x04;          // disable alt funct on PG2 (default setting)
  GPIO_PORTG_DEN_R |= 0x04;             // enable digital I/O on PG2 (default setting on LM3S811, not default on other microcontrollers)
  ss = 100;
  before = NVIC_ST_CURRENT_R;
  tt = sqrt(ss);
  elapsed = (before-NVIC_ST_CURRENT_R)&0x00FFFFFF;
  while(1){
    GPIO_PORTG2 = 0x04;                 // turn on LED
    tt = sqrt(ss);
    GPIO_PORTG2 = 0x00;                 // turn off LED
    // anything can go here, after the LED goes off before the repeat
    // use the oscilloscope/logic analyzer to measure high pulse time
    // this can be easier to see if there is also some low pulse time
    tt = sqrt(ss);
  }
}
