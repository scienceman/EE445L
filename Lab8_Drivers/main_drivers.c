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

#include "motor_driver.h"
#include "sonar.h"
#include "accel.h"
#include "Xbee.h"
#include "system.h"

tMotor *left, *right;

int main(void) {
	int i;
	System_Init();
	//pwm_test_init();
	motor_Init(PWM_GEN_1,PWM_OUT_2,PWM_OUT_3,1600,800, left);
	motor_Init(PWM_GEN_2,PWM_OUT_4,PWM_OUT_5,1600,800, right);

	for(i=0;i<100;i++) {
		set_motor(left, i);
		set_motor(right, -i);
		SysCtlDelay(20000000);
	}
	while(1);
}
