/***************************************************
  File Name: fixed.c
  Author(s): Kevin Gilbert and Gilberto Rodriguez
  Initial Creation Date: 1/22/14
  Description: stuff
  Lab Number: 1
  TA: 
  Date of last revision: 1/24/14
  Hardware Configuration: n/a
***************************************************/
#include <stdio.h>
#include <string.h>
#include "fixed.h"

void Fixed_uDecOut2s(unsigned long n,  char *string){
	char holderString[10];
	if (n > 99999){
		strcpy(holderString, "***.**");
	}else{
		holderString[0] = (char) (n % 10);
		strcat((char*) (n % 100), holderString);
		strcat(".", holderString);
		strcat((char*) (n / 100), holderString);
	}
	sprintf(string, holderString);
}

void Fixed_uDecOut2(unsigned long n) {
	 char output[10];
	 Fixed_uDecOut2s(n,output);
	 printf(output);
}

void Fixed_sDecOut3s(long n, char *string){
	char holderString[10];
	if ((n > 9999) || (n < -9999){
		strcpy(holderString, "***.**");
	}else{
		holderString[0] = (char) (n % 10);
		strcat((char*) (n % 100), holderString);
		strcat((char*) (n % 1000), holderString);
		strcat(".", holderString);
		strcat((char*) (n / 100), holderString);
		if (n != 0) strcat((n > 0) ? ("+") : ("-"), holderString);
	}
	sprintf(string, holderString);
}
void Fixed_sDecOut3(long n);
void Fixed_uBinOut8s(unsigned long n,  char *string){
char holderString[10];
	if ((n > 9999) || (n < -9999){
		strcpy(holderString, "***.**");
	}else{
		holderString[0] = (char) (n % 10);
		strcat((char*) (n % 100), holderString);
		strcat((char*) (n % 1000), holderString);
		strcat(".", holderString);
		strcat((char*) (n / 100), holderString);
		if (n != 0) strcat((n > 0) ? ("+") : ("-"), holderString);
	}
	sprintf(string, holderString);
}
}
void Fixed_uBinOut8(unsigned long n); 
