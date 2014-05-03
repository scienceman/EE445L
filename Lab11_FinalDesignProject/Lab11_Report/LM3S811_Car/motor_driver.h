/****************************************************************************
 * Motor Control Drivers Header File
 * EE445L - Spring 2014
 * 29-March-2014
 * Kevin Gilbert, Gilberto Rodriguez
 * 
 * PWM generation and duty cycle set functions
 * PWM generation from Stellarisware Documentation
 ***************************************************************************/

// Motor Data Structure	(half H-bridge
typedef struct tMotor {
	unsigned long PWM_GEN;
	unsigned long PWM_OUT_POS;
	unsigned long PWM_OUT_NEG;
} tMotor;

// Function Prototypes
void Motor_Out(int, int);
void pwm_test_init(void);
void PWM1_Init(unsigned short period, unsigned short duty);

// Initializes a motor using the L293D half H-bridge
void motor_Init(unsigned long PWM_Generator,
			    unsigned long PWM_Out1,
				unsigned long PWM_Out2, 
				unsigned long period,
			    unsigned long dutyCycle,
				tMotor* motor);
// Sets motor speed and direction
// INPUTS: -100 to 100
void set_motor(tMotor* motor, signed long speed);

void motor_Init_SW(unsigned long timer_periph_base,
				   unsigned long timer_base,  
				   unsigned long PWM_Out1,
				   unsigned long PWM_OUt2,
				   unsigned long period,
				   unsigned long dutyCycle,
				   tMotor* motor);

