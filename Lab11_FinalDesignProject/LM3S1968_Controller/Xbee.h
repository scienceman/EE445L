// EE445L Spring 2014
// Xbee Functions
// 8 April 2014
// Kevin Gilbert, Gilberto Rodriguez

#define STARTDELIM 0x7E
#define TX_DEST 0x79
#define RX_DEST 0x64
/************************************************
 * Comment for Receive
 ***********************************************/
#define TX

/************************************************
 * XBee Frame Data Structure
 ***********************************************/
 // Must escape reserver characters (0x7E,7D,11,13) by
 //		inserting x7D then the byte XOR'd with 0x20
typedef struct {
	unsigned char startDelim; // 0x7E
	signed short length;    // Length=sizeof(API+ID+destination+opt+message)
	unsigned char API;		   	
	unsigned char ID;																									   
	unsigned short destination;
	unsigned char opt;
	char* message;			  // Non-null terminated message
	unsigned char checksum;   // 0xFF-(sum [sizeof(components)])
} tXbee_frame;

/************************************************
 * Public XBee Function Prototypes
 ***********************************************/
void Xbee_Init(void);
// Message is non-null terminated string
// length is the size of the message
tXbee_frame Xbee_CreateTxFrame(char* message, int length);	   
tXbee_frame Xbee_ReceiveRxFrame(void);
void Xbee_SendTxFrame(tXbee_frame* frame);
void Xbee_TxStatus(void);
