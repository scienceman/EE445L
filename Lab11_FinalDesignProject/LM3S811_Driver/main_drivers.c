/*********************************************************
 * LM3S811 conrolled autonomous/Teleoperated car
 * EE445L - Lab11 Spring 2014
 * 21 April 2014
 * Kevin Gilbert, Gilberto Rodriguez
 *
 * Receives	data through XBee from LM3S1968 controller. Can be
 *	switched to run in autonmous obstacle avoidance mode
 ********************************************************/
/********************************************************
 * Stellarisware
 ********************************************************/ 
#include "../inc/hw_types.h"
#include "../driverlib/pin_map.h"
#include "../driverlib/gpio.h"
#include "../inc/hw_gpio.h"
#include "../driverlib/interrupt.h"
#include "../inc/hw_memmap.h"
#include "../driverlib/sysctl.h"
#include "../driverlib/interrupt.h"
#include "../inc/hw_pwm.h"
#include "../driverlib/pwm.h"
#include <stdio.h>
/********************************************************
 * Peripherial Drivers 
 ********************************************************/ 
#include "motor_driver.h"
#include "sonar.h"
#include "accel.h"
#include "Xbee.h"
#include "system.h"
#include "UART.h"
#include <string.h>

#include "lm3s811.h"

//#define MOTORTEST
				 	
tMotor drive, steer;

int main(void) {
	tXbee_frame cmd_frame;
	tXbee_frame feedback_frame;
	tSonarModule left_sonar, right_sonar;
	char fb_msg[20];
	int fb_length=0;
	signed int drive_power, steering;
	signed long i;
/************************************************************************************************
 * System Initilizations
 ***********************************************************************************************/
	System_Init();
#ifndef MOTORTEST
	UART0_811Init();
	Xbee_Init();
	left_sonar = Sonar_Init(CCP0_PERIPH, CCP0_PORT, CCP0_PIN, SYSCTL_PERIPH_GPIOD, 
								GPIO_PORTD_BASE, GPIO_PIN_6, 0);
	right_sonar = Sonar_Init(CCP2_PERIPH, CCP2_PORT, CCP2_PIN, SYSCTL_PERIPH_GPIOD, 
								GPIO_PORTD_BASE, GPIO_PIN_7, 0);
#else
	motor_Init(PWM_GEN_1,PWM_OUT_2,PWM_OUT_3,16000,8000,&drive);
	motor_Init(PWM_GEN_2,PWM_OUT_4,PWM_OUT_5,16000,8000,&steer);
#endif

	GPIOPinWrite(GPIO_PORTB_BASE, (GPIO_PIN_0 | GPIO_PIN_1), 0);
	GPIOPinWrite(GPIO_PORTE_BASE, (GPIO_PIN_0 | GPIO_PIN_1), 0);
	while(1) {
	#ifdef MOTORTEST
	    for(i=10;i<99;i++) {
			set_motor(&drive, i);
			set_motor(&steer, -i);
			SysCtlDelay((SysCtlClockGet()/3)/10);	// 100ms delay
		}
	#else
		cmd_frame = Xbee_ReceiveRxFrame();
		if(cmd_frame.message[0] == '*') {
			// Valid Command
//			drive_power = (cmd_frame.message[2]-0x30)*100;
//			drive_power += (cmd_frame.message[3]-0x30)*10;
//			drive_power	+= (cmd_frame.message[4]-0x30);
//			if(cmd_frame.message[1] == '-') {
//				drive_power *= -1;
//			}
//
//			steering = (cmd_frame.message[7]-0x30)*100;
//			steering += (cmd_frame.message[8]-0x30)*10;
//			steering += (cmd_frame.message[9]-0x30);
//			if(cmd_frame.message[6] == '-') {
//				drive_power *= -1;
//			}
			if(cmd_frame.message[7] == 't') {  	// Tele-operated
				if(cmd_frame.message[2] == '1') {
					if(cmd_frame.message[1] == '-') {
						GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0);
						GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0xFF);
					} else {
						GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0xFF);
						GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0);
					}
				} else {
					GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0);
					GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0);
				}
				if(cmd_frame.message[5] == '1') {
					if(cmd_frame.message[4] == '-') {
						GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);
						GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0xFF);
					} else {
					 	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0xFF);
						GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0);
					}
				} else {
					GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);
					GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0);
				}
			} else { 		// Autonomous
			 	if(left_sonar.distance < 500) {
					if(right_sonar.distance < 500) {
					// Reverse
						GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0);
						GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0xFF);			
					// Turn
						GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);
						GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0xFF);
					} else {
						GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);
						GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0xFF);
					}	
				} else if(right_sonar.distance < 500) {
					GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0xFF);
					GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0);
				}
			}
			strncpy(fb_msg,"*X,X",sizeof(fb_msg));
			fb_length = 4;
			feedback_frame = Xbee_CreateTxFrame(fb_msg, fb_length);
			Xbee_SendTxFrame(&feedback_frame);
		}
	#endif
	}
}
