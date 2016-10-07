#ifndef SCI_H
#define SCI_H

//Constant Defines
//8MHz bus clock
#define ECLOCK 8000000
//Module automatically divides Eclock by 16
#define RS232PRESCALER 16
//Required Baud Rate
#define BAUD_RATE 38400
//Configure SCICR1
//1 start bit, 8 data bits, 1 stop bit, no parity
#define N81 0b00000000
//Configure SCICR2
//Turn on transmitter 
#define TRANSMIT_ENABLE 0b00001000
//Turn on receiver
#define RECEIVER_ENABLE 0b00000100
#define RECEIVER_INTERRUPT_ENABLE 0b00100000
//Select receive data register bit
#define RECEIVE_FULL_FLAG_MASK 0b0010000
//Instruction buffer size
#define INSTRUCT_BUFF_SIZE 9 //5 * instruction size

//LED flashing defines 
#define DOWN_COUNT_VALUE      9                 // number of RTI interrupts between LED toggles
#define RTI_PERIOD            0x7F              // RTI period setup in RTICTL (see manual)
#define LED_BITS_MASK         (PTS_PTS2_MASK | PTS_PTS3_MASK)       // LED bits on port S
#define LED_DDR_MASK          (DDRS_DDRS2_MASK | DDRS_DDRS3_MASK)   // DDR for LED bits on port S
#define LED_BITS_INIT         PTS_PTS2_MASK     // initial value written to port S LED bits

//Macros
//Macro to set Baud Rate
#define BAUD_RATE_DIVISOR(baudRate) ((ECLOCK / RS232PRESCALER) / (baudRate)) //Make entire section into a header

//Globals
extern unsigned char volatile instructBuffer[INSTRUCT_BUFF_SIZE]; //Size is arbitrary and will be adjusted as needed
extern unsigned int volatile receivePosition;
extern unsigned char volatile instructStart; //indicates location of start of instruction based on start charater '~'
extern unsigned char volatile instructEnd; //indicates end of instruction in buffer 
extern unsigned char volatile myMessageFlag;

//prototypes
//RS-232 protoype
void initSCI(void);
//Echo prototype
void echo(unsigned char *);
//putcSCI prototype
void putcSCI(unsigned char); //Move to SCI header as extern
//putsSCI prototype
void putsSCI(unsigned char *); 






#endif