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
#include "../inc/hw_memmap.h"
#include "../driverlib/sysctl.h"
#include "../driverlib/interrupt.h"
#include "../inc/hw_timer.h"
#include "../driverlib/timer.h"
#include "rit128x96x4.h"
#include "../inc/hw_ints.h"
#include "Output.h"

#include "Output.h"
#include "system.h"
#include <stdio.h>

// Startup.s Function declaration
void DisableInterrupts(void);
void EnableInterrupts(void);
void StartCritical(void);
void EndCritical(void);
void WaitForInterrupt(void);


void System_Init() {
  	// 50Mhz Clock
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
	Switch_Init();
	Output_Init();
	Output_Color(15);
}

void Switch_Init(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 || GPIO_PIN_2 || GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_2);	 // Heartbeat
    GPIOPinTypeGPIOOutput(GPIO_PORTH_BASE, GPIO_PIN_0 || GPIO_PIN_1);
}

void motorIO_811_Init(void) {
	unsigned long period;
	SYSCTL_RCGC0_R |= SYSCTL_RCGC0_PWM;   // 1)activate PWM
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;   // GPIOG
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;
	period = SysCtlClockGet();
	GPIO_PORTB_AFSEL_R |= 0x03;        // Enable alt funct on PB0,1(PWM2,3)
	GPIO_PORTE_AFSEL_R |= 0x03;        // Enable alt funct on PE0,1(PWM4,5)
}

void motorIO_1968_Init(void) {
	SYSCTL_RCGC0_R |= SYSCTL_RCGC0_PWM;   // 1)activate PWM
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG;   // PG2->PWM0
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;	// PD1->PWM1
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;   // PF2,3->PWM4,5

	GPIO_PORTG_DEN_R |= 0x04;
	GPIO_PORTD_DEN_R |= 0x02;
	GPIO_PORTF_DEN_R |= 0x0C;

	GPIO_PORTG_AFSEL_R |= 0x04;        // Enable alt funct on PG2(PWM0)
	GPIO_PORTD_AFSEL_R |= 0x02;        // Enable alt funct on PD1(PWM1)
	GPIO_PORTF_AFSEL_R |= 0x0C;        // Enable alt funct on PF2,3(PWM4,5)
}




