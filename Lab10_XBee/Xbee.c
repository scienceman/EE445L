// EE445L Spring 2014
// Xbee Functions
// 8 April 2014
// Kevin Gilbert, Gilberto Rodriguez
#include "Xbee.h"
#include "UART.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/************************************************
 * Private XBee Function Prototypes
 ***********************************************/
static void sendATCommand(void);

void Xbee_Init(void) {

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

}
