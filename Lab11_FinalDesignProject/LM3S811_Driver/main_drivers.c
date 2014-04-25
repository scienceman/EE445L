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
#include "stdio.h"

#include "motor_driver.h"
#include "sonar.h"
#include "accel.h"
#include "Xbee.h"
#include "system.h"

#include "lm3s811.h"
				 	
tMotor left, right;

int main(void) {
	signed long i;
	System_Init();

	motor_Init(PWM_GEN_1,PWM_OUT_2,PWM_OUT_3,1600,800);
	motor_Init(PWM_GEN_2,PWM_OUT_4,PWM_OUT_5,1600,800);

    motor_Set(PWM_OUT_3, PWM_OUT_4, &left);
    motor_Set(PWM_OUT_2, PWM_OUT_5, &right);

	while(1) {
	    for(i=10;i<99;i++) {
			set_motor(&left, i);
			set_motor(&right, -i);
			SysCtlDelay((SysCtlClockGet()/3)/10);	// 100ms delay
		}
	}
}
