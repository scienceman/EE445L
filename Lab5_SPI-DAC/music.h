/******************************************************
 *	EE445L - Lab05 Spring 2014
 *	Music header file 
 *	Kevin Gilbert, Gilberto Rodriguez
 * 19 February 2014
 ******************************************************/
 
#define HIGHC 523
#define B 494
#define Bb 466
#define A 440
#define Ab 415 
#define G 392 
#define Gb 370 
#define F 349
#define E 330
#define Eb 311
#define D 294 
#define Db 277
#define C 262

 typedef struct{
    const unsigned short* begin;
    const unsigned short* current;
    int length;
 } Music;

 void Rewind(void);
 void Play(unsigned short);
 void Stop(void);
