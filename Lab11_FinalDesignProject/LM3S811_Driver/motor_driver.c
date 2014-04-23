/****************************************************************************
 * Motor Control Drivers
 * EE445L - Spring 2014
 * 29-March-2014
 * Kevin Gilbert, Gilberto Rodriguez
 * 
 * PWM generation and duty cycle set functions
 * PWM generation from Stellarisware Documentation
 ***************************************************************************/
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

#include "lm3s811.h"
#include "motor_driver.h"

// Motor init
void motor_Init(unsigned long PWM_Generator,
			    unsigned long PWM_Out1,
				unsigned long PWM_Out2, 
				unsigned long period,
			    unsigned long dutyCycle,
				tMotor* motor) {

	motor->PWM_GEN = PWM_Generator;
	motor->PWM_OUT_POS = PWM_Out1;
	motor->PWM_OUT_NEG = PWM_Out2;
	// __________Stellarisware Document Example___________
	//
	// Configure the PWM generator for count down mode with immediate updates
	// to the parameters.
	//
	PWMGenConfigure(PWM_BASE, PWM_Generator,
		PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

	PWMGenPeriodSet(PWM_BASE, PWM_Generator, period);

	PWMPulseWidthSet(PWM_BASE, PWM_Out1, dutyCycle);
	PWMPulseWidthSet(PWM_BASE, PWM_Out2, 1);
	// Start the timers in input generator
	PWMGenEnable(PWM_BASE, PWM_Generator);
	// Enable the outputs.
	PWMOutputState(PWM_BASE, (PWM_Out1 | PWM_Out2), true);	
}									  	

// Using period of 1600, input range for speed of -100 to 100 percent.
void set_motor(tMotor* motor, signed long speed) {
	if(speed > 0) {
		PWMPulseWidthSet(PWM_BASE, motor->PWM_OUT_POS, speed*16);
		PWMPulseWidthSet(PWM_BASE, motor->PWM_OUT_NEG, 1);
	} else {
		PWMPulseWidthSet(PWM_BASE, motor->PWM_OUT_POS, 1);
		PWMPulseWidthSet(PWM_BASE, motor->PWM_OUT_NEG, (-1)*speed*16);
	}
	// Enable the outputs.
	PWMOutputState(PWM_BASE, (motor->PWM_OUT_POS | motor->PWM_OUT_NEG), true);
}


/*************************************************************************************
 *	PWM generation test
 *	Using PWM0 (LED on PG2)
 *  LM3S1968
 *************************************************************************************/
#ifdef PART_LM3S1968
void pwm_test_init(void) {
    unsigned long period;	
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
	// Set the period. For a 50 KHz frequency, the period = 1/50,000, or 20
	// microseconds. For a 20 MHz clock, this translates to 400 clock ticks.
	// Use this value to set the period.
	PWMGenPeriodSet(PWM_BASE, PWM_GEN_0, 400);
    PWMGenPeriodSet(PWM_BASE, PWM_GEN_1, 400);
	// Set the pulse width of PWM0 for a 25% duty cycle.
	PWMPulseWidthSet(PWM_BASE, PWM_OUT_0, 100);
	// Set the pulse width of PWM1 for a 75% duty cycle.
	PWMPulseWidthSet(PWM_BASE, PWM_OUT_1, 300);

	PWMPulseWidthSet(PWM_BASE, PWM_OUT_2, 300);
	PWMPulseWidthSet(PWM_BASE, PWM_OUT_3, 1);
	// Start the timers in generator 0.
	PWMGenEnable(PWM_BASE, PWM_GEN_0);
	PWMGenEnable(PWM_BASE, PWM_GEN_1);
	// Enable the outputs.
	PWMOutputState(PWM_BASE, (PWM_OUT_0_BIT | PWM_OUT_1_BIT | PWM_OUT_2_BIT | PWM_OUT_3_BIT), true);
}
#endif
