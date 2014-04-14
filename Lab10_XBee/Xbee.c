// EE445L Spring 2014
// Xbee Functions
// 8 April 2014
// Kevin Gilbert, Gilberto Rodriguez
#include "../inc/hw_types.h"
#include "../driverlib/timer.h"	
#include "../driverlib/sysctl.h"
#include "lm3s1968.h"
#include "Xbee.h"
#include "UART.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char X = 0x58;
char* plusOut = "+++";
char buff[10] = {0};
/************************************************
 * Private XBee Function Prototypes
 ***********************************************/
static void sendATCommand(void);

void Xbee_Init(void) {
	char* response = &buff[0];
	GPIO_PORTG_DATA_R &= 0x04;
	do {
		sendATCommand();
		UART_InString(response,10);
	} while(response[0] != 'O' || response[1] != 'K');

  	GPIO_PORTG_DATA_R ^= 0x04;
	
	do {
		UART_OutString("ATDL4\r");
		SysCtlDelay(((SysCtlClockGet()/3)/50));	//20ms delay
		UART_InString(response,10);
	} while(response[0] != 'O' || response[1] != 'K');
	GPIO_PORTG_DATA_R ^= 0x04;
	do {
		UART_OutString("ATDH0\r");
		SysCtlDelay(((SysCtlClockGet()/3)/50));	//20ms delay
		UART_InString(response,10);
	} while(response[0] != 'O' || response[1] != 'K');	
	GPIO_PORTG_DATA_R ^= 0x04;
	do {
		UART_OutString("ATMY4E\r");
		SysCtlDelay(((SysCtlClockGet()/3)/50));	//20ms delay
		UART_InString(response,10);
	} while(response[0] != 'O' || response[1] != 'K');
	GPIO_PORTG_DATA_R ^= 0x04;
	do {
		UART_OutString("ATAP1\r");
		SysCtlDelay(((SysCtlClockGet()/3)/50));	//20ms delay
		UART_InString(response,10);
	} while(response[0] != 'O' || response[1] != 'K');
	GPIO_PORTG_DATA_R ^= 0x04;
	do {
		UART_OutString("ATCN\r");
		SysCtlDelay(((SysCtlClockGet()/3)/50));	//20ms delay
		UART_InString(response,10);
	} while(response[0] != 'O' || response[1] != 'K');	
	GPIO_PORTG_DATA_R ^= 0x04;
}

char generate_checksum(tXbee_frame* frame, int messageLen) {
	int stringSum,i;
	stringSum=0;
	for(i=0;i<messageLen;i++) {
    	stringSum += frame->message[i];
	}
	return 0xFF-((frame->API+frame->ID+frame->destination+frame->opt+stringSum)&0xFF);
}

void Xbee_CreateTxFrame(char* message, int length) {
	tXbee_frame frame;
	frame.startDelim = STARTDELIM;
	frame.API = 0x01;
	frame.ID = 0x00;
	frame.destination = 0x0000;
	frame.opt = 0x00;
	frame.message = message;
	frame.length = 5+length;
	frame.checksum = generate_checksum(&frame,length);
}

tXbee_frame Xbee_ReceiveRxFrame(void) {
	int i;
	char message[100] = {0};
	unsigned char ch;
	tXbee_frame frame;
	char destTop,destBottom,lenTop,lenBottom;

	while(ch != 0x7E){ ch = UART_InChar(); }    // Wait for new frame
		
	frame.startDelim = STARTDELIM;
	lenTop = UART_InChar();
	lenBottom = UART_InChar();
	frame.length = lenTop;
	frame.length = frame.length << 8;
	frame.length += lenBottom;

	frame.API = UART_InChar();
	frame.ID = UART_InChar();

	destTop = UART_InChar();
	destBottom = UART_InChar();
	frame.destination = destTop;
	frame.destination = frame.destination << 8;
	frame.destination += destBottom;

	frame.opt = UART_InChar();

	for(i=0;i<frame.length-5;i++) {	 // 5 bytes in length used to represent the API,ID,dest,and Opt.
		message[i]=UART_InChar();	
	}
	frame.message = message;
	
	frame.checksum = UART_InChar();
	if(frame.checksum != generate_checksum(&frame,(frame.length-5))) {
		// Checksums do not match
		frame.length = -1;	// Set length to -1
		return frame;
	}
	return frame;
}

void sendATCommand(void) {
	UART_OutChar(X);
	SysCtlDelay(((SysCtlClockGet()/3)));	//1 second delay
	SysCtlDelay(((SysCtlClockGet()/3)/10));	//100ms delay
	UART_OutString(plusOut);
	SysCtlDelay(((SysCtlClockGet()/3)));	//1 second delay
	SysCtlDelay(((SysCtlClockGet()/3)/10));	//100ms delay

}
