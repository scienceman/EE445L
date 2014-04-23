typedef struct {
	unsigned long captureBase;
	unsigned long triggerBase;
	unsigned long triggerPin;
	unsigned long capturePin;
	unsigned long triggerTime;
	unsigned long echoTime;
	unsigned long distance;
} tSonarModule;

tSonarModule Sonar_Init(unsigned long periph_base_cap, unsigned long base_cap, unsigned long capture,
							unsigned long periph_base_trig, unsigned long base_trig, unsigned long trigger);
void Sonar_Trigger(tSonarModule *sonar);
void Sonar_GetData(tSonarModule *sonar);
