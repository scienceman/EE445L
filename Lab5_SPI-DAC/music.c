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

Music song;

 void Rewind(void){
	 Stop();
	 song.current = song.begin;
 }
 void Play(unsigned short note){
	 pause = false;
	 DAC_Out(note);
}
	 
	 
 void Stop(void){
	 pause = true;
	 
 }
 
