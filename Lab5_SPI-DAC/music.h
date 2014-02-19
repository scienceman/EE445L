/******************************************************
 *	EE445L - Lab05 Spring 2014
 *	Music header file 
 *	Kevin Gilbert, Gilberto Rodriguez
 * 19 February 2014
 ******************************************************/

 struct Musicstruct{
    char* begin;
    char* current;
    int length;
 };

 typedef struct Music_struct Music;

 void Rewind(void);
 void Play(char*);
 void Stop(void);
