/*********************************************************
 * LM3S811 accelerometer measurement ADXL202JQC
 * EE445L - Lab11 Spring 2014
 * 27 April 2014
 * Kevin Gilbert, Gilberto Rodriguez
 *
 * Measures duty cycle from ADXL202JQC accelerometer (2-axis)
 ********************************************************/
/**
* 	Stellarisware 
*/
#include "../inc/hw_types.h"
#include "../inc/hw_gpio.h"								   	
#include "../inc/hw_memmap.h"
#include "../driverlib/pin_map.h"
#include "../driverlib/gpio.h"
#include "../driverlib/sysctl.h"
#include "../driverlib/interrupt.h"
/**
* 	Local Peripheral Drivers
*/
#include "accel.h"

/**
* 	Accelerometer initialization. Utilizes capture pins.
*/
void Accel_Init(unsigned long periph_base_y, unsigned long base_y, unsigned long pin_y, 
					unsigned long periph_base_x, unsigned long base_x, unsigned long pin_x) {	
	SysCtlPeripheralEnable(periph_base_y);
	SysCtlPeripheralEnable(periph_base_x); 

	GPIOPinTypeTimer(base_y, pin_y);
	GPIOPinTypeTimer(base_x, pin_x);
}
