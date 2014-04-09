// XBee Driver
// Runs on LM3S1968
// XBee test drivers
// EE445L Spring 2014
// Lab10
// Kevin Gilbert, Gilberto Rodriguez
// 8 April 2014
#include "lm3s1968.h"
#include "UART.h"
#include "xbee.h"

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

}
#endif

#ifndef TX
int main(void) {

}
#endif