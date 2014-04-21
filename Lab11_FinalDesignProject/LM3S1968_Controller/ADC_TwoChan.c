// ADC_TwoChan.c
// Runs on LM3S1968
// Provide a function that initializes Timer0A to trigger ADC
// SS2 conversions and request an interrupt when the conversion
// is complete.  Sample Sequencer 2 can perform up to 4
// conversions, so it will be used to sample channels 1 and 3.
// Daniel Valvano
// September 20, 2012

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
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

#include "ADC_TwoChan.h"
#include "FIFO.h"

#define NVIC_EN0_INT16          0x00010000  // Interrupt 16 enable
#define NVIC_EN0_R              (*((volatile unsigned long *)0xE000E100))  // IRQ 0 to 31 Set Enable Register
#define NVIC_PRI4_R             (*((volatile unsigned long *)0xE000E410))  // IRQ 16 to 19 Priority Register
#define TIMER0_CFG_R            (*((volatile unsigned long *)0x40030000))
#define TIMER0_TAMR_R           (*((volatile unsigned long *)0x40030004))
#define TIMER0_CTL_R            (*((volatile unsigned long *)0x4003000C))
#define TIMER0_IMR_R            (*((volatile unsigned long *)0x40030018))
#define TIMER0_TAILR_R          (*((volatile unsigned long *)0x40030028))
#define TIMER0_TAPR_R           (*((volatile unsigned long *)0x40030038))
#define TIMER_CFG_16_BIT        0x00000004  // 16-bit timer configuration,
                                            // function is controlled by bits
                                            // 1:0 of GPTMTAMR and GPTMTBMR
#define TIMER_TAMR_TAMR_PERIOD  0x00000002  // Periodic Timer mode
#define TIMER_CTL_TAOTE         0x00000020  // GPTM TimerA Output Trigger
                                            // Enable
#define TIMER_CTL_TAEN          0x00000001  // GPTM TimerA Enable
#define TIMER_IMR_TATOIM        0x00000001  // GPTM TimerA Time-Out Interrupt
                                            // Mask
#define TIMER_TAILR_TAILRL_M    0x0000FFFF  // GPTM TimerA Interval Load
                                            // Register Low
#define ADC_ACTSS_R            (*((volatile unsigned long *)0x40038000))
#define ADC_RIS_R              (*((volatile unsigned long *)0x40038004))
#define ADC_IM_R               (*((volatile unsigned long *)0x40038008))
#define ADC_ISC_R              (*((volatile unsigned long *)0x4003800C))
#define ADC_EMUX_R             (*((volatile unsigned long *)0x40038014))
#define ADC_SSPRI_R            (*((volatile unsigned long *)0x40038020))
#define ADC_PSSI_R             (*((volatile unsigned long *)0x40038028))
#define ADC_SSMUX2_R           (*((volatile unsigned long *)0x40038080))
#define ADC_SSCTL2_R           (*((volatile unsigned long *)0x40038084))
#define ADC_SSFIFO2_R          (*((volatile unsigned long *)0x40038088))
#define ADC_ACTSS_ASEN2         0x00000004  // ADC SS2 Enable
#define ADC_RIS_INR2            0x00000004  // SS2 Raw Interrupt Status
#define ADC_IM_MASK2            0x00000004  // SS2 Interrupt Mask
#define ADC_ISC_IN2             0x00000004  // SS2 Interrupt Status and Clear
#define ADC_EMUX_EM2_M          0x00000F00  // SS2 Trigger Select
#define ADC_EMUX_EM2_TIMER      0x00000500  // Timer
#define ADC_SSPRI_SS3_4TH       0x00003000  // fourth priority
#define ADC_SSPRI_SS2_3RD       0x00000200  // third priority
#define ADC_SSPRI_SS1_2ND       0x00000010  // second priority
#define ADC_SSPRI_SS0_1ST       0x00000000  // first priority
#define ADC_PSSI_SS2            0x00000004  // SS2 Initiate
#define ADC_SSMUX2_MUX1_M       0x00000070  // 2nd Sample Input Select
#define ADC_SSMUX2_MUX0_M       0x00000007  // 1st Sample Input Select
#define ADC_SSMUX2_MUX1_S       4           // 2nd Sample Input Select lshift
#define ADC_SSMUX2_MUX0_S       0           // 1st Sample Input Select lshift
#define ADC_SSCTL2_TS1          0x00000080  // 2nd Sample Temp Sensor Select
#define ADC_SSCTL2_IE1          0x00000040  // 2nd Sample Interrupt Enable
#define ADC_SSCTL2_END1         0x00000020  // 2nd Sample is End of Sequence
#define ADC_SSCTL2_D1           0x00000010  // 2nd Sample Diff Input Select
#define ADC_SSCTL2_TS0          0x00000008  // 1st Sample Temp Sensor Select
#define ADC_SSCTL2_IE0          0x00000004  // 1st Sample Interrupt Enable
#define ADC_SSCTL2_END0         0x00000002  // 1st Sample is End of Sequence
#define ADC_SSCTL2_D0           0x00000001  // 1st Sample Diff Input Select
#define ADC_SSFIFO2_DATA_M      0x000003FF  // Conversion Result Data mask
#define SYSCTL_RCGC0_R          (*((volatile unsigned long *)0x400FE100))
#define SYSCTL_RCGC1_R          (*((volatile unsigned long *)0x400FE104))
#define SYSCTL_RCGC0_ADC        0x00010000  // ADC Clock Gating Control
#define SYSCTL_RCGC0_ADCSPD_M   0x00000300  // ADC Sample Speed mask
#define SYSCTL_RCGC0_ADCSPD125K 0x00000000  // 125K samples/second
#define SYSCTL_RCGC1_TIMER0     0x00010000  // timer 0 Clock Gating Control

