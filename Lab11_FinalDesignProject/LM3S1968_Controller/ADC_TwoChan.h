// ADC_TwoChan.h
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
void ADC_Init(void);

// Get the results of the oldest ADC conversion from the FIFOs.
// If the FIFOs are empty, this function will block (wait)
// until data comes in.  This assumes that ADC sample sequencer
// 2 is initialized and generating interrupts or ADC_Init() has
// already been called.  Otherwise, this function will crash.
// Input: ch1Ptr  pointer to location to store channel 1 value
//        ch3Ptr  pointer to location to store channel 3 value
// Output: none
void ADC_GetResultsBlocking(unsigned short *ch1Ptr, unsigned short *ch3Ptr);

// Get the results of the oldest ADC conversion from the FIFOs.
// If the FIFOs are empty, this function will immediately
// return 1024 in both values.
// Input: ch1Ptr  pointer to location to store channel 1 value
//        ch3Ptr  pointer to location to store channel 3 value
// Output: none
void ADC_GetResults(unsigned short *ch1Ptr, unsigned short *ch3Ptr);

// Return the number of ADC conversions in the FIFOs.
// Input: none
// Output: number of results currently in Ch3Fifo
unsigned short ADC_GetNumResults(void);
