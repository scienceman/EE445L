// XBee Driver
// Runs on LM3S1968
// XBee test drivers
// EE445L Spring 2014
// Lab10
// Kevin Gilbert, Gilberto Rodriguez
// 8 April 2014
#include "lm3s1968.h"
#include "system.h"
#include "UART.h"
#include "xbee.h"

#include "../inc/hw_types.h"
#include "../inc/hw_memmap.h"
#include "../driverlib/uart.h"

#include <stdio.h>


// Startup function prototypes
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

/******************************
 * Comment to enable RX driver
 *****************************/
#define TX

#ifdef TX
int main(void) {
	char* message;
	int length;
	tXbee_frame frame;
	System_Init();
	message = "science";
	length=7;
	frame = Xbee_CreateTxFrame(message, length);
	Xbee_SendTxFrame(&frame);
	while(1);
}
#endif

#ifndef TX
int main(void) {
	tXbee_frame frame;
	unsigned long baud; 
	unsigned long config;
	System_Init();
	UART1_Init();
	Xbee_Init();
	UARTConfigGetExpClk(UART1_BASE,SysCtlClockGet(),&baud,&config);
	printf("baud: %d\r",baud);

	while(1) {
		frame = Xbee_ReceiveRxFrame();
		if(frame.length == -1) {
		 	printf("Dropped Frame [checksum missmatch]\r");
		} else {
			printf("%c",frame.message[0]);
		}
	}
}
#endif