#define ADC2CHSUCCESS 1 // represents success reading from FIFO
#define ADC2CHFAILED  0 // represents failed reading from FIFO

// FIFO macros
AddIndexFifo(Ch1,16,unsigned short,ADC2CHSUCCESS,ADC2CHFAILED)
// creates Ch1Fifo_Init() Ch1Fifo_Get() and Ch1Fifo_Put()
AddIndexFifo(Ch3,16,unsigned short,ADC2CHSUCCESS,ADC2CHFAILED)
// creates Ch3Fifo_Init() Ch3Fifo_Get() and Ch3Fifo_Put()

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
// the microcontroller.  Particularly, ADC sample sequencer 2
// is used here because it takes up to four samples, and only two
// samples are absolutely needed.  Sample sequencer 2 generates a
// raw interrupt when the second conversion is complete, and it
// is then promoted to an ADC controller interrupt.  Hardware
// Timer0A triggers the ADC conversion at the pre-programmed
// interval of 1,000 Hz, and software handles the interrupt to
// process the measurements when they are complete.
//
// A simpler approach would be to use software to trigger the
// ADC conversions, wait for them to complete, and then process
// the measurements.
//
// This initialization function sets up the ADC according to the
// following parameters.  Any parameters not explicitly listed
// below are not modified:
// Timer0A: enabled
// Mode: 16-bit, down counting
// One-shot or periodic: periodic
// Prescale value: 49
// Interval value: 999
// Sample time is busPeriod*(prescale+1)*(period+1)
//                0.00000002*50*1000 = 0.001 sec
// Max sample rate: <=125,000 samples/second
// Sequencer 0 priority: 1st (highest)
// Sequencer 1 priority: 2nd
// Sequencer 2 priority: 3rd
// Sequencer 3 priority: 4th (lowest)
// SS2 triggering event: Timer0A
// SS2 1st sample source: channel 1
// SS2 2nd sample source: channel 3
// SS2 control register: take 2 samples then stop and request interrupt
// SS2 interrupts: enabled and promoted to controller
void ADC_Init(void){  // assumes a 50 MHz bus clock
  volatile unsigned long delay;
  DisableInterrupts();
  Ch1Fifo_Init();
  Ch3Fifo_Init();
  SYSCTL_RCGC0_R |= SYSCTL_RCGC0_ADC;       // 1) activate ADC
  SYSCTL_RCGC0_R &= ~SYSCTL_RCGC0_ADCSPD_M; // 2) ADC sample speed 
  SYSCTL_RCGC0_R += SYSCTL_RCGC0_ADCSPD125K;//     for 125K ADC max rate 
  ADC_SSPRI_R = 0x3210;                    // 3) Priority of sequencers
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER0;    // 4) activate timer0
  delay = SYSCTL_RCGC1_R;                   //    allow time to finish 
  TIMER0_CTL_R &= ~TIMER_CTL_TAEN;          //    disable timer0A 
  TIMER0_CTL_R |= TIMER_CTL_TAOTE;          //    enable trigger to ADC
  TIMER0_CFG_R = TIMER_CFG_16_BIT;          //    16-bit timer mode
  TIMER0_TAMR_R = TIMER_TAMR_TAMR_PERIOD;   //    periodic mode
  TIMER0_TAPR_R = 49;                       //    prescale 1us
  TIMER0_TAILR_R = 999;                     //    reload value, 1ms
  TIMER0_IMR_R &= ~TIMER_IMR_TATOIM;        //    no rollover interrupt
  TIMER0_CTL_R |= TIMER_CTL_TAEN;           //    enable timer0A 
  ADC_ACTSS_R &= ~ADC_ACTSS_ASEN2;         // 5) disable sequencer 2
  ADC_EMUX_R &= ~ADC_EMUX_EM2_M;           // 6) SS2 trigger select 
  ADC_EMUX_R += ADC_EMUX_EM2_TIMER;        //    timer trigger event
  ADC_SSMUX2_R = 0x0031;                   // 7) which channels
  ADC_SSCTL2_R = 0x0060;                   // 8) IE1 and END1
  ADC_IM_R |= ADC_IM_MASK2;                // 9) enable SS2 interrupts
  ADC_ACTSS_R |= ADC_ACTSS_ASEN2;          // 10)enable sample seq 2
  NVIC_PRI4_R = (NVIC_PRI4_R&0xFFFFFF00)|0x00000040; // ADC2 priority 2
  NVIC_EN0_R |= NVIC_EN0_INT16;             // 12)enable interrupt 16 
  EnableInterrupts();                       // 13)enable all interrupts
}
void ADC2_Handler(void){
  ADC_ISC_R = ADC_ISC_IN2;      // acknowledge ADC sequence 2 completion
  Ch1Fifo_Put(ADC_SSFIFO2_R&ADC_SSFIFO2_DATA_M);  // Channel 1 first
  Ch3Fifo_Put(ADC_SSFIFO2_R&ADC_SSFIFO2_DATA_M);  // Channel 3 second
}

