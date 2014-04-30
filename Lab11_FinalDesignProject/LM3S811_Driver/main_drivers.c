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
#include "timerCtrl.h"
#include <string.h>

#include "lm3s811.h"

//#define MOTORTEST
#define MINRANGE 40

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
				 	
tMotor driveMotor, steerMotor;
tSonarModule left_sonar, right_sonar;
/**
* 	Driving Commands
*/
void drive(void);
void reverse(void);
void turn_left(void);
void turn_right(void);

void drive(void) {
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0xFF);
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0);
}

void reverse(void) {
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0);
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0xFF);
}

void turn_left(void) {
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0xFF);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0);
}

void turn_right(void) {
	GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0xFF);
}

int main(void) {
	tXbee_frame cmd_frame;
	tXbee_frame feedback_frame;
	char fb_msg[20];
	char left_snr_str[6];
	char right_snr_str[6];
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
	DisableInterrupts();
	left_sonar = Sonar_Init(CCP2_PERIPH, CCP2_PORT, CCP2_PIN, SYSCTL_PERIPH_GPIOD, 
								GPIO_PORTD_BASE, GPIO_PIN_6, 0);
	right_sonar = Sonar_Init(CCP0_PERIPH, CCP0_PORT, CCP0_PIN, SYSCTL_PERIPH_GPIOD, 
								GPIO_PORTD_BASE, GPIO_PIN_7, 0);
	Timer0_CaptureInit();
	Timer1_CaptureInit(); 
#else
	motor_Init(PWM_GEN_1,PWM_OUT_2,PWM_OUT_3,16000,8000,&driveMotor);
	motor_Init(PWM_GEN_2,PWM_OUT_4,PWM_OUT_5,16000,8000,&steerMotor);
#endif

	GPIOPinWrite(GPIO_PORTB_BASE, (GPIO_PIN_0 | GPIO_PIN_1), 0);
	GPIOPinWrite(GPIO_PORTE_BASE, (GPIO_PIN_0 | GPIO_PIN_1), 0);
	EnableInterrupts();
	while(1) {
	#ifdef MOTORTEST
	    for(i=10;i<99;i++) {
			set_motor(&driveMotor, i);
			set_motor(&steerMotor, -i);
			SysCtlDelay((SysCtlClockGet()/3)/10);	// 100ms delay
		}
	#else 
		cmd_frame = Xbee_ReceiveRxFrame();
		Sonar_Trigger(&left_sonar);
		//WaitForInterrupt();
		//SysCtlDelay((SysCtlClockGet()/3)/10);	//	100ms
		Sonar_Trigger(&right_sonar);
		//SysCtlDelay((SysCtlClockGet()/3)/10);	//	100ms
		//WaitForInterrupt();

		if(cmd_frame.message[0] == '*') {
			// Valid Command
			if(cmd_frame.message[7] == 't') {  	// Tele-operated
				if(cmd_frame.message[2] == '1') {
					if(cmd_frame.message[1] == '-') {
						reverse();
					} else {
						drive();
					}
				} else {
					// Stop
					GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0);
					GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0);
				}
				if(cmd_frame.message[5] == '1') {
					if(cmd_frame.message[4] == '-') {
						turn_right();
					} else {
						turn_left();
					}
				} else {
					// Stop
					GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);
					GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0);
				}
			} else { 		// Autonomous
			 	if(left_sonar.distance < MINRANGE) {
					if(right_sonar.distance < MINRANGE) {
						reverse();			
					// Turn
						turn_left();
					} else {
						turn_right();
						// Drive forward
						drive();
					}	
				} else if(right_sonar.distance < MINRANGE) {
					turn_left();
					// Drive forward
					drive();
				}
			}
			if(left_sonar.distance < 999) {
				left_snr_str[0] = (left_sonar.distance/100)+0x30;
				left_snr_str[1] = ((left_sonar.distance%100)/10)+0x30;
				left_snr_str[2] = (((left_sonar.distance%100)%10)+0x30);	
			} else {
			 	strncpy(left_snr_str,"XXX",6);
			} 
			if(right_sonar.distance < 999) {
				right_snr_str[0] = (right_sonar.distance/100)+0x30;
				right_snr_str[1] = ((right_sonar.distance%100)/10)+0x30;
				right_snr_str[2] = (((right_sonar.distance%100)%10)+0x30);
			} else {
			 	strncpy(right_snr_str,"XXX",6);
			} 
			fb_msg[0] = '*';
			for(i=0;i<3;i++) {
				fb_msg[1+i] = left_snr_str[i];
			}
			fb_msg[4] = ',';
			for(i=0;i<3;i++) {
				fb_msg[5+i] = right_snr_str[i];
			}
			fb_length = 8;
			feedback_frame = Xbee_CreateTxFrame(fb_msg, fb_length);
			Xbee_SendTxFrame(&feedback_frame);
		}
	#endif
	}
}
