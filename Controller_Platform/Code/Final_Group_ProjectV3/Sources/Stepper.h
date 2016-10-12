#ifndef STEPPER_H
#define STEPPER_H


//Constant defines
#define MA        0b10000000               // bit patterns for the stepper motor
#define MAC       0b10100000               
#define MC        0b00100000              
#define MBC       0b01100000              
#define MB        0b01000000              
#define MBD       0b01010000              
#define MD        0b00010000              
#define MAD       0b10010000 
#define TOP4BITS  0b11110000  

#define RTICTL_INIT 0b01001001            // init value for RTICTL reg 10 x 2^13 
#define CRGFLGINIT  0b11111111
#define CRGINTINIT  0b10000000
               
//Globals
extern unsigned char lookup[];
extern unsigned char stepsize;                   // desired step size
extern unsigned int lastIndex;                  // last index used

//prototypes
void stepperInit(void);                   // function to initialize stepper 
void changeStep(unsigned char *instructPtr);        // function change step size

#endif