#include "../inc/hw_types.h"
#include "../inc/hw_memmap.h"
#include "../driverlib/sysctl.h"
#include "../driverlib/timer.h"
#include "../driverlib/gpio.h"

#include "Sonar.h"

unsigned long sonar_mailbox = 0;

tSonarModule Sonar_Init(unsigned long base, unsigned long capture, unsigned long trigger) {
	tSonarModule sonar = {base, capture, trigger};
	//SysCtlPeripheralEnable(base);    // TODO: May not need SYSCTL enable with PinType rather than GPIOPinType
	PinTypeTimer(capture);
}

void Sonar_Trigger(tSonarModule *sonar) {
	GPIOPinWrite(sonar->base, sonar->triggerPin, 0xFF);
	SysCtlClockDelay((SysCtlGetClock()/3)/100000);			// 10uS delay
	GPIOPinWrite(sonar->base, sonar->triggerPin, 0x00;
	sonar->triggerTime = SysCtlGetTime();
}

void Sonar_GetData(tSonarModule *sonar) {
	
}