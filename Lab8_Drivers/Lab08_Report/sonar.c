#include "lm3s1968.h"
#include "../inc/hw_types.h"
#include "../driverlib/gpio.h"
#include "../inc/hw_gpio.h"
#include "../inc/hw_memmap.h"
#include "../driverlib/sysctl.h"
#include "../driverlib/interrupt.h"
#include "../inc/hw_timer.h"
#include "../driverlib/timer.h"

#include "sonar.h"

// Startup.s Function declaration
void DisableInterrupts(void);
void EnableInterrupts(void);
void StartCritical(void);
void EndCritical(void);
void WaitForInterrupt(void);

unsigned long Period; // 16-bit, 125 ns units 
unsigned long First; // Timer0A first edge 
unsigned long Done; // mailbox status set each rising 
unsigned long timeTrigger;
unsigned long timeEcho;
unsigned long range;

void PeriodMeasure_Init(void){ 
     SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER0;// activate timer0
                                   // activate port B and port G
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB+SYSCTL_RCGC2_GPIOG;
  First = 0;                     // allow time to finish activating
  GPIO_PORTG_DIR_R |= 0x04;        // make PG2 out (PG2 built-in LED)
  GPIO_PORTG_DEN_R |= 0x04;        // enable digital I/O on PG2
  GPIO_PORTB_DEN_R |= 0x01;        // enable digital I/O on PB0
  GPIO_PORTB_AFSEL_R |= 0x01;      // enable alt funct on PB0

  GPIO_PORTB_DEN_R |= 0x04;
  GPIO_PORTB_DIR_R |= 0x04;
  GPIO_PORTB_AFSEL_R &= ~0x04;

  TIMER0_CTL_R &= ~TIMER_CTL_TAEN; // disable timer0A during setup
  TIMER0_CFG_R = TIMER_CFG_16_BIT; // configure for 16-bit timer mode
                                   // configure for capture mode
  TIMER0_TAMR_R = (TIMER_TAMR_TACMR|TIMER_TAMR_TAMR_CAP);
                                   // configure for rising edge event
  TIMER0_CTL_R &= ~(TIMER_CTL_TAEVENT_POS|0xC);
  TIMER0_TAILR_R = 0x0000FFFF;     // start value
  TIMER0_IMR_R |= TIMER_IMR_CAEIM; // enable capture match interrupt
  TIMER0_ICR_R = TIMER_ICR_CAECINT;// clear timer0A capture match flag
  TIMER0_CTL_R |= TIMER_CTL_TAEN;  // enable timer0A 16-b, +edge timing, interrupts
                                   // Timer0A=priority 2
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x40000000; // top 3 bits
  NVIC_EN0_R |= NVIC_EN0_INT19;    // enable interrupt 19 in NVIC
     EnableInterrupts(); 
} 
void Timer0A_Handler(void){ 
     TIMER0_ICR_R = TIMER_ICR_CAECINT; // acknowledge timer0A capture 
     Period = (First - TIMER0_TAR_R)&0xFFFF; // 125ns resolution 
     First = TIMER0_TAR_R; // setup for next 
	 timeEcho = NVIC_ST_CURRENT_R;
	 range = timeEcho-timeTrigger;
     Done = 0xFF; 
} 