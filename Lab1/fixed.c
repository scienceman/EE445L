/************************************************************************
  File Name: fixed.c
  Author(s): Kevin Gilbert and Gilberto Rodriguez
  Initial Creation Date: 1/22/14
  Description: A set of funtions that can take in unsigned integer, signed integer, 
	                  and binary values and output them to the LED screen on the 1968 borad
  Lab Number: 1
  TA: Mahesh Srinivasan and Zichong Li
  Date of last revision: 1/24/14
  Hardware Configuration: n/a
*************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fixed.h"

void Fixed_uDecOut2s(unsigned long n,  char *string){
	char holderString[10];
	if (n > 99999){
		strcpy(holderString, "***.**");
	}else{
		holderString[0] = (char) (n % 10);
		sprintf(holderString, "%3lu.%02lu", n / 100, n % 100);
	}
	sprintf(string, holderString);
}

void Fixed_uDecOut2(unsigned long n) {
	 char output[10];
	 Fixed_uDecOut2s(n, output);
	 printf("%s\r",output);
}

void Fixed_sDecOut3s(long n, char *string){
	char holderString[10];
	if ((n > 9999) || (n < -9999)){
		strcpy(holderString, " *.***");
	}else{
		sprintf(holderString, "%s%lu.%03lu", (n >= 0) ? (" ") : ("-"), abs(n) / 1000, abs(n) % 1000);
	}
	sprintf(string, holderString);
}

void Fixed_sDecOut3(long n){
	 char output[10];
	 Fixed_sDecOut3s(n, output);
	 printf("%s\r",output);
}

void Fixed_uBinOut8s(unsigned long n,  char *string){
	unsigned long fixedNumber = (n * 100) / 256;
  char holderString[10];
	if (n >= 256000){
	 strcpy(holderString, "***.**");}
  else{
		holderString[0] = (char) (n % 10);
		sprintf(holderString, "%3lu.%02lu", fixedNumber / 100, fixedNumber % 100);
	}
  sprintf(string, holderString);
}

void Fixed_uBinOut8(unsigned long n){
	 char output[6];
	 Fixed_uBinOut8s(n, output);
	 printf("%s\r",output);
}
