/******************************************************
 *	EE445L - Lab05 Spring 2014
 *	Music header file 
 *	Kevin Gilbert, Gilberto Rodriguez
 * 19 February 2014
 ******************************************************/

 typedef struct{
    const unsigned short* begin;
    const unsigned short* current;
    int length;
 } Music;

 void Rewind(void);
 void Play(unsigned short);
 void Stop(void);
