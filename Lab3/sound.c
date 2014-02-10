#include "../inc/hw_types.h"
#include "../driverlib/gpio.h"
#include "../driverlib/pwm.h"
#include "../driverlib/sysctl.h"
#include "../driverlib/interrupt.h"
#include "../inc/hw_memmap.h"  
#include "SysTick.h"
#include "sound.h"

void alarmInit(void) {
	/*unsigned long ulPeriod;
 	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
	GPIOPinConfigure(GPIO_PH0_PWM2);
	ulPeriod = SysCtlClockGet() / 200; //PWM period
	GPIOPinTypePWM(GPIO_PORTH_BASE, GPIO_PIN_0);

	PWMGenConfigure(PWM_BASE, PWM_GEN_2, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(PWM_BASE, PWM_GEN_2, ulPeriod);

	    //Set PWM0 duty-50%
    PWMPulseWidthSet(PWM_BASE, PWM_OUT_2,ulPeriod/2);


    // Enable the PWM0 signal.
    PWMOutputState(PWM_BASE, PWM_OUT_2_BIT, true);

    // Enable the PWM generator for outputs PWM0 & PWM1.
    PWMGenEnable(PWM_BASE, PWM_GEN_2);

	while(1);
	
				 */

}
