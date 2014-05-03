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
#include "../inc/hw_ints.h"
#include "../inc/hw_memmap.h"
#include "../driverlib/timer.h"
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
#include "systick.h"
#include <string.h>

#include "lm3s811.h"

//#define MOTORTEST
//#define SONARTEST
//#define BROWNOUTTEST
//#define PWMTEST
#define MAIN
#define MINRANGE 40
#define PWM	95

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

/**
* 	Globals
*/				 	
tMotor driveMotor, steerMotor;
tSonarModule left_sonar, right_sonar;
unsigned int x_axis_accel, y_axis_accel;
int speed = 2;
int steering = 2;
tBoolean forward = true;
tBoolean left = false; 

/**
* 	Driving Commands
*/
void drive(int power);					    
void reverse(int power);
void turn_left(int power);
void turn_right(int power);

void drive(int power) {
	IntEnable(INT_TIMER1B);
	forward = true;
	speed = power;
}

void reverse(int power) {
	IntEnable(INT_TIMER1B);
	forward = false;
	speed = power;
}

void turn_left(int power) {
	IntEnable(INT_TIMER2A);
	left = true;
	steering = power;
}

void turn_right(int power) {
	IntEnable(INT_TIMER2A);
	left = false;
	steering = power;
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
	//Accel_Init(CCP1_PERIPH, CCP1_PORT, CCP1_PIN, CCP5_PERIPH, CCP5_PORT, CCP5_PIN);
	
	SysTick_Init();
#ifdef CCP_SONAR 
	Timer0_CaptureInit();
	Timer1_CaptureInit(); 
	Timer2_CaptureInit();
#endif

#else  
	Timer1_CaptureInit();
	Timer2_CaptureInit();
	//motor_Init(PWM_GEN_1,PWM_OUT_2,PWM_OUT_3,16000,8000,&driveMotor);
	//motor_Init(PWM_GEN_2,PWM_OUT_4,PWM_OUT_5,16000,8000,&steerMotor);	
#endif
	GPIOPinWrite(GPIO_PORTB_BASE, (GPIO_PIN_0 | GPIO_PIN_1), 0);
	GPIOPinWrite(GPIO_PORTE_BASE, (GPIO_PIN_0 | GPIO_PIN_1), 0);
	EnableInterrupts();
	while(1) {
	#ifdef MOTORTEST
	    for(i=10;i<95;i++) {
			speed = i;
			steering = i;
			//set_motor(&driveMotor, i);
			//set_motor(&steerMotor, -i);
			SysCtlDelay((SysCtlClockGet()/3)/20);	// 50ms delay
		}
		forward ^= 1;
		left ^= 1;
	#endif
	#ifdef SONARTEST
		Sonar_Trigger(&left_sonar);
		WaitForInterrupt();
		
		if(left_sonar.distance < 50)
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 1);
		else
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0);
	#endif
	#ifdef BROWNOUTTEST
	   	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0xFF);
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0);

		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0xFF);
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0);
		SysCtlDelay((SysCtlClockGet()/3)/1000);

		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0);
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);
		SysCtlDelay((SysCtlClockGet()/3)/4000);
	#endif
	#ifdef MAIN	   
		cmd_frame = Xbee_ReceiveRxFrame();
		Sonar_Trigger(&left_sonar);
		SysCtlDelay((SysCtlClockGet()/3)/100);	//	10ms
		Sonar_Trigger(&right_sonar);
		SysCtlDelay((SysCtlClockGet()/3)/100);	//	10ms

		if(cmd_frame.message[0] == '*') {
			// Valid Command
			if(cmd_frame.message[7] == 't') {  	// Tele-operated
				if(cmd_frame.message[2] == '1') {
					if(cmd_frame.message[1] == '-') {
						reverse(PWM);
					} else {
						drive(PWM);
					}
				} else {
					// Stop
					IntDisable(INT_TIMER1B);
					GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0);
					GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0);
				}
				if(cmd_frame.message[5] == '1') {
					if(cmd_frame.message[4] == '-') {
						turn_right(85);
					} else {
						turn_left(85);
					}
				} else {
					// Stop
					IntDisable(INT_TIMER2A);
					GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);
					GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0);

				}
			} else { 		// Autonomous
			 	if(left_sonar.distance < MINRANGE) {
					if(right_sonar.distance < MINRANGE) {
						reverse(85);			
					// Turn
						turn_left(85);
					} else {
						turn_right(85);
					// Drive forward
						drive(85);
					}	
				} else if(right_sonar.distance < MINRANGE) {
					turn_left(85);
					// Drive forward
					drive(85);
				}
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
	#endif
	}
}
