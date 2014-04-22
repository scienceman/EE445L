// ADC_Testmain.c
// Runs on LM3S1968
// Provide a function that initializes Timer0A to trigger ADC
// SS2 conversions and request an interrupt when the conversion
// is complete.  Sample Sequencer 2 can perform up to 4
// conversions, so it will be used to sample channels 1 and 3.
// Daniel Valvano
// September 20, 2012

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M3 Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2012
   Example 8.4, Program 8.9

 Copyright 2012 by Jonathan W. Valvano, valvano@mail.utexas.edu
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

// distance measuring slide potentiometer with scaling resistor connected to ADC1
// distance measuring slide potentiometer with scaling resistor connected to ADC3

#include "PLL.h"
#include "ADC_TwoChan.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

//debug code
//
// This program periodically samples ADC channels 1 and 3 and stores
// the result to a global variable that can be accessed with the JTAG
// debugger and viewed with the variable watch feature.
#define PG2                     (*((volatile unsigned long *)0x40026010))
#define GPIO_PORTG_DIR_R        (*((volatile unsigned long *)0x40026400))
#define GPIO_PORTG_DEN_R        (*((volatile unsigned long *)0x4002651C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOG      0x00000040  // port G Clock Gating Control

unsigned short Value1;
unsigned short Value3;
unsigned short Size;
int mainADCTest(void){
  PLL_Init();                              // set system clock to 50 MHz
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG;    // activate port G
  ADC_Init();                              // ADC channels 1 and 3, 1,000 Hz sampling
  GPIO_PORTG_DIR_R |= 0x04;                // make PG2 out (PG2 built-in LED)
  GPIO_PORTG_DEN_R |= 0x04;                // enable digital I/O on PG2
  while(1){
    PG2 ^= 0x04;                           // toggle LED
    ADC_GetResultsBlocking(&Value1, &Value3);
    Size = ADC_GetNumResults();
  }
}
