// ADCTestmain.c
// Runs on LM3S811
// Provide a function that initializes Timer0A to trigger ADC
// SS3 conversions and request an interrupt when the conversion
// is complete.
// Daniel Valvano
// June 30, 2011

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to the Arm Cortex M3",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2011

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

// Thumbwheel potentiometer with scaling resistor connected to ADC0

#include "ADCT0ATrigger.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

#include "lm3s1968.h"

//debug code
//
// This program periodically samples ADC channel 0 and stores the
// result to a global variable that can be accessed with the JTAG
// debugger and viewed with the variable watch feature.
//#define GPIO_PORTC_DATA_R       (*((volatile unsigned long *)0x400063FC))
//#define GPIO_PORTC_DIR_R        (*((volatile unsigned long *)0x40006400))
//#define GPIO_PORTC_DEN_R        (*((volatile unsigned long *)0x4000651C))
//#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
//#define SYSCTL_RCGC2_GPIOC      0x00000004  // port C Clock Gating Control


int main(void){
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG;    // activate port C
  ADC_InitTimer0ATriggerSeq3(0, 9, 59999); // ADC channel 0, 10 Hz sampling
  GPIO_PORTG_DIR_R |= 0x04;                // make PC5 out (PC5 built-in LED)
  GPIO_PORTG_DEN_R |= 0x04;                // enable digital I/O on PC5 (default setting)
  while(1){
    WaitForInterrupt();
    GPIO_PORTG_DATA_R ^= 0x04;           // toggle LED
  }
}
