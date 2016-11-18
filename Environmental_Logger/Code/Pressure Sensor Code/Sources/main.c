#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <stdio.h>
#include <stdlib.h>
#include "Macros.h"
#include "LCD.h"

//Treat as 16-bit register
#define TCTL_1_2 (*(volatile word * const)&TCTL1)

//Wake up converter
#define ADPU    0b10000000  //Wake up
#define FASTCLR 0b01000000  //Enable Fast Clear
#define ENABINT 0b00000000  //no interupt for now make 00000011 for both interupts
//Set up ATDCTL3
#define SEQLEN  0b00001000  //one conversion per sequence
#define FIFO    0b00000000  //Non-FIFO
#define FRZ     0b00000010  //FRZ for Debug enabled
//Set up ATDCTL4
#define SRES    0b10000000  //8-bit resolution
#define SMP     0b00100000  //4 A/D cycles
#define PRS     0b00000001  //8MHz bus clock
//Set up ATDCTL5
#define DATA    0b11000000  //Right adjusted, unsigned
#define SCAN    0b00000000  //One Scan
#define MULT    0b00000000  //Sample one channel
#define CHAN    0b00000001  //Channel 1 
//completion Flag
#define SCF 0b10000000

//Set up TSCR1
#define ENABTIME  0b10000000  //Allow timer to start counting
#define TFRZ      0b00100000  //Freeze during debug
#define TFASTCLR  0b00010000  //Enable fast clear
//Set up TSCR2
#define PRESCALER 0b00000011  // divide by 8 prescaler
//Enable Timer Channel interupts
//#define TENABINT  0b00000100  //Enable interupts on timer channel 2
//Set output compare behaviour
//#define CHAN0OUT  0b0000000000010000  //Channel 2 output compare action toggle waveform 
//number of ticks in a TCNT period
#define TCNTPERIOD 2 
//Define OC states
//#define OC_OFF		0x00
//#define OC_TOGGLE	0x01
//#define OC_GO_LO	0x02
//#define OC_GO_HI	0x03
//16-bit ints
#define DELAY 32768
//high polarity
//#define RISING 1
//Low polarity
//#define FALLING 0 
//Pulse width period in clock ticks
#define PERIODTICKS 20000 //change back
//offset to reach -90 with an input of 0
//#define NEGATIVE_90_DEGREE_OFFSET 600

typedef unsigned char uchar;
typedef unsigned int uint;

void delay(uint);
void configureADC(void);
void configureTimer(void);

//uint volatile ISRPulseWidth = 0;

unsigned int result = 0;
unsigned int temp = 0;
unsigned int count = 0;
unsigned long pressure = 0;
unsigned int multi =0;
//uchar volatile Edge = 0;
//uint volatile pulseWidth = 1500; //center


void main(void) 
{
  configureADC();
  
  configureTimer();
  
  //msDelay_init();
  
  LCDinit();
  
  TC2 = TCNT;
                     
  //EnableInterrupts;
  
  for(;;)
  {
    ATDCTL5 = (DATA | SCAN | MULT | CHAN);
    while (!(ATDSTAT0 & SCF));
    result = ATDDR0L;
    
    multi = (result * 10000);
    
    pressure = ((result * 100) / 61) * 100; 
    
    if(result != temp)
    {  
      if(count > 5000)
      {
        LCDclear();
        LCDprintf("%luPa", pressure);
        count = 0;
      }
      else
      {
        count++;
      }
    }
    
    temp = result;
    
    //msDelay(20); \

    //delay(DELAY);
    
    //LCDclear();
    
    //LCDprintf("%dPa", pressure); 
    
    //Critical region
    //DisableInterrupts;
    //pulseWidth = (7*result) + NEGATIVE_90_DEGREE_OFFSET;
    //EnableInterrupts;
  }
  
}

// servoMovement interrupt handler

// cofigureADC()

void configureADC(void)
{
  uint delayVar = DELAY;

  //Wake up converter
  ATDCTL2 = ADPU | FASTCLR;
  delay(delayVar);

  //Configure ATDCTL3
  ATDCTL3 = SEQLEN | FIFO | FRZ;

  //Configure ATDCTL4
  ATDCTL4 = SRES | SMP | PRS;  
}


// cofigureTimer

void configureTimer(void)
{
  //Configure TSCR2
  
  //TSCR2 = PRESCALER;
  TSCR2 	= 0x06; 	/* set prescaler to 64, no interrupt */
 	TSCR1 	= 0xB0; 	/* enable TCNT, fast timer flag clear, freeze */  

  //Make channel 2 output compare
  //MAKE_CHNL_OC(2);

  //Enable interupts for channel 02
  //CHNL_EN_INTERRUPTS(2);

  //Force OC to known state
  //FORCE_OC_ACTION_NOW(2,OC_GO_HI); 
  //Edge = RISING;

  //Delay before actual comparison occurs
  //TC2 = (PERIODTICKS + TCNT);

  //Configure TSCR1
  TSCR1 = ENABTIME | TFRZ | TFASTCLR;

  //Clear Pending Flags
  //TFLG1 = 0xFF; //Might not be needed

  //Toggle on OC
  //SET_OC_ACTION(2,OC_TOGGLE);  
}


// delay()

void delay(uint i)
{
  uint loop;
  
  for(loop = 0; loop < i; loop++);
}