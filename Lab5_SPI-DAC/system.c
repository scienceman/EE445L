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

#include "system.h"
#include "music.h"
#include <stdio.h>

tBoolean pause;

void System_Init() {
  	// 50Mhz Clock
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
    Switch_Init();
	SSI_Init();
	Output_Init();
	Output_Color(15);
}

void Switch_Init(void){
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOC+SYSCTL_RCGC2_GPIOG; // activate ports C and G
	pause = 0;
	GPIO_PORTC_DIR_R &= ~0x1C; // make PC4 in
	GPIO_PORTC_DEN_R |= 0x1C; // enable I/O on PC4
	GPIO_PORTC_IS_R &= ~0x1C; // PC4 is edge-sensitive
	GPIO_PORTC_IBE_R &= ~0x1C; // PC4 is not both edges
	GPIO_PORTC_IEV_R |= 0x1C; // PC4 falling edge event
	GPIO_PORTC_ICR_R = 0x1C; // clear flag4
	GPIO_PORTC_IM_R |= 0x1C; // arm interrupt on PC4
    GPIO_PORTG_DIR_R |= 0x04;        // make PG2 out (PG2 built-in LED)
    GPIO_PORTG_DEN_R |= 0x04;        // enable digital I/O on PG2
	GPIO_PORTG_AFSEL_R &= ~0x04;
    GPIO_PORTG_DATA_R &= ~0x04;              // clear PG2		  */
	NVIC_PRI0_R = (NVIC_PRI0_R&0xFF00FFFF)|0x00A00000;
	// priority 5
	NVIC_EN0_R |= 4; // enable int 2 in NVIC 
	//GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_2);	 // Heartbeat
}

unsigned short receive;

void SSI_Init(void) {
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
	SSI1_DR_R = 0;
	SSI1_CR1_R |= SSI_CR1_SSE;    // Enable SSI
}


void DAC_Out(unsigned short code){
	while((SSI1_SR_R & SSI_SR_TNF) == 0){};
	SSI1_DR_R = code;
}

extern unsigned short Volume;
#define MAXVOL 9

void GPIOPortC_Handler(void) {
	GPIO_PORTC_ICR_R = 0x3C;
	    switch(GPIO_PORTC_DATA_R&0x3C){
        case(0x10): //(1) button (closest to board) Rewind
					Rewind();
          break;
				case(0x08): //(2) button Play/Pause
					if(pause){ Play(0); }
					else{ Stop(); }
					break;
				case(0x04):  //(3) button Mode (idk what it does yet) ABBBBBBBBBK(
					//TimerLoadSet(TIMER0_BASE, TIMER_A, A);
					Volume = (Volume + 1) % MAXVOL;
					printf("Volume: %02d\n",Volume);
					break;
				default:
					break;
    }
}
