// Lab04 - EE445L Spring 2014
// LM3S1968
// Stepper motor controller using Finite State Machine
// Kevin Gilbert, Gilberto Rodriguez
// February 12, 2014

#include "lm3s1968.h"
#include "SysTick.h"
#include "../inc/hw_types.h"
#include "../inc/hw_gpio.h"
#include "../inc/hw_memmap.h"
#include "../driverlib/gpio.h"
#include "../driverlib/sysctl.h"
#include "../driverlib/interrupt.h"

//******************************************************************
/*	Finite State Machine Data Structure -> FSM
 *
 *
 *
 */

//******************************************************************
/*	System Initilizations 
 *		- Set Clock to 50Mhz with PLL
 *		- Initialize Heartbeat							 	
 *		- Initialize PortC for switch inputs
 *		- Initialize Systick
 */
// Function declaration
void system_Init(void);

void system_Init() {
  	// 50Mhz Clock
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
	// Port Inits
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	// Switch Inits
	GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_2);	 // Heartbeat
	GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5); // PC2-5 Input switches
  	// SysTick Init
	SysTick_Init();
}

int main(void){
	// Call system init function
	system_Init();
	//***************************************************************
	/*	Finite state machine
	 *	Pointer and data structure init
	 *	Updates occur in background
	 */

	while(1);
}
