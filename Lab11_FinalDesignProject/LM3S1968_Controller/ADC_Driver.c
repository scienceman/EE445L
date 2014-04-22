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

void ADCDualChannel_Init(unsigned long sysctl_base, unsigned long base, unsigned long sequence, unsigned long channel1, unsigned long channel2) {
	SysCtlPeripheralEnable(sysctl_base);
	//
	// Enable the first sample sequencer to capture the value of channel 1 and 3 when
	// the processor trigger occurs.
	//
	ADCSequenceConfigure(base, 0, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(base, sequence, 0, ADC_CTL_IE | channel1);
	ADCSequenceStepConfigure(base, sequence, 1, ADC_CTL_IE | ADC_CTL_END | channel2);

	ADCSequenceEnable(base, 0);
}

void ADCDualChannel_Read(unsigned long base, unsigned long sequence, unsigned long *adcBuff) {
		ADCProcessorTrigger(base, sequence);
		while(!ADCIntStatus(base, sequence, false));
		ADCSequenceDataGet(base, sequence, adcBuff);
}
