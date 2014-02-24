/******************************************************
 *	EE445L - Lab05 Spring 2014
 *	Music header file 
 *	Kevin Gilbert, Gilberto Rodriguez
 * 19 February 2014
 ******************************************************/

//Interrupt period is 50000000/32/440 = 3551 counts = 71us
 
#define HIGHC 2988 // 523 Hz   
#define B 3163  // 494 Hz
#define Bb 3353 // 466 Hz
#define A 3551	// 440 Hz
#define Ab 3765 // 415 Hz
#define G 3986  // 392 Hz
#define Gb 4223 // 370 Hz 
#define F 4477  // 349 Hz
#define E 4735  // 330 Hz
#define Eb 5024 // 311 Hz
#define D 5315  // 294 Hz
#define Db 5641 // 277 Hz
#define C 5964  // 262 Hz

 typedef struct{
    const unsigned short* begin;
    const unsigned short* current;
    int length;
 } Music;

 void Rewind(void);
 void Play(unsigned short);
 void Stop(void);
