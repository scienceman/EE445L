#include "switch.h"
#include "lm3s1968.h"
#include "../inc/hw_types.h"
#include "../driverlib/gpio.h"
#include "../inc/hw_gpio.h"
#include "../driverlib/interrupt.h"
#include "../inc/hw_memmap.h"
#include "../driverlib/sysctl.h"
#include "../driverlib/interrupt.h"
#include "rit128x96x4.h"
#include <stdio.h>

extern int hours, minutes, count, alarmflag;
extern tBoolean reset;
int main_menu = 1;
int counter, alarm_hours, alarm_minutes;
int set_alarm, set_time;
int displayClock, button, alarm;
char alarmTime[6];

void Switch_Init(void){
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG;
    set_alarm = 0;set_time = 0;
	displayClock=0; alarm = 1;
	button = alarm_hours = alarm_minutes = 0;

	GPIOPinTypeGPIOInput(GPIO_PORTG_BASE, GPIO_PIN_7);
	GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_2);
    GPIO_PORTG_DATA_R &= 0xFB;        // Clear PG2
	EdgeCounter_Init();
}

volatile unsigned long FallingEdges = 0;
void EdgeCounter_Init(void){
	SYSCTL_RCGC2_R |= 0x00000004; // activate port C
	FallingEdges = 0; // initialize counter
	GPIO_PORTC_DIR_R &= ~0x3C; // make PC4 in
	GPIO_PORTC_DEN_R |= 0x3C; // enable I/O on PC4
	GPIO_PORTC_IS_R &= ~0x3C; // PC4 is edge-sensitive
	GPIO_PORTC_IBE_R &= ~0x3C; // PC4 is not both edges
	GPIO_PORTC_IEV_R |= 0x3C; // PC4 falling edge event
	GPIO_PORTC_ICR_R = 0x3C; // clear flag4
	GPIO_PORTC_IM_R |= 0x3C; // arm interrupt on PC4
	NVIC_PRI0_R = (NVIC_PRI0_R&0xFF00FFFF)|0x00A00000;
	// priority 5
	NVIC_EN0_R |= 4; // enable int 2 in NVIC 
}
//********************************************************************
//	GPIOPortC_Handler
// 	Sets flags that are handled in main.
void GPIOPortC_Handler(void) {
	GPIO_PORTC_ICR_R = 0x3C;
	main_menu = 0;
	//RIT128x96x4Clear();
	counter = count % 10;
	    switch(GPIO_PORTC_DATA_R&0x3C){
        case(0x20): //(1) button (closest to board)
			if(set_time){
				hours += 1;
				hours %= 24;
			}else if(set_alarm){
				alarm_hours += 1;
				alarm_hours %= 24;
			}else{
				toggleDisplay();
			}
            break;
        case(0x10): //(2) button
			if(set_time){
				minutes += 1;
				minutes %= 60;
			}else if(set_alarm){
				alarm_minutes += 1;
				alarm_minutes %= 60;
			}else{
			 	set_time = 1;
			}
			break;
        case(0x08):  //(3) button
			if(set_time){
				minutes -= 1;
				if(minutes<0) {
				 	minutes = 59;
				}
				minutes %= 60;
			}else if(set_alarm){
			 	alarm_minutes -= 1;
				if(alarm_minutes<0){
				 	alarm_minutes = 59;
				}
			}else {
			 	set_alarm=1;
			}
            break;
        case(0x04):  //(4) button
			if(set_time){
				hours -= 1;
				if(hours<0) {
				 	hours=23;
				}
				hours %= 24;
			}else if(set_alarm){
			 	alarm_hours -= 1;
				if(alarm_hours<0){
				 	alarm_hours = 23;
				}
			}else{
				alarmflag ^= 1;
				alarm = 1;
				if(alarmflag) {
					sprintf(alarmTime, "%d:%02d\r",alarm_hours,alarm_minutes);
				} else {
				 	sprintf(alarmTime, "Alarm OFF");
				}
				reset = true;
			}
            break;
		default:
			break;
    }
	button = 1;
}

void toggleDisplay(void) {
	displayClock ^= 1;
}