// Get the results of the oldest ADC conversion from the FIFOs.
// If the FIFOs are empty, this function will block (wait)
// until data comes in.  This assumes that ADC sample sequencer
// 2 is initialized and generating interrupts or ADC_Init() has
// already been called.  Otherwise, this function will crash.
// Input: ch1Ptr  pointer to location to store channel 1 value
//        ch3Ptr  pointer to location to store channel 3 value
// Output: none
void ADC_GetResultsBlocking(unsigned short *ch1Ptr, unsigned short *ch3Ptr){
  while(Ch1Fifo_Get(ch1Ptr) == ADC2CHFAILED){};
  while(Ch3Fifo_Get(ch3Ptr) == ADC2CHFAILED){};
}

// Get the results of the oldest ADC conversion from the FIFOs.
// If the FIFOs are empty, this function will immediately
// return 1024 in both values.
// Input: ch1Ptr  pointer to location to store channel 1 value
//        ch3Ptr  pointer to location to store channel 3 value
// Output: none
void ADC_GetResults(unsigned short *ch1Ptr, unsigned short *ch3Ptr){
  if(Ch3Fifo_Get(ch3Ptr) == ADC2CHFAILED){
    (*ch1Ptr) = 1024;
    (*ch3Ptr) = 1024;
  } else{
    Ch1Fifo_Get(ch1Ptr);
  }
}

// Return the number of ADC conversions in the FIFOs.
// Input: none
// Output: number of results currently in Ch3Fifo
unsigned short ADC_GetNumResults(void){
  return Ch3Fifo_Size();
}
