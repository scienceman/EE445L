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
unsigned long LUTADCtoTemp(unsigned long adc);

#include "lm3s1968.h"
#include "Output.h"
#include "Timer0A.h"
#include "rit128x96x4.h"
#include "line.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void drawGraph_handler(void);

extern volatile unsigned long ADCvalue;

unsigned long const ADCdata[53]={0,19,32,46,60,74,89,103,118,133,148,
     164,179,195,211,227,243,260,277,294,311,
     329,347,365,383,401,420,439,458,478,498,
     518,538,559,579,601,622,644,666,688,711,
     734,757,781,804,829,853,878,903,929,955,981,1024};

unsigned long const Tdata[53]={4000,4000,3960,3920,3880,3840,3800,3760,3720,3680,3640,
     3600,3560,3520,3480,3440,3400,3360,3320,3280,3240,
     3200,3160,3120,3080,3040,3000,2960,2920,2880,2840,
     2800,2760,2720,2680,2640,2600,2560,2520,2480,2440,
     2400,2360,2320,2280,2240,2200,2160,2120,2080,2040,2000,2000};


unsigned char graph[(128/2)*96/2] = {0};
		 
unsigned long LUTADCtoTemp(unsigned long adc) {
	signed long i=0;
	char temperature[6];
	const char units[3] = " C";
	char tempOut[10];	
	for(i=0;i<53;i++) {
	  if(ADCvalue < ADCdata[i]) {
	    if(abs((ADCdata[i]-adc)) > abs((adc-ADCdata[i-1]))) {
			i--;	
		}
		break;
	  }
	}
	//Fixed_uDecOut2(Tdata[i]);
	Fixed_uDecOut2s(Tdata[i], temperature);
	sprintf(tempOut, "%s%s", temperature, units);
	RIT128x96x4StringDraw(tempOut, 40, 25, 15);
	return Tdata[i];
}

unsigned long LinRegADCtoTemp(unsigned long adc) {
	unsigned long i=0;
	char temperature[6];
	const char units[3] = " C";
	char tempOut[10];
	i=adc*(-208);   //Line of Best Fit: T(X)=-2.077+3929.95
	i+= 402995;	
	i /= 100;
	Fixed_uDecOut2s(i, temperature);
	sprintf(tempOut, "%s%s", temperature, units);
	RIT128x96x4StringDraw(tempOut, 40, 25, 15);
	return i;
}

unsigned long ExpRegADCtoTemp(unsigned long adc) {
 	unsigned long i=0;
	char temperature[6];
	const char units[3] = " C";
	char tempOut[10];
	// T(X) = 4049.9156 exp(-0.000715*X)
	i=4049.9156*exp(-0.000715*adc);
	Fixed_uDecOut2s(i, temperature);
	sprintf(tempOut, "%s%s", temperature, units);
	RIT128x96x4StringDraw(tempOut, 40, 25, 15);
	return i;
}

  volatile int x=0;
  volatile int y=30;
  volatile unsigned long prevADC;

int main(void){
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG;    // activate port C
  ADC_InitTimer0ATriggerSeq3(0, 9, 59999); // ADC channel 0, 10 Hz sampling
  GPIO_PORTG_DIR_R |= 0x04;                // make PC5 out (PC5 built-in LED)
  GPIO_PORTG_DEN_R |= 0x04;                // enable digital I/O on PC5 (default setting)
  Output_Init();
  Output_Color(15);

  RIT128x96x4_Line(0,5,0,45,15);
  RIT128x96x4_Line(0,45,120,45,15);
  
  Timer0A_Init(drawGraph_handler, 59999);

  RIT128x96x4ImageDraw(graph,0,96/2,128,96/2);   
  while(1){
    WaitForInterrupt();
    GPIO_PORTG_DATA_R ^= 0x04;           // toggle LED
	//LinRegADCtoTemp(ADCvalue);
	//LUTADCtoTemp(ADCvalue);
	ExpRegADCtoTemp(ADCvalue);
	RIT128x96x4_ShowImageLines();
  }
}

void drawGraph_handler(void) {
	static int delay=0;
	if(delay == 0) {
		//RIT128x96x4_Line(x,(int)(60-(LinRegADCtoTemp(prevADC)/100)),++x,(int)(60-(LinRegADCtoTemp(ADCvalue)/100)),15);
		//RIT128x96x4_Line(x,(int)(60-(LUTADCtoTemp(prevADC)/100)),++x,(int)(60-(LUTADCtoTemp(ADCvalue)/100)),15);
		RIT128x96x4_Line(x,(int)(60-(ExpRegADCtoTemp(prevADC)/100)),++x,(int)(60-(ExpRegADCtoTemp(ADCvalue)/100)),15);
		if(x>120) {
			RIT128x96x4_ClearImage();
		    RIT128x96x4_Line(0,5,0,45,15);
  			RIT128x96x4_Line(0,45,120,45,15);
			x=0;
		}		
	} 
	delay = (delay+1)%2;
	prevADC = ADCvalue;
}
