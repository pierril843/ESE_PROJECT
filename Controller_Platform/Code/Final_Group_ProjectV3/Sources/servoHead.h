#ifndef SERVOHEAD_H
#define SERVOHEAD_H

//Defines
//Treat as 16-bit register
#define TCTL_1_2 (*(volatile word * const)&TCTL1)

//Set up TSCR1
#define ENABTIME  0b10000000  //Allow timer to start counting
#define TFRZ      0b00100000  //Freeze during debug
#define TFASTCLR  0b00010000  //Enable fast clear
//Set up TSCR2
#define PRESCALER 0b00000011  // divide by 8 prescaler
//Enable Timer Channel interupts
#define TENABINT  0b00000100  //Enable interupts on timer channel 2
//Set output compare behaviour
//#define CHAN0OUT  0b0000000000010000  //Channel 2 output compare action toggle waveform 
//number of ticks in a TCNT period
#define TCNTPERIOD 2 
//Define OC states
#define OC_OFF		0x00
#define OC_TOGGLE	0x01
#define OC_GO_LO	0x02
#define OC_GO_HI	0x03
//16-bit ints
#define DELAY 32768
//high polarity
#define RISING 1
//Low polarity
#define FALLING 0 
//Pulse width period in clock ticks
#define PERIODTICKS 20000 //change back
//offset to reach -90 with an input of 0
#define NEGATIVE_90_DEGREE_OFFSET 600

//Globals
extern uint volatile ISRPulseWidth;
extern uchar result;
extern uchar volatile Edge;
extern uint volatile pulseWidth; //center
extern uint volatile testVar2;
extern uchar servoDirection;

//Protoypes
void configureTimer(void);
void setPulseWidth(unsigned char *);













#endif