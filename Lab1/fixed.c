/***************************************************
  File Name: fixed.c
  Author(s): Kevin Gilbert and Gilberto Rodriguez
  Initial Creation Date: 1/22/14
  Description: stuff
  Lab Number: 1
  TA: some dude
  Date of last revision: 1/24/14
  Hardware Configuration: n/a
***************************************************/
#include <stdio.h>
#include <string.h>
#include "fixed.h"

void Fixed_uDecOut2s(unsigned long n,  char *string){
	unsigned long fixedNumber = (n*100) / 256;
	char holderString[10];
	if (fixedNumber > 99999){
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
	long fixedNumber = (n*100) / 256;
	char holderString[10];
	if (fixedNumber > 9999 || fixedNumber < -9999){
		strcpy(holderString, "***.**");
	}else{
		holderString[0] = (char) (n % 10);
		strcat((char*) (n % 100), holderString);
		strcat((char*) (n % 1000), holderString);
		strcat(".", holderString);
		strcat((char*) (n / 100), holderString);
	}
	sprintf(string, holderString);
}
void Fixed_sDecOut3(long n);
void Fixed_uBinOut8s(unsigned long n,  char *string); 
void Fixed_uBinOut8(unsigned long n); 
