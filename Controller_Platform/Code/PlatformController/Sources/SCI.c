#include "SCI.h"
#include <string.h>
#include "derivative.h" 

//globals
unsigned char volatile instructBuffer[INSTRUCT_BUFF_SIZE]; //Size is arbitrary and will be adjusted as needed
unsigned int volatile receivePosition = 0;
unsigned char volatile instructStart = 0; //indicates location of start of instruction based on start charater '~'
unsigned char volatile instructEnd = 0; //indicates end of instruction in buffer 
unsigned char volatile myMessageFlag = 1;


void initSCI()
{   
   unsigned char commsError = 0;
   
   //LCDclear();
   //LCDprintf("SCI init");
   
    //Set Baud Rate
    SCIBD = BAUD_RATE_DIVISOR(BAUD_RATE);
    //Received data is formatted N81
    SCICR1 = N81; //Should make defines for individual bits
    //Turn on transmit and receive, Enable receive interrupts
    SCICR2 |= (TRANSMIT_ENABLE | RECEIVER_ENABLE | RECEIVER_INTERRUPT_ENABLE);
    
    //Handle comms errors 
    commsError = SCISR1;  //Is this correct?
    SCIDRL = 0;          //should it be a loop?     
}

void echo(unsigned char *instruction)
{
  putsSCI(instruction);    
}

void putsSCI(unsigned char *instruction)
{
    unsigned char i = 0;
    unsigned int lenStr = 0;
    
    lenStr = strlen((const signed char *)instruction);
    
    for (i = 0; i < lenStr; i++)//must make dynamic
    {
    
     putcSCI(*instruction++);
    
    }
    
}

void putcSCI(unsigned char instruction)
{
  
  while(!(SCISR1 & SCISR1_TDRE_MASK));
    
  SCIDRL = instruction;
}  