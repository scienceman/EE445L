// XBee Driver
// Runs on LM3S1968
// XBee test drivers
// EE445L Spring 2014
// Lab10
// Kevin Gilbert, Gilberto Rodriguez
// 8 April 2014
//#include "system.h"
#include "UART.h"
#include "xbee.h"

#include "../inc/hw_types.h"
#include "../inc/hw_memmap.h"
#include "../driverlib/uart.h"

#include <stdio.h>
#include <string.h>


// Startup function prototypes
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

/******************************
 * Comment TX in header to enable RX driver
 *****************************/

#define TX

#ifdef TX
	#include "lm3s1968.h"
#else 
	#include "lm3s811.h"
	#include "../inc/hw_types.h"
	#include "../inc/hw_memmap.h"
	#include "../driverlib/sysctl.h"
	#include "../driverlib/gpio.h"
	void LM811_Init(void);

	void LM811_Init(void) {
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
		GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1);
		GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0|GPIO_PIN_1);
		GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0|GPIO_PIN_1, 0x00);
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1, 0x00);
	}
#endif

#ifdef TX
int main(void) {
	tXbee_frame frame;
	char message[100] = "science";
	System_Init();
	UART1_Init();
	Xbee_Init();
	//Output_Init();
	//Output_Color(15);
	frame = Xbee_CreateTxFrame(message, 7);
	printf("Transmitting...\r");
	while(1) {
		Xbee_SendTxFrame(&frame);
	}
}

#else

int main(void) {
	tXbee_frame frame;
	LM811_Init();
	UART0_811Init();
	Xbee_Init();
	GPIO_PORTB_DATA_R |= 0x01;

	while(1) {
		frame = Xbee_ReceiveRxFrame();

		if(frame.message[0] != 's') {
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0);
			GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0x01);
		} else {
			GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0x00);
			GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0x02);
		}
	}
}
#endif

