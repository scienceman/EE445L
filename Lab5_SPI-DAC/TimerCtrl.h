// TimerCtrl.h
// Runs on LM3S1968
// Use Timer0 in periodic mode to request interrupts at a particular
// period.
// Kevin Gilbert, Gilberto Rodriguez
// February 23 2014

// ***************** Timer0A_Init ****************
// Activate Timer0A interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in usec
// Outputs: none
void Timer0_Init(unsigned short period);

extern unsigned short Volume;


