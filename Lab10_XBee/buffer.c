/************************************************************
 * Buffer File for transmitting/receiving XBee Frames
 * 
 * EE445L Spring 2014
 * 11 April 2014
 * Kevin Gilbert, Gilberto Rodriguez
 ***********************************************************/
#include "Xbee.h"
#include "buffer.h"

static tXbee_frame* frameBuffer[BUFFSIZE];
static int buffPt=0;

long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value

void push_frame(tXbee_frame *frame) {
	long criticalSection;
	if(buffPt==BUFFSIZE-1) {
	 	return;
	} else {
		criticalSection = StartCritical();
		frameBuffer[buffPt++] = frame;
		EndCritical(criticalSection);
	}	
}

tXbee_frame* pop_frame(void) {
	long criticalSection;
 	if(buffPt==0) {
	 	return 0;   // return null pointer on empty buffer
	} else {
		criticalSection = StartCritical();
	 	return frameBuffer[buffPt--];
		EndCritical(criticalSection);
	}
}
