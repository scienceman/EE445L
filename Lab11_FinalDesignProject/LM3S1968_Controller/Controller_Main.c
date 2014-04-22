/*********************************************************
 * Remote Controller for 811 conrolled autonomous car
 * EE445L - Lab11 Spring 2014
 * 21 April 2014
 * Kevin Gilbert, Gilberto Rodriguez
 *
 * Receives	data through ADC from PSP controller. Relays
 *	data through XBee module to the car.
 ********************************************************/
/********************************************************
 * Stellarisware
 ********************************************************/ 
#include "../inc/hw_types.h"
#include "../inc/hw_memmap.h"
#include "../driverlib/gpio.h"
#include "../driverlib/sysctl.h"
#include "../driverlib/adc.h"

/********************************************************
 * Peripherial Drivers 
 ********************************************************/ 
#include "Xbee.h"
#include "UART.h"
#include "system.h"
#include "ADC_driver.h"

#include <stdio.h>

extern unsigned long ulValue;

unsigned long adc[2];

int main(void) {
	int count=0;
	System_Init();
	UART1_Init();
	Xbee_Init();

	printf("ADC channels 1 and 3\r");
	ADCDualChannel_Init(SYSCTL_PERIPH_ADC0, ADC0_BASE, 0, ADC_CTL_CH1, ADC_CTL_CH3);
	GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, 0x04);
	while(1) {
		ADCDualChannel_Read(ADC0_BASE, 0, &adc[0]);
		printf("1:%ld 3:%ld %d\n",adc[0],adc[1],count++);
	}
}
