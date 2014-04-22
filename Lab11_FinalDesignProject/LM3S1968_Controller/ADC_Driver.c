/*********************************************************
 * ADC Init and Control
 * EE445L - Lab11 Spring 2014
 * 21 April 2014
 * Kevin Gilbert, Gilberto Rodriguez
 *
 * LM3S1968 ADC interface
 ********************************************************/
#include "../inc/hw_types.h"
#include "../inc/hw_memmap.h"
#include "../driverlib/gpio.h"
#include "../driverlib/adc.h"
#include "../driverlib/sysctl.h"

#include "lm3s1968.h"
#include "ADC_Driver.h"

unsigned long ulValue;

void ADC_Init(unsigned long base, unsigned long channel) {
	//
	// Enable the first sample sequencer to capture the value of channel 0 when
	// the processor trigger occurs.
	//
	ADCSequenceConfigure(base, 0, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(base, 0, 0, ADC_CTL_IE | ADC_CTL_END | channel);
	ADCSequenceEnable(base, 0);
	//
	// Trigger the sample sequence.
	//
	ADCProcessorTrigger(base, 0);
	//
	// Wait until the sample sequence has completed.
	//
	while(!ADCIntStatus(base, 0, false))
	{
	}
	//
	// Read the value from the ADC.
	//
	ADCSequenceDataGet(base, 0, &ulValue);
}
