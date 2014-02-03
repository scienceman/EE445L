#include <math.h>
#include "SysTick.h"
#include "../inc/hw_types.h"
#include "../driverlib/sysctl.h"
#include "lm3s1968.h"
#include "stdio.h"
#include "FIFO.h"
#include "Output.h"
#include "rit128x96x4.h"
#include "line.h"

int lab2_partD(void);

volatile unsigned long before, elapsed;
int volatile result;

// Change to main if not testing other functions, or call from main for simplicity.
int lab2_partD(void){
	int result1=0,result2=0,result3=0;
	rxDataType data; 
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
	SYSCTL_XTAL_8MHZ); // 50 MHz
	SysTick_Init(); // initialize SysTick timer, see SysTick.c
	RxFifo_Init();
	Output_Init();
	Output_Color(15);
		
	RxFifo_Put(12);
	before = NVIC_ST_CURRENT_R;
	result1 = timeTest1(&data);
	elapsed = (before-NVIC_ST_CURRENT_R)&0x00FFFFFF;
	printf("Time for test1 is %u bus cycles.\r", elapsed);

	RxFifo_Put(13);
	before = NVIC_ST_CURRENT_R;
	result2 = timeTest2(&data);
	elapsed = (before-NVIC_ST_CURRENT_R)&0x00FFFFFF;
	printf("Time for test2 is %u bus cycles.\r", elapsed);

	RxFifo_Put(14);
	before = NVIC_ST_CURRENT_R;
	result3 = timeTest3(&data);
	elapsed = (before-NVIC_ST_CURRENT_R)&0x00FFFFFF;
	printf("Time for test3 is %u bus cycles.\r", elapsed);

   	printf("timeTest1 flag: %d\r",result1);
	printf("timeTest2 flag: %d\r",result2);
	printf("timeTest3 flag: %d\r",result3);
	return 0;
} 

int main(void) {
  Output_Init();
  Output_Color(15);
  RIT128x96x4_ClearImage();

  while(1);
}
