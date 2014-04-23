#define PART_LM3S1968

#include "../inc/hw_types.h"
#include "../inc/hw_memmap.h"
#include "../driverlib/sysctl.h"
#include "../driverlib/pin_map.h"
#include "../driverlib/timer.h"
#include "../driverlib/gpio.h"
#include "../driverlib/pin_map.h"

#include "lm3s1968.h"
#include "timerCtrl.h"
#include "Sonar.h"

#define GPIO_PB0_CCP0	0x00010001
#define GPIO_PB1_CCP2   0x00010401

unsigned long sonar_mailbox = 0;

tSonarModule Sonar_Init(unsigned long periph_base_cap, unsigned long base_cap, unsigned long capture,
							unsigned long periph_base_trig, unsigned long base_trig, unsigned long trigger) {
	tSonarModule sonar;
	sonar.captureBase = base_cap;
	sonar.triggerBase = base_trig; 
	sonar.capturePin = capture;
	sonar.triggerPin = trigger;
	SysCtlPeripheralEnable(periph_base_cap);
	SysCtlPeripheralEnable(periph_base_trig); 
	GPIOPinConfigure(GPIO_PB1_CCP2); 
	GPIOPinTypeTimer(base_cap, capture);
	GPIOPinTypeGPIOOutput(base_trig, trigger);


	return sonar;
}

void Sonar_Trigger(tSonarModule *sonar) {
	GPIOPinWrite(sonar->triggerBase, sonar->triggerPin, 0xFF);
	SysCtlDelay((SysCtlClockGet()/3)/100000);		// 10uS delay
	GPIOPinWrite(sonar->triggerBase, sonar->triggerPin, 0x00);
	//sonar->triggerTime = NVIC_ST_CURRENT_R;
	sonar->triggerTime = TimerValueGet(TIMER1_BASE, TIMER_A); 
}

void Sonar_GetData(tSonarModule *sonar) {
	
}
