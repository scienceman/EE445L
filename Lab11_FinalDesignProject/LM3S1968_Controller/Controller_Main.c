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
#define PART_LM3S1968
#define PART_IS_LM3S1968
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

#define CCP0_PERIPH		SYSCTL_PERIPH_GPIOB
#define CCP0_PORT   	GPIO_PORTB_BASE
#define CCP0_PIN        GPIO_PIN_0

#define CCP2_PERIPH     SYSCTL_PERIPH_GPIOB
#define CCP2_PORT       GPIO_PORTB_BASE
#define CCP2_PIN        GPIO_PIN_1

#define GPIO_PB1_CCP2   0x00010401

//#define ADCTEST
 
extern tBoolean newSonar;

unsigned long adc[2];
tSonarModule sonar;

int main(void) {
	int count=0;
	System_Init();
	SysTick_Init();
	UART1_Init();
	Xbee_Init();
	Timer1_CaptureInit();
	sonar = Sonar_Init(CCP2_PERIPH, CCP2_PORT, CCP2_PIN, SYSCTL_PERIPH_GPIOF, 
							GPIO_PORTF_BASE, GPIO_PIN_5, GPIO_PB1_CCP2);
	Timer0_Init(60000, 60000);
	//printf("ADC channels 1 and 3\r");
	ADCDualChannel_Init(SYSCTL_PERIPH_ADC0, ADC0_BASE, 0, ADC_CTL_CH1, ADC_CTL_CH3);
	GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, 0x00);
	while(1) {
	#ifdef ADCTEST
		ADCDualChannel_Read(ADC0_BASE, 0, &adc[0]);
		printf("1:%ld 3:%ld %d\n",adc[0],adc[1],count++);
	#else
		while(!newSonar); 
		if(sonar.distance < 1000) 
			printf("Sonar: %03ld cm\n",sonar.distance);
		newSonar = false;
	#endif									  
	}
}
