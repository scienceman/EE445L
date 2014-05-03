#include "../inc/hw_types.h"
#include "../inc/hw_memmap.h"
#include "../driverlib/sysctl.h"
#include "../driverlib/pin_map.h"
#include "../driverlib/timer.h"
#include "../driverlib/gpio.h"
#include "../driverlib/pin_map.h"

#include "lm3s811.h"
#include "timerCtrl.h"
#include "Sonar.h"

void GPIOPortD_Handler(void);

unsigned long sonar_mailbox = 0;
extern tSonarModule left_sonar, right_sonar;

tSonarModule Sonar_Init(unsigned long periph_base_cap, unsigned long base_cap, unsigned long capture,
							unsigned long periph_base_trig, unsigned long base_trig, unsigned long trigger,
						    unsigned long pinConfig) {
// Create sonar module structure
	tSonarModule sonar;
	sonar.captureBase = base_cap;
	sonar.triggerBase = base_trig; 
	sonar.capturePin = capture;
	sonar.triggerPin = trigger;
	sonar.distance = 0;
// Initialize trigger and capture pins
	SysCtlPeripheralEnable(periph_base_cap);
	SysCtlPeripheralEnable(periph_base_trig); 
#ifndef PART_LM3S811
	GPIOPinConfigure(pinConfig);
#endif
#ifdef CCP_SONAR
	GPIOPinTypeTimer(base_cap, capture);
#else
//	GPIOPinTypeGPIOInput(base_cap, capture);
//	GPIOIntTypeSet(base_cap, capture, GPIO_RISING_EDGE);
//	GPIOPinIntEnable(base_cap, capture);
//	GPIOPortIntRegister(base_cap, GPIOPortD_Handler); 

	GPIO_PORTD_DIR_R &= ~0x30; // make PD4,5 input
	GPIO_PORTD_DEN_R |= 0x30; // enable I/O on PD4,5
	GPIO_PORTD_IS_R &= ~0x30; // PD4,5 is edge-sensitive
	GPIO_PORTD_IBE_R &= ~0x30; // PD4,5 is not both edges
	GPIO_PORTD_IEV_R |= 0x30; // PD4,5 rising edge event
	GPIO_PORTD_ICR_R = 0x30; // clear flag4,5
	GPIO_PORTD_IM_R |= 0x30; // arm interrupt on PD4,5
	NVIC_PRI0_R = (NVIC_PRI0_R&0x00FFFFFF)|0x50000000;
	// priority 5
	NVIC_EN0_R |= 8; // enable int 3 in NVIC 
#endif

	GPIOPinTypeGPIOOutput(base_trig, trigger);

	GPIOPinWrite(base_trig, trigger, 0x00);
	return sonar;
}

void Sonar_Trigger(tSonarModule *sonar) {
	GPIOPinWrite(sonar->triggerBase, sonar->triggerPin, 0xFF);
	SysCtlDelay((SysCtlClockGet()/3)/100000);		// 10uS delay
	GPIOPinWrite(sonar->triggerBase, sonar->triggerPin, 0x00);
#ifdef CCP_SONAR
	if(sonar->capturePin == CCP0_PIN)  
		sonar->triggerTime = TimerValueGet(TIMER0_BASE, TIMER_A);
	else 
		sonar->triggerTime = TimerValueGet(TIMER1_BASE, TIMER_A);
#else
	sonar->triggerTime = NVIC_ST_CURRENT_R; 
#endif
}

void GPIOPortD_Handler(void) {
	if(GPIO_PORTD_DATA_R&0x10) {
		GPIO_PORTD_ICR_R = 0x10;
		right_sonar.echoTime = NVIC_ST_CURRENT_R;
		right_sonar.distance = (right_sonar.triggerTime - right_sonar.echoTime) / 58; 
		if(right_sonar.distance > 999) {
			right_sonar.distance = 999;
		}
	} else {	 
	 	GPIO_PORTD_ICR_R = 0x20;
		left_sonar.echoTime = NVIC_ST_CURRENT_R;
		left_sonar.distance = (left_sonar.triggerTime - left_sonar.echoTime) / 58; 
		if(left_sonar.distance > 999) {
			left_sonar.distance = 999;
		}
	}
}
