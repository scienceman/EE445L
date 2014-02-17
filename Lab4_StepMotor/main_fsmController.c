// Lab04 - EE445L Spring 2014
// LM3S1968
// Stepper motor controller using Finite State Machine
// Kevin Gilbert, Gilberto Rodriguez
// February 12, 2014

#include "lm3s1968.h"
#include "SysTick.h"
#include "switch.h"
#include "../inc/hw_types.h"
#include "../inc/hw_gpio.h"
#include "../inc/hw_memmap.h"
#include "../driverlib/gpio.h"
#include "../driverlib/sysctl.h"
#include "../driverlib/interrupt.h"

#include "fsm.h"
#include "Output.h"
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
    Switch_Init();
	Output_Init();
	Output_Color(15);
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
	 SysTick_IE_Init(500000);
	 EnableInterrupts();

	 while(1);
}
