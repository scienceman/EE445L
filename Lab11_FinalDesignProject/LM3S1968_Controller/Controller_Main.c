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
#include "../driverlib/sysctl.h"
#include "../driverlib/pin_map.h"
#include "../driverlib/gpio.h"
#include "../driverlib/adc.h"

/********************************************************
 * Peripherial Drivers 
 ********************************************************/ 
#include "lm3s1968.h"
#include "Xbee.h"
#include "UART.h"
#include "system.h"
#include "InputCapture.h"
#include "timerCtrl.h"
#include "ADC_driver.h"
#include "SysTick.h"
#include "Sonar.h"

#include <stdio.h>

unsigned long adc[2];

int main(void) {
/**
*	XBee Frame variables
*/
	tXbee_frame frame;
	char cmd[20];
/**
* 	System Initialization
*/
	System_Init();
	SysTick_Init();
	UART1_Init();
	Xbee_Init();
	ADCDualChannel_Init(SYSCTL_PERIPH_ADC0, ADC0_BASE, 0, ADC_CTL_CH1, ADC_CTL_CH3);

	GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, 0x00);
	while(1) {
		ADCDualChannel_Read(ADC0_BASE, 0, &adc[0]);
		cmd[0] = '*';
		if(adc[1] < 350) {		// FORWARD
			cmd[1] = '+';
			cmd[2] = '1';	
		} else {
			if(adc[1] > 650) {
				cmd[1] = '-';
				cmd[2] = '1';
			} else {
			 	cmd[1] = '+';
				cmd[2] = '0';
			}
		}
		
		cmd[3] = ','; 
		if(adc[0] < 350) {		// LEFT
			cmd[4] = '+';
			cmd[5] = '1';		
		} else {
		 	if(adc[0] > 725) {
				cmd[4] = '-';
				cmd[5] = '1';
			} else {
			 	cmd[4] = '+';
				cmd[5] = '0';
			}
		}

		cmd[6] = ',';
		if(GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_0)) {
			// Autonomous mode
			cmd[7] = 'a';	// Autonomous
		} else {
		 	cmd[7] = 't';	// Tele-operated
		}
		cmd[8] = 0;
	 	frame = Xbee_CreateTxFrame(&cmd[0], 8);
		printf("cmd: %s\n",cmd);
		Xbee_SendTxFrame(&frame);								  
	}
}
