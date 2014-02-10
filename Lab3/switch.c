#include "switch.h"
#include "lm3s1968.h"
#include "../inc/hw_types.h"
#include "../driverlib/gpio.h"
#include "../inc/hw_gpio.h"
#include "../driverlib/interrupt.h"
#include "../inc/hw_memmap.h"

//#define NVIC_PRI0_R (*((volatile unsigned long*)0xE000E400)) 

void Switch_Init(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTG_BASE, GPIO_PIN_2);
    GPIOPortIntRegister(GPIO_PORTG_BASE, GPIOPortG_Handler);
    GPIOIntTypeSet(GPIO_PORTG_BASE, GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3, GPIO_FALLING_EDGE);
    GPIOPinIntEnable(GPIO_PORTG_BASE, GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3);
    GPIO_PORTG_DATA_R &= 0xFB;        // Clear PG2
}

void GPIOPortG_Wait(void);

void GPIOPortG_Handler(void){
    GPIO_PORTG_ICR_R = 0x78;
	GPIO_PORTG_DATA_R |= 0x04;
}
