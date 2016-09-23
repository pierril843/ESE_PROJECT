/*=======================================================================================
Embedded Systems Software Assignment 6 - Driving an RC servo
By: Ryan Furry 30/03/16

This program uses the ADC on the HCS12 CPU to convert input voltages into 
pulse width offsets to drive a servo motor. The PWM wave is produced by timer
channel 2 which is configured for output compare. The program is interrupt
driven and main line only reads new values from the ADC. 
=======================================================================================*/

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <stdio.h>
#include <stdlib.h>
#include "Macros.h"
#include "servoHead.h"
#include "LCD.h"

typedef unsigned char uchar;
typedef unsigned int uint;

uint volatile ISRPulseWidth = 0;

uchar result = 0;
uchar volatile Edge = 0;
uint volatile pulseWidth = 1500; //center
uint volatile testVar2 = 1500;
uchar servoDirection = 0;

void setPulseWidth(unsigned char *instructPtr) 
{
   //Critical region
   DisableInterrupts;
   servoDirection = instructPtr[2]; // get direction for servo from Linux command
   EnableInterrupts;  
   
   if(instructPtr[2] == '1') {
   LCDclear();
   LCDprintf("Servo Dir: DOWN");
   } 
   else if (instructPtr[2] == '2') {
   LCDclear();
   LCDprintf("Servo Dir: UP ");
   } 
}


/*===================================cofigureTimer()===================================== 
This function configures the on chip timer module to the required settings using the
defines above

Inputs - No inputs

Outputs - Timer module is configured for proper operation
=======================================================================================*/
void configureTimer(void)
{
  //Configure TSCR2
  TSCR2 = PRESCALER;
  
  //Configure TSCR1
  TSCR1 = ENABTIME | TFRZ | TFASTCLR;

  //Make channel 2 output compare
  MAKE_CHNL_OC(2);

  //Enable interupts for channel 02
  CHNL_EN_INTERRUPTS(2);

  //Force OC to known state
  FORCE_OC_ACTION_NOW(2,OC_GO_HI); 
  Edge = RISING;

  //Delay before actual comparison occurs
  TC2 = (PERIODTICKS + TCNT);



  //Clear Pending Flags
  TFLG1 = TFLG1_C2F_MASK; //Might not be needed

  //Toggle on OC
  SET_OC_ACTION(2,OC_TOGGLE);
  //Get current timer count
  TC2 = TCNT;  
}

