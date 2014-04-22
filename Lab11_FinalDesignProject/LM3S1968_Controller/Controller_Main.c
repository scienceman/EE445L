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

int main(void) {
	System_Init();
	UART1_Init();
	Xbee_Init();

	GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, 0x04);
	printf("sciene\r");
	ADC_Init(ADC0_BASE, ADC_CTL_CH0);
	printf("%ld\r",ulValue);
	while(1);
}
