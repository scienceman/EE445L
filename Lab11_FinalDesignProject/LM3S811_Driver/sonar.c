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

unsigned long sonar_mailbox = 0;
extern tSonarModule left_sonar;

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
	GPIOPinTypeTimer(base_cap, capture);
	GPIOPinTypeGPIOOutput(base_trig, trigger);

	GPIOPinWrite(base_trig, trigger, 0x00);
	return sonar;
}

void Sonar_Trigger(tSonarModule *sonar) {
	GPIOPinWrite(sonar->triggerBase, sonar->triggerPin, 0xFF);
	SysCtlDelay((SysCtlClockGet()/3)/100000);		// 10uS delay
	GPIOPinWrite(sonar->triggerBase, sonar->triggerPin, 0x00);
	if(sonar->captureBase == CCP0_PORT)  
		sonar->triggerTime = TimerValueGet(TIMER0_BASE, TIMER_A);
	else 
		sonar->triggerTime = TimerValueGet(TIMER1_BASE, TIMER_A); 
}
