/*	Finite State Machine Data Structure -> FSM
 *	Linked List Data Structure
 * 		- Output (unsigned int)
 *		- Next State Pointer (FSM*)
 */
 #define NUM_STATES 8
typedef struct tState {
   char* name;
   unsigned char Output;
   struct tState* nextState[NUM_STATES];
} tState;

void states_init(void);
void setCurrentState(tState* state);
tState* getCurrentState(void);

