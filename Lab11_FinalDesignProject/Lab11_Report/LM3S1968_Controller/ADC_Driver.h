
void ADCDualChannel_Init(unsigned long sysctl_base, unsigned long base, unsigned long sequence,
							 unsigned long channel1, unsigned long channel2);

void ADCDualChannel_Read(unsigned long base, unsigned long sequence, unsigned long *adcBuff);
