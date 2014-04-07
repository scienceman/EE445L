// ADCT0ATrigger.c
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

//#define NVIC_EN0_INT17          0x00020000  // Interrupt 17 enable
//#define NVIC_EN0_R              (*((volatile unsigned long *)0xE000E100))  // IRQ 0 to 31 Set Enable Register
//#define NVIC_PRI4_R             (*((volatile unsigned long *)0xE000E410))  // IRQ 16 to 19 Priority Register
//#define TIMER0_CFG_R            (*((volatile unsigned long *)0x40030000))
//#define TIMER0_TAMR_R           (*((volatile unsigned long *)0x40030004))
//#define TIMER0_CTL_R            (*((volatile unsigned long *)0x4003000C))
//#define TIMER0_IMR_R            (*((volatile unsigned long *)0x40030018))
//#define TIMER0_TAILR_R          (*((volatile unsigned long *)0x40030028))
//#define TIMER0_TAPR_R           (*((volatile unsigned long *)0x40030038))
//#define TIMER_CFG_16_BIT        0x00000004  // 16-bit timer configuration,
//                                            // function is controlled by bits
//                                            // 1:0 of GPTMTAMR and GPTMTBMR
//#define TIMER_TAMR_TAMR_PERIOD  0x00000002  // Periodic Timer mode
//#define TIMER_CTL_TAOTE         0x00000020  // GPTM TimerA Output Trigger
//                                            // Enable
//#define TIMER_CTL_TAEN          0x00000001  // GPTM TimerA Enable
//#define TIMER_IMR_TATOIM        0x00000001  // GPTM TimerA Time-Out Interrupt
//                                            // Mask
//#define TIMER_TAILR_TAILRL_M    0x0000FFFF  // GPTM TimerA Interval Load
//                                            // Register Low
//#define ADC_ACTSS_R             (*((volatile unsigned long *)0x40038000))
//#define ADC0_RIS_R              (*((volatile unsigned long *)0x40038004))
//#define ADC0_IM_R               (*((volatile unsigned long *)0x40038008))
//#define ADC0_ISC_R              (*((volatile unsigned long *)0x4003800C))
//#define ADC0_EMUX_R             (*((volatile unsigned long *)0x40038014))
//#define ADC0_SSPRI_R            (*((volatile unsigned long *)0x40038020))
//#define ADC0_PSSI_R             (*((volatile unsigned long *)0x40038028))
//#define ADC0_SSMUX3_R           (*((volatile unsigned long *)0x400380A0))
//#define ADC0_SSCTL3_R           (*((volatile unsigned long *)0x400380A4))
//#define ADC0_SSFIFO3_R          (*((volatile unsigned long *)0x400380A8))
//#define ADC_ACTSS_ASEN3         0x00000008  // ADC SS3 Enable
//#define ADC_RIS_INR3            0x00000008  // SS3 Raw Interrupt Status
//#define ADC_IM_MASK3            0x00000008  // SS3 Interrupt Mask
//#define ADC_ISC_IN3             0x00000008  // SS3 Interrupt Status and Clear
//#define ADC_EMUX_EM3_M          0x0000F000  // SS3 Trigger Select mask
//#define ADC_EMUX_EM3_TIMER      0x00005000  // Timer
//#define ADC_SSPRI_SS3_4TH       0x00003000  // fourth priority
//#define ADC_SSPRI_SS2_3RD       0x00000200  // third priority
//#define ADC_SSPRI_SS1_2ND       0x00000010  // second priority
//#define ADC_SSPRI_SS0_1ST       0x00000000  // first priority
//#define ADC_PSSI_SS3            0x00000008  // SS3 Initiate
//#define ADC_SSMUX3_MUX0_M       0x00000003  // 1st Sample Input Select mask
//#define ADC_SSMUX3_MUX0_S       0           // 1st Sample Input Select lshift
//#define ADC_SSCTL3_TS0          0x00000008  // 1st Sample Temp Sensor Select
//#define ADC_SSCTL3_IE0          0x00000004  // 1st Sample Interrupt Enable
//#define ADC_SSCTL3_END0         0x00000002  // 1st Sample is End of Sequence
//#define ADC_SSCTL3_D0           0x00000001  // 1st Sample Diff Input Select
//#define ADC_SSFIFO3_DATA_M      0x000003FF  // Conversion Result Data mask
//#define SYSCTL_RCGC0_R          (*((volatile unsigned long *)0x400FE100))
//#define SYSCTL_RCGC1_R          (*((volatile unsigned long *)0x400FE104))
//#define SYSCTL_RCGC0_ADC        0x00010000  // ADC0 Clock Gating Control
//#define SYSCTL_RCGC0_ADCSPD_M   0x00000300  // ADC Sample Speed mask
//#define SYSCTL_RCGC0_ADCSPD125K 0x00000000  // 125K samples/second
//#define SYSCTL_RCGC1_TIMER0     0x00010000  // timer 0 Clock Gating Control

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

