#include "lm3s1968.h"

void Switch_Init(void){
    int delay;
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG;
    delay = SYSCTL_RCGC2_R;
    GPIO_PORTG_DIR_R &= 0x0F;  // enable PG7-4
    GPIO_PORTG_DIR_R |= 0x04;  // set up PG2 LED
    GPIO_PORTG_DEN_R |= 0xF4;
    GPIO_PORTG_PUR_R |= 0xF0;  
    GPIO_PORTG2 = 0x00;        // Clear PG2
}
