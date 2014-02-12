#include <stdio.h>
#include <stdlib.h>
#include "fixed.h"
#include "output.h"
// const will place these structures in ROM
const struct outTestCase{       // used to test routines
  unsigned long InNumber;       // test input number
  char OutBuffer[10];           // Output String  
};
const struct s_outTestCase {
  signed long InNumber;
  char OutBUffer[10];
};

typedef const struct outTestCase outTestCaseType;
typedef const struct s_outTestCase s_outTestCaseType;
outTestCaseType outTests1[10]={ 
{     0,  "  0.00" }, //      0/100 = 0.00  
{     1,  "  0.01" }, //      1/100 = 0.01  
{    99,  "  0.99" }, //     99/100 = 0.99
{   100,  "  1.00" }, //    100/100 = 1.00
{   999,  "  9.99" }, //    999/100 = 0.99
{  1000,  " 10.00" }, //   1000/100 = 10.00
{  9999,  " 99.99" }, //   9999/100 = 99.99
{ 10000,  "100.00" }, //  10000/100 = 100.00
{ 99999,  "999.99" }, //  99999/100 = 999.99
{100000,  "***.**" }, //  error condition
};

s_outTestCaseType outTests2[10]={ 
{-100000,  " *.***"  }, //      error condition   
{ -10000,  " *.***"  }, //      error condition 
{  -9999,  "-9.999" }, //   -9999/100 = -9.999
{   -999,  "-0.999" }, //    -999/100 = -0.999
{     -1,  "-0.001" }, //      -1/100 = -0.001
{      0,  " 0.000" }, //       0/100 =  0.000
{     123, " 0.123" }, //     123/100 =  0.123
{    1234, " 1.234" }, //    1234/100 =  1.234
{    9999, " 9.999" }, //    9999/100 =  9.999
{   10000, " *.**"  }, //  error condition
};

outTestCaseType outTests3[16]={ 
{     0,  "  0.00" }, //      0/256 = 0.00  
{     4,  "  0.01" }, //      4/256 = 0.01  
{    10,  "  0.03" }, //     10/256 = 0.03
{   200,  "  0.78" }, //    200/256 = 0.78
{   254,  "  0.99" }, //    254/256 = 0.99
{   505,  "  1.97" }, //    505/256 = 1.97
{  1070,  "  4.17" }, //   1070/256 = 4.17
{  5120,  " 20.00" }, //   5120/256 = 20.00
{ 12184,  " 47.59" }, //  12184/256 = 47.59
{ 26000,  "101.56" }, //  26000/256 = 101.56
{ 32767,  "127.99" }, //  32767/256 = 127.99
{ 32768,  "128.00" }, //  32768/256 = 128
{ 34567,  "135.02" }, //  34567/256 = 135.02
{123456,  "482.25" }, // 123456/256 = 482.25
{255998,  "999.99" }, // 255998/256 = 999.99
{256000,  "***.**" }  // error
};
unsigned int Errors,AnError;
char Buffer[10];
int main(void){ // possible main program that tests your functions
  unsigned int i;
  Output_Init();
  Output_Color(15);

  Errors = 0;
  for(i=0; i<16; i++){
    printf("uBinOut8: ");
    Fixed_uBinOut8(outTests3[i].InNumber);
  }
  printf("________________\r");
  Delay(8000000);

  for(i=0; i<10; i++) {
	printf("sDecOut3: ");
	Fixed_sDecOut3(outTests2[i].InNumber);
  }
  printf("________________\r");
  Delay(8000000);
  for(i=0;i<10;i++) {
	printf("uDecOut2: ");
	Fixed_uDecOut2(outTests1[i].InNumber);
  }
  Delay(8000000);
    if(strcmp(Buffer, outTests3[i].OutBuffer)){
      Errors++;
      AnError = i;
    }	
  for(;;) {} /* wait forever */
}
