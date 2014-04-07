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
#include "fixed.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

unsigned long LinRegADCtoTemp(unsigned long adc);
void LUTADCtoTemp(void);

#include "lm3s1968.h"
#include "Output.h"
#include "Timer0A.h"
#include "rit128x96x4.h"
#include "line.h"
#include <stdio.h>

void drawGraph_handler(void);

extern volatile unsigned long ADCvalue;

unsigned long const ADCdata[53]={0,29,40,52,64,76,89,102,115,128,142,
     156,170,185,200,215,230,246,262,279,296,
     313,330,348,366,385,404,423,443,463,484,
     505,526,548,570,592,615,638,662,686,711,
     736,761,787,814,840,868,895,923,952,981,1010,1024};

unsigned long const Tdata[53]={4000,4000,3960,3920,3880,3840,3800,3760,3720,3680,3640,
     3600,3560,3520,3480,3440,3400,3360,3320,3280,3240,
     3200,3160,3120,3080,3040,3000,2960,2920,2880,2840,
     2800,2760,2720,2680,2640,2600,2560,2520,2480,2440,
     2400,2360,2320,2280,2240,2200,2160,2120,2080,2040,2000,2000};

unsigned char graph[(128/2)*96/2] = {0};

void LUTADCtoTemp() {
	signed long i=0;	
	for(i=0;i<53;i++) {
	  if(ADCvalue < ADCdata[i]) {
	    break;
	  }
	}
	Fixed_uDecOut2(Tdata[i]);
}

unsigned long LinRegADCtoTemp(unsigned long adc) {
	unsigned long i=0;
	i=adc*(-2);   //Line of Best Fit: T(X)=-2.077+3929.95
	i+=3930;	
	Fixed_uDecOut2(i);
	return i;
}

  volatile int x=0;
  volatile int y=0;
  volatile unsigned long prevADC;

int main(void){
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG;    // activate port C
  ADC_InitTimer0ATriggerSeq3(0, 9, 59999); // ADC channel 0, 10 Hz sampling
  GPIO_PORTG_DIR_R |= 0x04;                // make PC5 out (PC5 built-in LED)
  GPIO_PORTG_DEN_R |= 0x04;                // enable digital I/O on PC5 (default setting)
  Output_Init();
  Output_Color(15);

  Timer0A_Init(drawGraph_handler, 59999);

  RIT128x96x4ImageDraw(graph,0,96/2,128,96/2);   
  while(1){
    WaitForInterrupt();
    GPIO_PORTG_DATA_R ^= 0x04;           // toggle LED
	LinRegADCtoTemp(ADCvalue);
  }
}

void drawGraph_handler(void) {
	static int delay=0;
	if(delay == 0) {
		RIT128x96x4_Line(x,(int)(LinRegADCtoTemp(prevADC)/100),++x,(int)(LinRegADCtoTemp(ADCvalue)/100),15);
		RIT128x96x4_ShowImageLines();		
	} 
	delay = (delay+1)%10;
	prevADC = ADCvalue;
}
