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

extern int hours, minutes;
int main_menu = 1;
int counter, alarm_hours, alarm_minutes;
int alarm, set_alarm, set_time;
int displayClock;

void Switch_Init(void){
    //int delay;
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG;
    alarm = 0; set_alarm = 0;set_time = 0;
	displayClock=0;
	alarm_hours = alarm_minutes = 0;
    //delay = SYSCTL_RCGC2_R;
    GPIO_PORTG_DIR_R &= ~0x78;  // enable PG7-4
    GPIO_PORTG_DIR_R |= 0x04;  // set up PG2 LED
    GPIO_PORTG_DEN_R |= 0x7C;
    GPIO_PORTG_PUR_R |= 0x78;  
    GPIO_PORTG_IS_R &= ~0x78;
    GPIO_PORTG_IBE_R &= ~0x78;
    GPIO_PORTG_IEV_R &= ~0x78;
    GPIO_PORTG_ICR_R |= 0x78;
    GPIO_PORTG_IM_R |= 0x78;
    GPIO_PORTG_DATA_R &= 0xFB;        // Clear PG2
    NVIC_PRI7_R = (NVIC_PRI7_R & 0x00FFFFFF) | 0x40000000;	// Priority bits for PortG 31-29
    NVIC_EN0_R |= (NVIC_EN0_INT2 + NVIC_EN0_INT3);
	//NVIC_EN0_R |= 0x80000;
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

void GPIOPortC_Handler(void) {
	GPIO_PORTC_ICR_R = 0x3C;
	    switch(GPIO_PORTC_DATA_R&0x3C){
        case(0x20): //top button
			RIT128x96x4Clear();
			toggleDisplay();
            break;
        case(0x10): //bottom button
            break;
        case(0x08):  //left button
            break;
        case(0x04):  //right button
            break;
		default:
			break;
    }
}

void GPIOPortG_Wait(void){
    if (main_menu){
        printf("(1) Set Time\r");
        printf("(2) Set Alarm\r");
        printf("(3) Turn on/off Alarm\r");
        printf("(4) Display Mode\r");
    }
    switch(GPIO_PORTG_DATA_R&0x78){
        case(0x70): //top button
			while(GPIO_PORTG_DATA_R&0x79 == 0x70);
            RIT128x96x4Clear();
            main_menu = 0;
			toggleDisplay();
            break;
        case(0x68): //bottom button
            RIT128x96x4Clear();
            main_menu = 0;
            break;
        case(0x58):  //left button
            RIT128x96x4Clear();
            main_menu = 0;
            break;
        case(0x38):  //right button
            RIT128x96x4Clear();
            main_menu = 0;
            break;
		default:
			break;
    }
}

void toggleDisplay(void) {
	displayClock ^= 1;
}

void GPIOPortG_Handler(void){
    GPIO_PORTG_ICR_R = 0x78;
    GPIO_PORTG_DATA_R ^= 0x04;
}
