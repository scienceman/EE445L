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

int main_menu = 1;
int counter;

void Switch_Init(void){
    int delay;
   SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG;
    delay = SYSCTL_RCGC2_R;
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
    NVIC_PRI0_R = (NVIC_PRI0_R & 0x00FFFFFF) | 0x40000000;	// Priority bits for PortG 31-29
    NVIC_EN0_R |= (NVIC_EN0_INT2 + NVIC_EN0_INT3);
	//NVIC_EN0_R |= 0x80000;
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
            RIT128x96x4Clear();
            main_menu = 0;
            break;
        case(0x68): //bottom button
            break;
        case(0x8):  //left button
            break;
        case(0x8):  //right button
            break;
    }
}

void GPIOPortG_Handler(void){
    GPIO_PORTG_ICR_R = 0x78;
    GPIO_PORTG_DATA_R |= 0x04;
}
