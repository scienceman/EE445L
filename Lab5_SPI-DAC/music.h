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

#define SONGLEN 24
#define MARIOLEN 78
#define MARIOLEN2 126

 typedef struct{
    const unsigned short* begin;
    const unsigned short* current;
    int length;
 } Music;

 typedef struct {
	int frequency;
	int duration;
 } tNote;

 void Rewind(void);
 void Play(unsigned short);
 void Stop(void);

extern const unsigned short Wave[32];
extern const unsigned short Wave2[32];
extern const unsigned short Wave3[32];

extern tNote mario[MARIOLEN];
extern tNote mario2[MARIOLEN2];
