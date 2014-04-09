// EE445L Spring 2014
// Xbee Functions
// 8 April 2014
// Kevin Gilbert, Gilberto Rodriguez
/************************************************
 * XBee Frame Data Structure
 ***********************************************/
typedef struct {
	const unsigned char startDelim = 0x7E;
	unsigned short length;    // Length=sizeof(API+ID+destination+opt+message)
	unsigned char API;
	unsigned char ID;
	unsigned short destination;
	unsigned char opt;
	char* message;			  // Non-null terminated message
	unsigned char checksum;   // 0xFF-(sum of components)
} tXbee_frame;

/************************************************
 * Public XBee Function Prototypes
 ***********************************************/
void Xbee_Init(void);
void Xbee_CreateTxFrame(int data);
char Xbee_ReceiveRxFrame(void);
void Xbee_SendTxFrame(void);
void Xbee_TxStatus(void);
