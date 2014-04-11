// EE445L Spring 2014
// Xbee Functions
// 8 April 2014
// Kevin Gilbert, Gilberto Rodriguez
#include "Xbee.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/************************************************
 * Private XBee Function Prototypes
 ***********************************************/
static void sendATCommand(void);

void Xbee_Init(void) {

}

void Xbee_CreateTxFrame(char* message, int length) {
	int i,stringSum;
	tXbee_frame frame;
	stringSum=0;
	frame.startDelim = 0x7E;
	frame.API = 0x01;
	frame.ID = 0x00;
	frame.destination = 0x0000;
	frame.opt = 0x00;
	frame.message = message;
	//frame.length = sizeof(frame)+length-sizeof(frame.length)-sizeof(frame.startDelim)-sizeof(frame.message)-sizeof(frame.checksum);
	frame.length = 5+length;
	for(i=0;i<length;i++) {
    	stringSum += message[i];
	}
	frame.checksum = 0xFF-(frame.API+frame.ID+frame.destination+frame.opt+stringSum);
}
