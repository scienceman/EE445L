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

#include "fsm.h"
#include "SysTickInts.h"

//******************************************************************
/*	System Initilizations 
 *		- Set Clock to 50Mhz with PLL
 *		- Initialize Heartbeat							 	
 *		- Initialize PortC for switch inputs
 *		- Initialize Systick
 */
// Function declaration
void system_Init(void);
void DisableInterrupts(void);
void EnableInterrupts(void);
void StartCritical(void);
void EndCritical(void);
void WaitForInterrupt(void);

void system_Init() {
  	// 50Mhz Clock
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
	// Port Inits
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	// Switch Inits
	GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_2);	 // Heartbeat
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);  // PA0-3 stepper output
	GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4); // PC2-4 Input switches
}

int main(void){
	// Call system init function
	system_Init();
	//***************************************************************
	/*	Finite state machine
	 *	Pointer and data structure init
	 *	Updates occur in background
	 */
	 // Initialize states
	 states_init();
	 // Initialize FSM driver
	 SysTick_IE_Init(50000);
	 EnableInterrupts();

	 while(1);
}
