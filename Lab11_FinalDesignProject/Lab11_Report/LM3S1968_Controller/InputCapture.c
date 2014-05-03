// InputCapture.c
// Runs on LM3S1968
// Use Timer0A in edge time mode to request interrupts on the rising
// edge of PB0 (CCP0), and count the pulses.
// Daniel Valvano
// Oct 25, 2012

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2012
   Example 6.1, Program 6.1

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

// external signal connected to PB0 (CCP0) (trigger on rising edge)
#include "LM3S1968.h"
#include "../inc/hw_types.h"
#include "../driverlib/sysctl.h"
#include "../inc/hw_sysctl.h"
#include "Sonar.h"

#define TIMER_CFG_16_BIT        0x00000004  // 16-bit timer configuration,
                                            // function is controlled by bits
                                            // 1:0 of GPTMTAMR and GPTMTBMR
#define TIMER_TAMR_TACMR        0x00000004  // GPTM TimerA Capture Mode
#define TIMER_TAMR_TAMR_CAP     0x00000003  // Capture mode
#define TIMER_CTL_TAEN          0x00000001  // GPTM TimerA Enable
#define TIMER_CTL_TAEVENT_POS   0x00000000  // Positive edge
#define TIMER_IMR_CAEIM         0x00000004  // GPTM CaptureA Event Interrupt
                                            // Mask
#define TIMER_ICR_CAECINT       0x00000004  // GPTM CaptureA Event Interrupt
                                            // Clear
#define TIMER_TAILR_TAILRL_M    0x0000FFFF  // GPTM TimerA Interval Load
                                            // Register Low
void SysTick_IE_Init(unsigned long period);

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

extern tSonarModule sonar;
volatile unsigned long Count;      // incremented on interrupt
unsigned long Period = 0;
unsigned long First = 0;

void TimerCapture_Init(void){
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER0;// activate timer0
                                   // activate port B and port G
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB+SYSCTL_RCGC2_GPIOG;
  Count = 0;                       // allow time to finish activating
  GPIO_PORTG_DIR_R |= 0x04;        // make PG2 out (PG2 built-in LED)
  GPIO_PORTG_DEN_R |= 0x04;        // enable digital I/O on PG2
  GPIO_PORTB_DEN_R |= 0x01;        // enable digital I/O on PB0
  GPIO_PORTB_AFSEL_R |= 0x01;      // enable alt funct on PB0

  GPIO_PORTB_DEN_R |= 0x04;
  GPIO_PORTB_DIR_R |= 0x04;
  GPIO_PORTB_AFSEL_R &= ~0x04;

  TIMER0_CTL_R &= ~TIMER_CTL_TAEN; // disable timer0A during setup
  TIMER0_CFG_R = TIMER_CFG_16_BIT; // configure for 16-bit timer mode
                                   // configure for capture mode
  TIMER0_TAMR_R = (TIMER_TAMR_TACMR|TIMER_TAMR_TAMR_CAP);
                                   // configure for rising edge event
  TIMER0_CTL_R &= ~(TIMER_CTL_TAEVENT_POS|0xC);
  TIMER0_TAILR_R = 0x0000FFFF;     // start value
  TIMER0_IMR_R |= TIMER_IMR_CAEIM; // enable capture match interrupt
  TIMER0_ICR_R = TIMER_ICR_CAECINT;// clear timer0A capture match flag
  TIMER0_CTL_R |= TIMER_CTL_TAEN;  // enable timer0A 16-b, +edge timing, interrupts
                                   // Timer0A=priority 2
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x40000000; // top 3 bits
  NVIC_EN0_R |= NVIC_EN0_INT19;    // enable interrupt 19 in NVIC
  EnableInterrupts();
}
void Timer0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_CAECINT;// acknowledge timer0A capture match
  GPIO_PORTG_DATA_R = GPIO_PORTG_DATA_R^0x04; // toggle PG2
	sonar.echoTime = NVIC_ST_CURRENT_R;
	sonar.distance = (sonar.echoTime - sonar.triggerTime) /29 / 2; 
}

//void SysTick_IE_Init(unsigned long period){
//  int index;
//  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD; // activate port D
//  index = 0;
//  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
//  NVIC_ST_RELOAD_R = period-1;// reload value
//  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
//  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; // priority 2
//                              // enable SysTick with core clock and interrupts
//  NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE+NVIC_ST_CTRL_CLK_SRC+NVIC_ST_CTRL_INTEN;
//}
//
//void SysTick_Period(unsigned long period) {
//	NVIC_ST_RELOAD_R = period - 1;
//}

//void SysTick_Handler(void) {
//	//GPIO_PORTG_DATA_R = GPIO_PORTG_DATA_R^0x04; // toggle PG2
//	GPIO_PORTB_DATA_R |= 0x04;
//	SysCtlDelay(170);
//	GPIO_PORTB_DATA_R &= ~0x04;
//}

//debug code
//int main(void){
//  TimerCapture_Init(); // initialize timer0A in capture mode
//  SysTick_IE_Init(1000000);
//  while(1){
//    WaitForInterrupt();
//  }
//}
