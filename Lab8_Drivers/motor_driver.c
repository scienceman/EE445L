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

#include "lm3s1968.h"
#include "motor_driver.h"

#define PART_LM3S1968
#define PART_IS_LM3S1968
//#define SYSCTL_RCGC2_GPIOG 0x00000040  // Port G Clock Gating Control

//PH0->PWM2
//PH1->PWM3
void pwm_test_init(void) {
	unsigned long period;
//	SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
//	
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
//	
//	period = SysCtlClockGet() / 200; //200 Hz
//
//	//GPIOPinConfigure(GPIO_PH0_PWM2);
//	//GPIOPinConfigure(GPIO_PH1_PWM3);
//	GPIOPinTypePWM(GPIO_PORTH_BASE, (GPIO_PIN_0 | GPIO_PIN_1));
//	PWMGenConfigure(PWM1_BASE, PWM_GEN_1, (PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC));
//
//	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_1, period);
//
//	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, period/2);
//	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, 1);
//
//	PWMGenEnable(PWM1_BASE, PWM_GEN_1);
//
//	PWMOutputState(PWM1_BASE, (PWM_OUT_2 | PWM_OUT_3), true);
	
	// __________Stellarisware Document Example___________

	SYSCTL_RCGC0_R |= SYSCTL_RCGC0_PWM;   // 1)activate PWM
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG;   // GPIOG
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOH;
	period = SysCtlClockGet();
	GPIO_PORTG_AFSEL_R |= 0x04;           //  enable alt funct on PG2
	GPIO_PORTH_AFSEL_R |= 0x3;            // PH0-1 = PWM2,3
	//
	// Configure the PWM generator for count down mode with immediate updates
	// to the parameters.
	//
	PWMGenConfigure(PWM_BASE, PWM_GEN_0,
		PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenConfigure(PWM_BASE, PWM_GEN_1,
		PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	//
	// Set the period. For a 50 KHz frequency, the period = 1/50,000, or 20
	// microseconds. For a 20 MHz clock, this translates to 400 clock ticks.
	// Use this value to set the period.
	//
	PWMGenPeriodSet(PWM_BASE, PWM_GEN_0, 400);
    PWMGenPeriodSet(PWM_BASE, PWM_GEN_1, 400);
	//
	// Set the pulse width of PWM0 for a 25% duty cycle.
	//
	PWMPulseWidthSet(PWM_BASE, PWM_OUT_0, 100);
	//
	// Set the pulse width of PWM1 for a 75% duty cycle.
	//
	PWMPulseWidthSet(PWM_BASE, PWM_OUT_1, 300);

	PWMPulseWidthSet(PWM_BASE, PWM_OUT_2, 300);
	PWMPulseWidthSet(PWM_BASE, PWM_OUT_3, 1);
	//
	// Start the timers in generator 0.
	//
	PWMGenEnable(PWM_BASE, PWM_GEN_0);
	PWMGenEnable(PWM_BASE, PWM_GEN_1);
	//
	// Enable the outputs.
	//
	PWMOutputState(PWM_BASE, (PWM_OUT_0_BIT | PWM_OUT_1_BIT | PWM_OUT_2_BIT | PWM_OUT_3_BIT), true);
}
