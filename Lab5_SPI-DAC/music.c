/************************* Music.c *************************
 * EE445L - Lab05 
 * Spring 2014
 * SSI - DAC
 * Kevin Gilbert, Gilberto Rodriguez
 * 19 February 2014
 ***********************************************************/

 #include "music.h"
 #include "../inc/hw_types.h"
 #include "system.h"
 #include "../inc/hw_types.h"
#include "../driverlib/gpio.h"
#include "../inc/hw_gpio.h"
#include "../inc/hw_memmap.h"
#include "../driverlib/sysctl.h"
#include "../driverlib/interrupt.h"
#include "../inc/hw_timer.h"
#include "../driverlib/timer.h"
#include "../inc/hw_ints.h"
 
 extern tBoolean pause;
 extern int noteIndex2;
 extern int noteIndex;

 void Rewind(void){
	 Stop();
	 noteIndex=0;
	 noteIndex2=0;
 }
 void Play(unsigned short note){
	 pause = false;
	 IntEnable(INT_TIMER0A);
	 IntEnable(INT_TIMER1A);
}
	 	 
 void Stop(void){
	 pause = true;
	 IntDisable(INT_TIMER0A);
	 IntDisable(INT_TIMER1A);	 
 }

const unsigned short Wave[32] = {  
  125,143,159,175,189,200,208,213,215,213,208,
  200,189,175,159,143,125,107,91,75,61,50,
  42,37,35,37,42,50,61,75,91,107
};  

const unsigned short Wave2[32]= {
2048,2438,2813,3159,3462,3711,3896,4010,4048,4010,3896,
3711,3462,3159,2813,2438,2048,1658,1283,937,634,385,
200,86,48,86,200,385,634,937,1283,1658};

const unsigned short Wave3[32] = {  
  1250,1430,1590,1750,1890,2000,2080,2130,2150,2130,2080,
  2000,1890,1750,1590,1430,1250,1070,910,750,610,500,
  420,370,350,370,420,500,610,750,910,1070
};

tNote mario[MARIOLEN] = { {D,4},{E,4},{Gb,4},{G,4},{A,4},{Bb,4},{B,2},{0,1},{B,2},{0,1},{B,4},{0,1},{B,4},{0,1},
						  {B,8},{0,1},{G,4},{E/2,12},{Eb/2,12},{E/2,12},{0,2},{G,2},{A,2},{B,2},{HIGHC,2},{D/2,2},
						  {E/2,12}, {Eb/2, 8}, {F/2, 4}, {E/2, 12}, {0, 10}, {G, 2}, {D/2, 12}, {Db/2, 12}, {D/2, 12},
						  {0,2}, {G, 2}, {A, 2}, {B, 2}, {HIGHC, 2}, {Db/2, 2}, {D/2, 12}, {G, 8}, {F/2, 4}, {E/2, 12},
						  {0, 10}, {G, 2}, {G/2, 12}, {0, 1}, {G/2, 12}, {0, 1}, {G/2, 12}, {0, 1}, {G, 4}, {A/2, 4},
						  {0, 2}, {G/2, 2}, {F/2, 12}, {0, 1}, {F/2, 12}, {0, 1}, {F/2, 12}, {0, 1}, {F/2, 4}, {G/2, 4},
						  {0, 2}, {F/2, 2}, {D/2, 12}, {A, 4}, {B, 4}, {F/2, 4}, {E/2, 2}, {0, 1}, {E/2, 2}, {0, 1},
						  {E/2, 6}, {G, 2}, {A, 12}};
							
tNote mario2[MARIOLEN2] = { {E/2, 4}, {Eb/2, 4}, {D/2, 4}, {Db/2, 4}, {D/2, 4}, {Eb/2, 4}, {E/2, 2},
							{0, 1}, {E/2, 2}, {0,1}, {E/2, 4}, {A, 4}, {0, 1}, {A, 12}, {0, 1}, {D, 4},
							{A, 4}, {D/2, 4}, {0, 1}, {Db, 4}, {A, 4}, {Db/2, 4}, {0, 1}, {D, 4}, {A, 4},
							{D/2, 4}, {0, 1}, {Gb, 4}, {A, 4}, {D/2, 4}, {0, 1}, {D, 4}, {A, 4}, {D/2, 4},
							{0, 1}, {Db, 4}, {A, 4}, {Db/2, 4}, {0, 1}, {D, 4}, {A, 4}, {D/2, 4}, {0, 1},
							{Gb, 4}, {A, 4}, {D/2, 4}, {0, 1}, {E, 4}, {A, 4}, {Db/2}, {0, 1}, {Eb, 4}, 
							{Ab, 4}, {HIGHC, 4}, {0, 1}, {E, 4}, {A, 4}, {Db/2}, {0, 1}, {Db, 4}, {A, 4},
							{Db, 4}, {0, 1}, {E, 4}, {A, 4}, {Db/2}, {0, 1}, {Db, 4}, {A, 4}, {Db/2, 4}, 
							{0, 1}, {C, 4}, {A, 4}, {C/2, 4}, {0, 1}, {A*2, 4}, {A, 4}, {Db/2, 4}, 
							{0, 1}, {D, 4}, {A, 4}, {Gb/2, 4}, {0, 1}, {Db, 4}, {A, 4}, {E/2, 4}, {0, 1},
							{C, 4}, {A, 4}, {HIGHC, 4}, {0, 1}, {Eb, 4}, {A, 4}, {Gb/2, 4}, {0, 1},
							{E, 4}, {B, 4}, {G/2, 4}, {0, 1}, {Eb, 4}, {B, 4}, {G/2, 4}, {0, 1}, {C, 4},
							{B, 4}, {G/2, 4}, {0, 1}, {Db, 4}, {A, 4}, {G/2, 4}, {0, 1}, {D*2, 4}, {A, 4},
							{Gb, 4}, {0, 1}, {A*2, 4}, {A, 4}, {0, 1}, {A, 4}, {G, 2}, {0, 1}, {G, 2},
							{0, 1}, {G, 6}, {Db, 2}, {C, 12}};
 
