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
	SPI_Init();
	Output_Init();
	Output_Color(15);
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

int data = 0;

void SPI_Init(void) {
 	int delay;
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_SSI1;   // Activate SSI1
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTE_AFSEL_R |= 0x0B;  // PE0,1,3 Alt function = SSI1Clk, SS11FSS, SSI1Tx
	GPIO_PORTE_DEN_R |= 0x0B;
	SSI1_CR1_R &= ~SSI_CR1_SSE;  // Disable SSI for config
	SSI1_CR1_R &= ~SSI_CR1_MS;   // Master mode
	SSI1_CPSR_R = (SSI1_CPSR_R & ~SSI_CPSR_CPSDVSR_M)+2;   // SSI clock 3 Mhz  SSIClk = FSysClk / (CPSDVSR * (1 + SCR))
	SSI1_CR0_R &= ~(SSI_CR0_SCR_M | SSI_CR0_SPH | SSI_CR0_SPO);   // Clear SCR, SPH, SPO
	SSI1_CR0_R = (SSI1_CR0_R & ~SSI_CR0_FRF_M)+SSI_CR0_FRF_MOTO;  //freescale
	SSI1_CR0_R = (SSI1_CR0_R & ~SSI_CR0_DSS_M)+SSI_CR0_DSS_16;    // 16 bit data
	SSI1_CR0_R = data;
	SSI1_CR1_R |= SSI_CR1_SSE;    // Enable SSI
}
