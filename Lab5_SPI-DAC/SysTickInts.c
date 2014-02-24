// SysTickInts.c
// Runs on LM3S1968
// Use the SysTick timer to request interrupts at a particular period.
// Daniel Valvano
// June 27, 2011

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to the Arm Cortex M3",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2011

   Program 5.12, section 5.7

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

// oscilloscope or LED connected to PD0 for period measurement
#include "inc/hw_types.h"
#include "DriverLib/sysctl.h"
#include "lm3s1968.h"
#include "SysTickInts.h"
#include "music.h"
#include "TimerCtrl.h"
#include "system.h"

#define NVIC_SYS_PRI3_R         (*((volatile unsigned long *)0xE000ED20))  // Sys. Handlers 12 to 15 Priority
#define NVIC_ST_CTRL_R          (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile unsigned long *)0xE000E018))
#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode
#define GPIO_PORTD_DIR_R        (*((volatile unsigned long *)0x40007400))
#define GPIO_PORTD_DEN_R        (*((volatile unsigned long *)0x4000751C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOD      0x00000008  // port D Clock Gating Control

void SysTick_Period(unsigned long period);   // Reset reload value
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
extern int counter, set_time, main_menu, set_alarm, displayClock, alarm;
unsigned char index;
unsigned int count=0, hours=0, minutes=0;

// **************SysTick_Init*********************
// Initialize Systick periodic interrupts
// Input: interrupt period
//        Units of period are 20ns
//        Maximum is 2^24-1
//        Minimum is determined by length of ISR
// Output: none
void SysTick_IE_Init(unsigned long period){
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD; // activate port D
  index = 0;
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1;// reload value
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; // priority 2
                              // enable SysTick with core clock and interrupts
  NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE+NVIC_ST_CTRL_CLK_SRC+NVIC_ST_CTRL_INTEN;
}

void SysTick_Period(unsigned long period) {
	NVIC_ST_RELOAD_R = period - 1;
}

// Interrupt service routine
// Executed every 20ns*(period)

//extern unsigned short DACout;
unsigned short wave2;
extern unsigned short Volume;
extern const unsigned short Wave[32];
extern unsigned int changeNote;
extern tNote mario[MARIOLEN];
extern tNote mario2[MARIOLEN2];
unsigned int I2=0;
int dur2 = 1;
int noteIndex2=0;


void SysTick_Handler(void){
   	long critSection;
	if(mario2[noteIndex2].frequency) {
		critSection = StartCritical();
		//DACout = DACout - Wave[(I2)] + Wave[(I2+1)%32]; // Remove old wave component, update new
		wave2 = Wave[I2];
		EndCritical(critSection);
		//DAC_Out(DACout*Volume);
	}
	I2 = (I2+1)%32; // 0 to 31
	 
	if(changeNote) {
		if(mario2[noteIndex2].frequency == 0) {
			I2--;
		} else {
		 	 SysTick_Period(mario2[noteIndex2].frequency);
		}
		if(dur2 > mario2[noteIndex2].duration) {
			noteIndex2 = (noteIndex2 + 1) % MARIOLEN2;
			dur2=1;
		} else { dur2++; }
		changeNote = 0;
	}
}

