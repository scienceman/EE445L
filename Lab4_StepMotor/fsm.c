// EE445L Spring 2014
// LM3S1968
// Finite State Machine Module
// Kevin Gilbert, Gilberto Rodriguez
// February 12, 2014
#include "fsm.h"

tState start,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13,s14,s15,s16,s17,s18;

tState* currentState;

void setCurrentState(tState* state) {
	currentState = state;
}

tState* getCurrentState(void) {
	return currentState;
}

/*****************************************************
 * Input Format:
 *    switch 0,1,2
 *	  -> PC2,PC3,PC4
 ****************************************************/
void states_init(void) {
	currentState = &start;

	start.Output = 5;
	start.nextState[0] = &start; // 000
	start.nextState[1] = &s1; // 001
	start.nextState[2] = &s1; // 010
	start.nextState[3] = &s1; // 011
	start.nextState[4] = &s1; // 100
	start.nextState[5] = &s1; // 101
	start.nextState[6] = &s1; // 110
	start.nextState[7] = &s1; // 111

	s1.Output = 6;
	s1.nextState[0] = &s1; // 000
	s1.nextState[1] = &s1; // 001
	s1.nextState[2] = &s1; // 010
	s1.nextState[3] = &s1; // 011
	s1.nextState[4] = &s1; // 100
	s1.nextState[5] = &s1; // 101
	s1.nextState[6] = &s1; // 110
	s1.nextState[7] = &s1; // 111

	s2.Output = 10;
	s2.nextState[0] = &s1; // 000
	s2.nextState[1] = &s1; // 001
	s2.nextState[2] = &s1; // 010
	s2.nextState[3] = &s1; // 011
	s2.nextState[4] = &s1; // 100
	s2.nextState[5] = &s1; // 101
	s2.nextState[6] = &s1; // 110
	s2.nextState[7] = &s1; // 111

	s3.Output = 9;
	s3.nextState[0] = &s1; // 000
	s3.nextState[1] = &s1; // 001
	s3.nextState[2] = &s1; // 010
	s3.nextState[3] = &s1; // 011
	s3.nextState[4] = &s1; // 100
	s3.nextState[5] = &s1; // 101
	s3.nextState[6] = &s1; // 110
	s3.nextState[7] = &s1; // 111

	s4.Output = 6;
	s4.nextState[0] = &s1; // 000
	s4.nextState[1] = &s1; // 001
	s4.nextState[2] = &s1; // 010
	s4.nextState[3] = &s1; // 011
	s4.nextState[4] = &s1; // 100
	s4.nextState[5] = &s1; // 101
	s4.nextState[6] = &s1; // 110
	s4.nextState[7] = &s1; // 111
	
	s5.Output = 6;
	s5.nextState[0] = &s1; // 000
	s5.nextState[1] = &s1; // 001
	s5.nextState[2] = &s1; // 010
	s5.nextState[3] = &s1; // 011
	s5.nextState[4] = &s1; // 100
	s5.nextState[5] = &s1; // 101
	s5.nextState[6] = &s1; // 110
	s5.nextState[7] = &s1; // 111

	s6.Output = 6;
	s6.nextState[0] = &s1; // 000
	s6.nextState[1] = &s1; // 001
	s6.nextState[2] = &s1; // 010
	s6.nextState[3] = &s1; // 011
	s6.nextState[4] = &s1; // 100
	s6.nextState[5] = &s1; // 101
	s6.nextState[6] = &s1; // 110
	s6.nextState[7] = &s1; // 111

	s7.Output = 6;
	s7.nextState[0] = &s1; // 000
	s7.nextState[1] = &s1; // 001
	s7.nextState[2] = &s1; // 010
	s7.nextState[3] = &s1; // 011
	s7.nextState[4] = &s1; // 100
	s7.nextState[5] = &s1; // 101
	s7.nextState[6] = &s1; // 110
	s7.nextState[7] = &s1; // 111

	s8.Output = 6;
	s8.nextState[0] = &s1; // 000
	s8.nextState[1] = &s1; // 001
	s8.nextState[2] = &s1; // 010
	s8.nextState[3] = &s1; // 011
	s8.nextState[4] = &s1; // 100
	s8.nextState[5] = &s1; // 101
	s8.nextState[6] = &s1; // 110
	s8.nextState[7] = &s1; // 111

	s9.Output = 6;
	s9.nextState[0] = &s1; // 000
	s9.nextState[1] = &s1; // 001
	s9.nextState[2] = &s1; // 010
	s9.nextState[3] = &s1; // 011
	s9.nextState[4] = &s1; // 100
	s9.nextState[5] = &s1; // 101
	s9.nextState[6] = &s1; // 110
	s9.nextState[7] = &s1; // 111

	s10.Output = 6;
	s10.nextState[0] = &s1; // 000
	s10.nextState[1] = &s1; // 001
	s10.nextState[2] = &s1; // 010
	s10.nextState[3] = &s1; // 011
	s10.nextState[4] = &s1; // 100
	s10.nextState[5] = &s1; // 101
	s10.nextState[6] = &s1; // 110
	s10.nextState[7] = &s1; // 111

	s11.Output = 6;
	s11.nextState[0] = &s1; // 000
	s11.nextState[1] = &s1; // 001
	s11.nextState[2] = &s1; // 010
	s11.nextState[3] = &s1; // 011
	s11.nextState[4] = &s1; // 100
	s11.nextState[5] = &s1; // 101
	s11.nextState[6] = &s1; // 110
	s11.nextState[7] = &s1; // 111

	s12.Output = 6;
	s12.nextState[0] = &s1; // 000
	s12.nextState[1] = &s1; // 001
	s12.nextState[2] = &s1; // 010
	s12.nextState[3] = &s1; // 011
	s12.nextState[4] = &s1; // 100
	s12.nextState[5] = &s1; // 101
	s12.nextState[6] = &s1; // 110
	s12.nextState[7] = &s1; // 111

	s13.Output = 6;
	s13.nextState[0] = &s1; // 000
	s13.nextState[1] = &s1; // 001
	s13.nextState[2] = &s1; // 010
	s13.nextState[3] = &s1; // 011
	s13.nextState[4] = &s1; // 100
	s13.nextState[5] = &s1; // 101
	s13.nextState[6] = &s1; // 110
	s13.nextState[7] = &s1; // 111

	s14.Output = 6;
	s14.nextState[0] = &s1; // 000
	s14.nextState[1] = &s1; // 001
	s14.nextState[2] = &s1; // 010
	s14.nextState[3] = &s1; // 011
	s14.nextState[4] = &s1; // 100
	s14.nextState[5] = &s1; // 101
	s14.nextState[6] = &s1; // 110
	s14.nextState[7] = &s1; // 111

	s15.Output = 6;
	s15.nextState[0] = &s1; // 000
	s15.nextState[1] = &s1; // 001
	s15.nextState[2] = &s1; // 010
	s15.nextState[3] = &s1; // 011
	s15.nextState[4] = &s1; // 100
	s15.nextState[5] = &s1; // 101
	s15.nextState[6] = &s1; // 110
	s15.nextState[7] = &s1; // 111

	s16.Output = 6;
	s16.nextState[0] = &s1; // 000
	s16.nextState[1] = &s1; // 001
	s16.nextState[2] = &s1; // 010
	s16.nextState[3] = &s1; // 011
	s16.nextState[4] = &s1; // 100
	s16.nextState[5] = &s1; // 101
	s16.nextState[6] = &s1; // 110
	s16.nextState[7] = &s1; // 111

	s17.Output = 6;
	s17.nextState[0] = &s1; // 000
	s17.nextState[1] = &s1; // 001
	s17.nextState[2] = &s1; // 010
	s17.nextState[3] = &s1; // 011
	s17.nextState[4] = &s1; // 100
	s17.nextState[5] = &s1; // 101
	s17.nextState[6] = &s1; // 110
	s17.nextState[7] = &s1; // 111

	s18.Output = 6;
	s18.nextState[0] = &s1; // 000
	s18.nextState[1] = &s1; // 001
	s18.nextState[2] = &s1; // 010
	s18.nextState[3] = &s1; // 011
	s18.nextState[4] = &s1; // 100
	s18.nextState[5] = &s1; // 101
	s18.nextState[6] = &s1; // 110
	s18.nextState[7] = &s1; // 111
}	




