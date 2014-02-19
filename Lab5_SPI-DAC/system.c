/******************************************************************
 *	EE445L - Spring 2014
 *	LM3S1968 System Functions
 *
 *	Kevin Gilbert, Gilberto Rodriguez
 *	19 February 2014
 ******************************************************************/
#include "lm3s1968.h"
#include "../inc/hw_types.h"
#include "../driverlib/gpio.h"
#include "../inc/hw_gpio.h"
#include "../driverlib/interrupt.h"
#include "../inc/hw_memmap.h"
#include "../driverlib/sysctl.h"
#include "../driverlib/interrupt.h"
#include "rit128x96x4.h"
#include "Output.h"
#include <stdio.h>

#include "system.h"

void System_Init() {
  	// 50Mhz Clock
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
    Switch_Init();
	//Output_Init();
	//Output_Color(15);
}

void Switch_Init(void){
    // Port Inits
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    
	// Switch Inits
	GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_2);	 // Heartbeat
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);  // PA0-3 stepper output
	GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4); // PC2-4 Input switches
}
