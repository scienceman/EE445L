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
#include "line.h"

#include <stdio.h>

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

unsigned long adc[2];
extern int autonomous_flag;

int main(void) {
/**
*	XBee Frame variables
*/
	tXbee_frame frame;
	tXbee_frame fb_frame;
	char cmd[20];
	int left_sonar, right_sonar;
/**
* 	System Initialization
*/
	System_Init();
	SysTick_Init();
	printf("Entering UART\r");
	UART1_Init();
	printf("Exiting UART\r");
	Xbee_Init();
	ADCDualChannel_Init(SYSCTL_PERIPH_ADC0, ADC0_BASE, 0, ADC_CTL_CH1, ADC_CTL_CH3);
	printf("Xbee/ADC Init Complete\r");
	EnableInterrupts();

	GPIOPinWrite(GPIO_PORTG_BASE, GPIO_PIN_2, 0x00);
	while(1) {
		ADCDualChannel_Read(ADC0_BASE, 0, &adc[0]);
		cmd[0] = '*';
		if(adc[0] < 350) {		// FORWARD
			cmd[1] = '-';
			cmd[2] = '1';	
		} else {
			if(adc[0] > 650) {
				cmd[1] = '+';
				cmd[2] = '1';
			} else {
			 	cmd[1] = '+';
				cmd[2] = '0';
			}
		}
		
		cmd[3] = ','; 
		if(adc[1] < 350) {		// LEFT
			cmd[4] = '-';
			cmd[5] = '1';		
		} else {
		 	if(adc[1] > 725) {
				cmd[4] = '+';
				cmd[5] = '1';
			} else {
			 	cmd[4] = '+';
				cmd[5] = '0';
			}
		}

		cmd[6] = ',';
		if(autonomous_flag) {
			cmd[7] = 'a';	// Autonomous
		} else {
		 	cmd[7] = 't';	// Tele-operated
		}
		cmd[8] = 0;
	 	frame = Xbee_CreateTxFrame(&cmd[0], 8);
		RIT128x96x4StringDraw(cmd,45,5,15);	 
		Xbee_SendTxFrame(&frame);
		
		fb_frame = Xbee_ReceiveRxFrame();
		if(fb_frame.message[0] == '*') {
			fb_frame.message[fb_frame.length-5]=0;
			if(fb_frame.message[1] != 'X' && fb_frame.message[5] != 'X') { 
				RIT128x96x4StringDraw(fb_frame.message,45,20,15);
				left_sonar = (fb_frame.message[1]-0x30)*100;
				left_sonar += ((fb_frame.message[2]-0x30)*10);
				left_sonar += (fb_frame.message[3]-0x30);
							   
				right_sonar = (fb_frame.message[5]-0x30)*100;
				right_sonar += ((fb_frame.message[6]-0x30)*10);
				right_sonar += (fb_frame.message[7]-0x30);
				RIT128x96x4_ClearImage();
				RIT128x96x4_Line(85,48-(left_sonar/3),100,48-(left_sonar/3),15);
				RIT128x96x4_Line(110,48-(right_sonar/3),127,48-(right_sonar/3),15);
				// Cut off Range
				RIT128x96x4_Line(80,48-(40/3),127,48-(40/3),10);	
				RIT128x96x4_ShowImageLines();
			}	  
		}							  
	}
}
