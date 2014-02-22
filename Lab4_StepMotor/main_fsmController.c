// Lab04 - EE445L Spring 2014
// LM3S1968
// Stepper motor controller using Finite State Machine
// Kevin Gilbert, Gilberto Rodriguez
// February 12, 2014

#include "lm3s1968.h"
#include "switch.h"
#include "../inc/hw_types.h"
#include "../inc/hw_gpio.h"
#include "../inc/hw_memmap.h"
#include "../driverlib/gpio.h"
#include "../driverlib/sysctl.h"
#include "../driverlib/interrupt.h"
#include <stdio.h>

#include "fsm.h"
#include "Output.h"
#include "Timer0A.h"

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

tState* state;
unsigned int nextState;
unsigned char input; 

void fsmDriver(void) {
    GPIO_PORTG_DATA_R ^= 0x04;
	state = getCurrentState();
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, state->Output);		
	input = GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4)&0x1C;
	input = input >> 2;
	setCurrentState(state->nextState[input]);
}

//#define DEBUG

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
	 Timer0A_Init(&fsmDriver,10000);  // period in usec. 100Hz (10000 usec)
	 state = getCurrentState();
	 EnableInterrupts();

	 while(1) {
	 #ifdef DEBUG
		printf("state: %s\r",(*state).name);
		printf("Output: %d\r",(*state).Output);
	 #endif
	 }
}
