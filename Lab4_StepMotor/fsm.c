// EE445L Spring 2014
// LM3S1968
// Finite State Machine Module
// Kevin Gilbert, Gilberto Rodriguez
// February 12, 2014
#include "fsm.h"
#include "Output.h"
#include "rit128x96x4.h"
#include <stdio.h>

tState start,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13,s14,s15,s16,s17,s18,s19,s20,s21,s22;

tState* currentState;
char* statePrint, stateOut;

void setCurrentState(tState* state) {
	currentState = state;
	printf("state: %s\r",(*state).name);
	printf("Output: %d\r",(*state).Output);
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

	start.name = "start";
	start.Output = 5;
	start.nextState[0] = &start; // 000
	start.nextState[1] = &s3; // 001
	start.nextState[2] = &s1; // 010
	start.nextState[3] = &s3; // 011
	start.nextState[4] = &s1; // 100
	start.nextState[5] = &s1; // 101
	start.nextState[6] = &s4; // 110
	start.nextState[7] = &s4; // 111

	s1.name = "s1";
	s1.Output = 6;
	s1.nextState[0] = &s1; // 000
	s1.nextState[1] = &s20; // 001
	s1.nextState[2] = &start; // 010
	s1.nextState[3] = &start; // 011
	s1.nextState[4] = &s2; // 100
	s1.nextState[5] = &s1; // 101
	s1.nextState[6] = &start; // 110
	s1.nextState[7] = &start; // 111

	s2.name = "s2";
	s2.Output = 10;
	s2.nextState[0] = &s2; // 000
	s2.nextState[1] = &s21; // 001
	s2.nextState[2] = &s1; // 010
	s2.nextState[3] = &s1; // 011
	s2.nextState[4] = &s3; // 100
	s2.nextState[5] = &s3; // 101
	s2.nextState[6] = &s4; // 110
	s2.nextState[7] = &s4; // 111

	s3.name = "s3";
	s3.Output = 9;
	s3.nextState[0] = &s3; // 000
	s3.nextState[1] = &s21; // 001
	s3.nextState[2] = &s2; // 010
	s3.nextState[3] = &s2; // 011
	s3.nextState[4] = &start; // 100
	s3.nextState[5] = &start; // 101
	s3.nextState[6] = &start; // 110
	s3.nextState[7] = &start; // 111

	s4.name = "s4";
	s4.Output = 6;
	s4.nextState[0] = &s4; // 000
	s4.nextState[1] = &s20; // 001
	s4.nextState[2] = &start; // 010
	s4.nextState[3] = &start; // 011
	s4.nextState[4] = &s2; // 100
	s4.nextState[5] = &s2; // 101
	s4.nextState[6] = &s5; // 110
	s4.nextState[7] = &s5; // 111
	
	s5.name = "s5";
	s5.Output = 10;
	s5.nextState[0] = &s5; // 000
	s5.nextState[1] = &s21; // 001
	s5.nextState[2] = &s1; // 010
	s5.nextState[3] = &s1; // 011
	s5.nextState[4] = &s3; // 100
	s5.nextState[5] = &s3; // 101
	s5.nextState[6] = &s6; // 110
	s5.nextState[7] = &s6; // 111

	s6.name = "s6";
	s6.Output = 9;
	s6.nextState[0] = &s6; // 000
	s6.nextState[1] = &s22; // 001
	s6.nextState[2] = &s2; // 010
	s6.nextState[3] = &s2; // 011
	s6.nextState[4] = &start; // 100
	s6.nextState[5] = &start; // 101
	s6.nextState[6] = &s7; // 110
	s6.nextState[7] = &s7; // 111

	s7.name = "s7";
	s7.Output = 5;
	s7.nextState[0] = &s7; // 000
	s7.nextState[1] = &s19; // 001
	s7.nextState[2] = &s3; // 010
	s7.nextState[3] = &s3; // 011
	s7.nextState[4] = &s1; // 100
	s7.nextState[5] = &s1; // 101
	s7.nextState[6] = &s8; // 110
	s7.nextState[7] = &s8; // 111

	s8.name = "s8";
	s8.Output = 6;
	s8.nextState[0] = &s8; // 000
	s8.nextState[1] = &s20; // 001
	s8.nextState[2] = &start; // 010
	s8.nextState[3] = &start; // 011
	s8.nextState[4] = &s2; // 100
	s8.nextState[5] = &s2; // 101
	s8.nextState[6] = &s9; // 110
	s8.nextState[7] = &s9; // 111

	s9.name = "s9";
	s9.Output = 10;
	s9.nextState[0] = &s9; // 000
	s9.nextState[1] = &s21; // 001
	s9.nextState[2] = &s1; // 010
	s9.nextState[3] = &s1; // 011
	s9.nextState[4] = &s3; // 100
	s9.nextState[5] = &s3; // 101
	s9.nextState[6] = &s10; // 110
	s9.nextState[7] = &s10; // 111

	s10.name = "s10";
	s10.Output = 9;
	s10.nextState[0] = &s10; // 000
	s10.nextState[1] = &s22; // 001
	s10.nextState[2] = &s2; // 010
	s10.nextState[3] = &s2; // 011
	s10.nextState[4] = &start; // 100
	s10.nextState[5] = &start; // 101
	s10.nextState[6] = &s11; // 110
	s10.nextState[7] = &s11; // 111

	s11.name = "s11";
	s11.Output = 5;
	s11.nextState[0] = &s11; // 000
	s11.nextState[1] = &s19; // 001
	s11.nextState[2] = &s3; // 010
	s11.nextState[3] = &s3; // 011
	s11.nextState[4] = &s1; // 100
	s11.nextState[5] = &s1; // 101
	s11.nextState[6] = &s12; // 110
	s11.nextState[7] = &s12; // 111

	s12.name = "s12";
	s12.Output = 9;
	s12.nextState[0] = &s12; // 000
	s12.nextState[1] = &s22; // 001
	s12.nextState[2] = &s2; // 010
	s12.nextState[3] = &s2; // 011
	s12.nextState[4] = &start; // 100
	s12.nextState[5] = &start; // 101
	s12.nextState[6] = &s13; // 110
	s12.nextState[7] = &s13; // 111

	s13.name = "s13";
	s13.Output = 10;
	s13.nextState[0] = &s13; // 000
	s13.nextState[1] = &s21; // 001
	s13.nextState[2] = &s1; // 010
	s13.nextState[3] = &s1; // 011
	s13.nextState[4] = &s3; // 100
	s13.nextState[5] = &s3; // 101
	s13.nextState[6] = &s14; // 110
	s13.nextState[7] = &s14; // 111

	s14.name = "s14";
	s14.Output = 6;
	s14.nextState[0] = &s14; // 000
	s14.nextState[1] = &s20; // 001
	s14.nextState[2] = &start; // 010
	s14.nextState[3] = &start; // 011
	s14.nextState[4] = &s2; // 100
	s14.nextState[5] = &s2; // 101
	s14.nextState[6] = &s15; // 110
	s14.nextState[7] = &s15; // 111

	s15.name = "s15";
	s15.Output = 5;
	s15.nextState[0] = &s15; // 000
	s15.nextState[1] = &s19; // 001
	s15.nextState[2] = &s3; // 010
	s15.nextState[3] = &s3; // 011
	s15.nextState[4] = &s1; // 100
	s15.nextState[5] = &s1; // 101
	s15.nextState[6] = &s16; // 110
	s15.nextState[7] = &s16; // 111

	s16.name = "s16";
	s16.Output = 9;
	s16.nextState[0] = &s16; // 000
	s16.nextState[1] = &s22; // 001
	s16.nextState[2] = &s2; // 010
	s16.nextState[3] = &s2; // 011
	s16.nextState[4] = &start; // 100
	s16.nextState[5] = &start; // 101
	s16.nextState[6] = &s17; // 110
	s16.nextState[7] = &s17; // 111

	s17.name = "s17";
	s17.Output = 10;
	s17.nextState[0] = &s17; // 000
	s17.nextState[1] = &s21; // 001
	s17.nextState[2] = &s1; // 010
	s17.nextState[3] = &s1; // 011
	s17.nextState[4] = &s3; // 100
	s17.nextState[5] = &s3; // 101
	s17.nextState[6] = &s18; // 110
	s17.nextState[7] = &s18; // 111

	s18.name = "s18";
	s18.Output = 6;
	s18.nextState[0] = &s18; // 000
	s18.nextState[1] = &s20; // 001
	s18.nextState[2] = &start; // 010
	s18.nextState[3] = &start; // 011
	s18.nextState[4] = &s2; // 100
	s18.nextState[5] = &s2; // 101
	s18.nextState[6] = &start; // 110
	s18.nextState[7] = &start; // 111

	s19.name = "s19";
	s19.Output = 5;
	s19.nextState[0] = &s1; // 000
	s19.nextState[1] = &s19; // 001
	s19.nextState[2] = &s1; // 010
	s19.nextState[3] = &s19; // 011
	s19.nextState[4] = &s1; // 100
	s19.nextState[5] = &s19; // 101
	s19.nextState[6] = &s4; // 110
	s19.nextState[7] = &s19; // 111

	s20.name = "s20";
	s20.Output = 6;
	s20.nextState[0] = &s2; // 000
	s20.nextState[1] = &s20; // 001
	s20.nextState[2] = &s2; // 010
	s20.nextState[3] = &s20; // 011
	s20.nextState[4] = &s2; // 100
	s20.nextState[5] = &s20; // 101
	s20.nextState[6] = &start; // 110
	s20.nextState[7] = &s20; // 111

	s21.name = "s21";
	s21.Output = 10;
	s21.nextState[0] = &s3; // 000
	s21.nextState[1] = &s21; // 001
	s21.nextState[2] = &s3; // 010
	s21.nextState[3] = &s21; // 011
	s21.nextState[4] = &s3; // 100
	s21.nextState[5] = &s21; // 101
	s21.nextState[6] = &s4; // 110
	s21.nextState[7] = &s21; // 111
	
	s22.name = "s22";
	s22.Output = 9;
	s22.nextState[0] = &start; // 000
	s22.nextState[1] = &s22; // 001
	s22.nextState[2] = &start; // 010
	s22.nextState[3] = &s22; // 011
	s22.nextState[4] = &start; // 100
	s22.nextState[5] = &s22; // 101
	s22.nextState[6] = &start; // 110
	s22.nextState[7] = &s22; // 111

}	




