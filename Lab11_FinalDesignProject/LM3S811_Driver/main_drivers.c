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

#include "lm3s1968.h"

void SysTick_IE_Init(unsigned long period);

extern unsigned long timeTrigger;
extern unsigned long range;
extern unsigned long Period;
extern unsigned long First;					 	
tMotor left, right;

int main(void) {
	signed long i;
	System_Init();
	SysTick_IE_Init(10000000);

	motor_Init(PWM_GEN_0,PWM_OUT_0,PWM_OUT_1,1600,800, &left);
	motor_Init(PWM_GEN_2,PWM_OUT_4,PWM_OUT_5,1600,800, &right);

	while(1) {
	    for(i=50;i<100;i++) {
			set_motor(&left, i);
			set_motor(&right, -i);
			SysCtlDelay(10000000);
			printf("Range: %d\r",range);
		}
	}
}

void SysTick_IE_Init(unsigned long period){
  int index;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD; // activate port D
  index = 0;
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1;// reload value
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; // priority 2
                              // enable SysTick with core clock and interrupts
  NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE+NVIC_ST_CTRL_CLK_SRC+NVIC_ST_CTRL_INTEN;
}

void SysTick_Period(unsigned long period) {
	NVIC_ST_RELOAD_R = period - 1;
}

void SysTick_Handler(void) {
	//GPIO_PORTG_DATA_R = GPIO_PORTG_DATA_R^0x04; // toggle PG2
	GPIO_PORTB_DATA_R |= 0x04;
	SysCtlDelay(170);
	GPIO_PORTB_DATA_R &= ~0x04;
	timeTrigger = NVIC_ST_CURRENT_R;
}
