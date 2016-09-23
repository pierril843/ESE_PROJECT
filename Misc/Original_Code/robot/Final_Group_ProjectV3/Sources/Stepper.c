/*=======================================================================================
Stepper RTI code
By: 

=======================================================================================*/

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <stdio.h>
#include <stdlib.h>
#include "Macros.h"
#include "LCD.h"

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

unsigned char lookup[8] = {
  MA, MAC, MC, MBC, MB, MBD, MD, MAD
};

unsigned char stepsize;
unsigned int lastIndex;


void stepperInit(void)
{
  LCDclear();
  LCDprintf("stepper init");
  
  ATDDIEN = 0b11000000;
  
  
  stepsize = 0;
  
  lastIndex = 0;
  
  RTICTL = RTICTL_INIT;      // set RTI interval
  
  //Clear any possibly pending interrupts         
  CRGFLG = CRGFLGINIT;    //clear any possibly pending interrupts
                
  //enable interrupts from the module
  CRGINT = CRGINTINIT;    // set RTI to generate interrupt
  
  //LCDclear();
  //LCDprintf("step init Done");
}

void changeStep(unsigned char *instructPtr) 
{
  if(instructPtr[2] == '2') 
  {
    DisableInterrupts;
    stepsize = 1;
    EnableInterrupts;
    LCDclear();
    LCDprintf("STEPPER: R"); 
  }
  
  else if(instructPtr[2] == '1')
  {
    DisableInterrupts;
    stepsize = -1;
    EnableInterrupts;
    LCDclear();
    LCDprintf("STEPPER: L");  
  }
  
  else if(instructPtr[2] == '0')
  {
    DisableInterrupts;
    stepsize = 0;
    EnableInterrupts;  
  }
    
}


