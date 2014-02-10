#include "switch.h"
#include "lm3s1968.h"
#include "gpio.h"
#include "interrupt.h"
#include "hw_memmap.h"

//#define NVIC_PRI0_R (*((volatile unsigned long*)0xE000E400)) 

void Switch_Init(void){
    int delay;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTG_BASE, GPIO_PIN_2);
    GPIOPortIntRegister(GPIO_PORTG_BASE, GPIOPortG_Handler);
    GPIOIntTypeSet(GPIO_PORTG_BASE, GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3, GPIO_FALLING_EDGE);
    GPIOPinIntEnable(GPIO_PORTG_BASE, GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3);
//    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG;
//    delay = SYSCTL_RCGC2_R;
//    GPIO_PORTG_DIR_R &= ~0x78;  // enable PG7-4
//    GPIO_PORTG_DIR_R |= 0x04;  // set up PG2 LED
//    GPIO_PORTG_DEN_R |= 0x7C;
//    GPIO_PORTG_PUR_R |= 0x78;  
//    GPIO_PORTG_IS_R &= ~0x78;
//    GPIO_PORTG_IBE_R &= ~0x78;
//    GPIO_PORTG_IEV_R &= ~0x78;
//    GPIO_PORTG_ICR_R |= 0x78;
//    GPIO_PORTG_IM_R |= 0x78;
      GPIO_PORTG_DATA_R &= 0xFB;        // Clear PG2
//    NVIC_PRI0_R = (NVIC_PRI0_R & 0x00FFFFFF) | 0x40000000;	// Priority bits for PortG 31-29
//    NVIC_EN0_R |= (NVIC_EN0_INT2 + NVIC_EN0_INT3);
	//NVIC_EN0_R |= 0x80000;
}



void GPIOPortG_Handler(void){
    GPIO_PORTG_ICR_R = 0x78;
	GPIO_PORTG_DATA_R |= 0x04;
}
