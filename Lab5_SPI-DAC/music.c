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
 
 extern tBoolean pause;
 
 const unsigned short Wave[32]= {
2048,2438,2813,3159,3462,3711,3896,4010,4048,4010,3896,
3711,3462,3159,2813,2438,2048,1658,1283,937,634,385,
200,86,48,86,200,385,634,937,1283,1658};

Music song = {&Wave[0], &Wave[0], 32};

 void Rewind(void){
	 Stop();
	 song.current = song.begin;
 }
 void Play(unsigned short note){
	 DAC_Out(note);
}
	 
	 
 void Stop(void){
	 pause = true;
 }
 