// There are many choices to make when using the ADC, and many
// different combinations of settings will all do basically the
// same thing.  For simplicity, this function makes some choices
// for you.  When calling this function, be sure that it does
// not conflict with any other software that may be running on
// the microcontroller.  Particularly, ADC sample sequencer 3
// is used here because it only takes one sample, and only one
// sample is absolutely needed.  Sample sequencer 3 generates a
// raw interrupt when the conversion is complete, and it is then
// promoted to an ADC controller interrupt.  Hardware Timer0A
// triggers the ADC conversion at the programmed interval, and
// software handles the interrupt to process the measurement
// when it is complete.
//
// A simpler approach would be to use software to trigger the
// ADC conversion, wait for it to complete, and then process the
// measurement.
//
// This initialization function sets up the ADC according to the
// following parameters.  Any parameters not explicitly listed
// below are not modified:
// Timer0A: enabled
// Mode: 16-bit, down counting
// One-shot or periodic: periodic
// Prescale value: programmable using variable 'prescale' [0:255]
// Interval value: programmable using variable 'period' [0:65535]
// Sample time is busPeriod*(prescale+1)*(period+1)
// Max sample rate: <=125,000 samples/second
// Sequencer 0 priority: 1st (highest)
// Sequencer 1 priority: 2nd
// Sequencer 2 priority: 3rd
// Sequencer 3 priority: 4th (lowest)
// SS3 triggering event: Timer0A
// SS3 1st sample source: programmable using variable 'channelNum' [0:3]
// SS3 interrupts: enabled and promoted to controller

#include "lm3s1968.h"

void ADC_InitTimer0ATriggerSeq3(unsigned char channelNum, unsigned char prescale, unsigned short period){
  volatile unsigned long delay;
  // channelNum must be 0-3 (inclusive) corresponding to ADC0 through ADC3
  if(channelNum > 3){
    return;                                 // invalid input, do nothing
  }
  DisableInterrupts();
  // **** general initialization ****
  SYSCTL_RCGC0_R |= SYSCTL_RCGC0_ADC;       // activate ADC
  SYSCTL_RCGC0_R &= ~SYSCTL_RCGC0_ADCSPD_M; // clear ADC sample speed field
  SYSCTL_RCGC0_R += SYSCTL_RCGC0_ADCSPD125K;// configure for 125K ADC max sample rate (default setting)
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER0;    // activate timer0
  delay = SYSCTL_RCGC1_R;                   // allow time to finish activating
  TIMER0_CTL_R &= ~TIMER_CTL_TAEN;          // disable timer0A during setup
  TIMER0_CTL_R |= TIMER_CTL_TAOTE;          // enable timer0A trigger to ADC
  TIMER0_CFG_R = TIMER_CFG_16_BIT;          // configure for 16-bit timer mode
  // **** timer0A initialization ****
  TIMER0_TAMR_R = TIMER_TAMR_TAMR_PERIOD;   // configure for periodic mode
  TIMER0_TAPR_R = prescale;                 // prescale value for trigger
  TIMER0_TAILR_R = period;                  // start value for trigger
  TIMER0_IMR_R &= ~TIMER_IMR_TATOIM;        // disable timeout (rollover) interrupt
  TIMER0_CTL_R |= TIMER_CTL_TAEN;           // enable timer0A 16-b, periodic, no interrupts
  // **** ADC initialization ****
                                            // sequencer 0 is highest priority (default setting)
                                            // sequencer 1 is second-highest priority (default setting)
                                            // sequencer 2 is third-highest priority (default setting)
                                            // sequencer 3 is lowest priority (default setting)
  ADC0_SSPRI_R = (ADC_SSPRI_SS0_1ST|ADC_SSPRI_SS1_2ND|ADC_SSPRI_SS2_3RD|ADC_SSPRI_SS3_4TH);
  ADC_ACTSS_R &= ~ADC_ACTSS_ASEN3;          // disable sample sequencer 3
  ADC0_EMUX_R &= ~ADC_EMUX_EM3_M;           // clear SS3 trigger select field
  ADC0_EMUX_R += ADC_EMUX_EM3_TIMER;        // configure for timer trigger event
  ADC0_SSMUX3_R &= ~ADC_SSMUX3_MUX0_M;      // clear SS3 1st sample input select field
                                            // configure for 'channelNum' as first sample input
  ADC0_SSMUX3_R += (channelNum<<ADC_SSMUX3_MUX0_S);
  ADC0_SSCTL3_R = (0                        // settings for 1st sample:
                   & ~ADC_SSCTL3_TS0        // read pin specified by ADC0_SSMUX3_R (default setting)
                   | ADC_SSCTL3_IE0         // raw interrupt asserted here
                   | ADC_SSCTL3_END0        // sample is end of sequence (default setting, hardwired)
                   & ~ADC_SSCTL3_D0);       // differential mode not used (default setting)
  ADC0_IM_R |= ADC_IM_MASK3;                // enable SS3 interrupts
  ADC_ACTSS_R |= ADC_ACTSS_ASEN3;           // enable sample sequencer 3
  // **** interrupt initialization ****
                                            // ADC3=priority 2
  NVIC_PRI4_R = (NVIC_PRI4_R&0xFFFF00FF)|0x00004000; // bits 13-15
  NVIC_EN0_R |= NVIC_EN0_INT17;             // enable interrupt 17 in NVIC
  EnableInterrupts();
}



volatile unsigned long ADCvalue;
void ADC3_Handler(void){
  ADC0_ISC_R = ADC_ISC_IN3;             // acknowledge ADC sequence 3 completion
  ADCvalue = ADC0_SSFIFO3_R&ADC_SSFIFO3_DATA_M;
}
