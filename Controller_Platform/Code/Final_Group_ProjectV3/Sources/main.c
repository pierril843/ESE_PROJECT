/*================================================================================== 
Motor Platform controller code for execution on semester 2 manufactored PCB
Group project completed by: Josh Perry, Riley Pierce, Ryan Furry
last update - 06/05/14
change list -

===================================================================================*/

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <stdlib.h>
#include <stdio.h>
//#include <ctype.h> //for tolower
#include <string.h>
//include macros header
#include "Macros.h"
//include stepper header
#include "Stepper.h"
//include servo header
#include "servoHead.h"
//include LCD header
#include "LCD.h"
//include DC motor header
#include "DC Motor.h"
//include RS-232 header
#include "SCI.h"

//Prototype
void logCommands();

//Defines
#define LIMITLEFT 0b01000000
#define LIMITRIGHT 0b10000000

//Camera homing prototype
void homeCam(void);

//Globals for camera home
unsigned int limitswitches;
unsigned int volatile stepCount;

void main(void)
{
   unsigned char fullInstruction[8] = "";
   unsigned char instructOffset = 0;
   unsigned char i;
   unsigned char switchStatementError = 0;
   unsigned int  instructSize = 0;
   unsigned char errorMessage[6] = "I X X";
   
//LED flashing code for debug
  PTS = LED_BITS_INIT;
  DDRS  |= LED_DDR_MASK;
  
  DDRT = TOP4BITS;  
 
  //Initialize servo
  configureTimer();
  
  //Initialize LCD
  LCDinit();
     
  //Initialize DC motor
  DC_Motor_init();
  
  //Initialize RS-232
  initSCI();
  
  //Initialize Stepper
  stepperInit();
 
  EnableInterrupts;
  
  // home camera
  homeCam(); 
  
  
  //infinite for loop
  for (;;)
  {
    while (myMessageFlag == 0); //busy wait inbetween instructions. 
    
    //critical region
    DisableInterrupts;
    instructOffset = instructStart;
    for (i = 0; i < 5; i++) //change back for full command
    {
       fullInstruction[i] = instructBuffer[instructOffset]; //copy instruction
       instructOffset++;
    }
    //instructSize = (instructEnd - instructStart);
    myMessageFlag = 0;
    EnableInterrupts;
    //*buffPtr = fullInstruction[0];
    
    //determine who the instruction is for based on first character 
    switch(fullInstruction[0])
    {
      case 'R':
      //function call
      setDCMotorSpeed(fullInstruction);
      echo(fullInstruction); //temp for debugging
      break;
      case 'L':
      //function call
      setDCMotorSpeed(fullInstruction);
      echo(fullInstruction); //temp for debugging
      break;
      case 'S':
      //function call
      changeStep(fullInstruction);
      echo(fullInstruction); //temp for debugging
      break;
      case 'O':
      //function call
      setPulseWidth(fullInstruction);
      echo(fullInstruction); //temp for debugging
      break;
      case 'P':
      //function call
       echo(fullInstruction); //temp for debugging
      break;
      case 'E':
      //function call
      echo(fullInstruction);
      break;
      case 'C':
      homeCam();
      echo(fullInstruction);
      break;
      default:
      echo(errorMessage);  
    }
  }
} 

//=================================Interrupt Handlers==============================================

/*                                receiveInstruction() 
This interrupt handler reads the contents of register SCIDRL into a ring buffer
Global variables touched - receivePosition, instructionBuffer[]  */
interrupt 20 void receiveInstruction() //correct interrupt vector?
{
   unsigned char flagValue; 
   static int countDown = DOWN_COUNT_VALUE; //for LED debugging
    
   flagValue = SCISR1 | RECEIVE_FULL_FLAG_MASK;  //How to read leave flags untouched??
  
   instructBuffer[receivePosition] = SCIDRL;
   if (instructBuffer[receivePosition] == '~') //found start character
   {
    instructStart = (receivePosition+2); //offset into ring buffer for start of instruction
   }
   else if (instructBuffer[receivePosition] == '?') //found end character
   {
    instructEnd = (receivePosition-2); //offset into ring buffer for end of command
    myMessageFlag = 1;
   }
   receivePosition = ((receivePosition + 1) % (INSTRUCT_BUFF_SIZE)); //ring buffer
   
   
   
//LED flashing code for debug purposes=================================================================
  CRGFLG = CRGFLG_RTIF_MASK; 
  
  if (--countDown == 0){                    // count this interupt -- if counted down to 0, toggle LEDs
            countDown = DOWN_COUNT_VALUE;       // reset downcounter
            PTS ^= LED_BITS_MASK;               // toggle LEDs
      }
//=======================================================================================================                  
}
 

/*=====================servoMovement interrupt handler===================================
This function sets up the action of the next OC event based on the previous action.
It checks if the previous edge was rising or falling to determine if a pulse needs to be sent

Global variables - Edge, CurrentCount, dutyCycle, pulseWidth, nextDelay

Inputs - The function uses Edge to determine the required action

Outputs - OC action is toggled every event
=======================================================================================*/
interrupt 10 void servoMovement(void)
{
  // If direction up and not at up limit: move servo up
  if ((servoDirection == '1') && (pulseWidth <= 2350))
  {
    pulseWidth += 5;
  }
  // If direction down and not at down limit: move servo down
  else if ((servoDirection == '2') && (pulseWidth >= 1300))
  {
    pulseWidth -= 5;
  }
 
  if (Edge == RISING)
  {
    ISRPulseWidth = pulseWidth;
    TC2 += ISRPulseWidth;
    Edge = FALLING;    
  }
  else
  {
    TC2 +=  (PERIODTICKS - ISRPulseWidth);
    ISRPulseWidth = 0;
    Edge = RISING;    
  }  
  
}

/*======================================stepper motor interrupt handler==========================
 
enter function description

================================================================================================*/
  interrupt 7 void stepperMovement(void)
  {
     unsigned int newIndex; 
      
     CRGFLG = 0b10000000;    //clear any possibly pending interrupts
     
     limitswitches = PTAD;
     
     if((limitswitches & LIMITLEFT) && (stepsize == 255))
     {
      stepsize = 0;
     }
     
     else if((limitswitches & LIMITRIGHT) && (stepsize == 1))
     {
      stepsize = 0;
     }
     
     if(stepsize != 0)
       stepCount++;
     
     
     newIndex = ((lastIndex + stepsize) % 8);    // calculate new index into array
         
     lastIndex = newIndex;  // update lastIndex
     
     FORCE_BITS(PTT, TOP4BITS, (lookup[newIndex]));
  
  }

/*=======================================homeCam()======================================================

 enter function description

=======================================================================================================*/
void homeCam()
{  
  int stepsRemaining = 0;
  
  LCDclear();
  LCDprintf("Homing Cam");
  
  pulseWidth = 1500;
  
  while(!(limitswitches & LIMITLEFT))
  {
    stepsize = 255;
  }
  
  stepsize = 0; 
  
  stepCount = 0;
  
  while(!(limitswitches & LIMITRIGHT))
  {
    stepsize = 1;
  }
  stepsize = 0;
  
  stepsRemaining = (stepCount / 2);
  
  stepCount = 0;
  
  while(stepsRemaining > stepCount)
  {
    stepsize = 255;
  }
  
  stepsize = 0;
  
  LCDclear();
  LCDprintf("Done HomeCam");
  
}
//===================================================================================================  

  
  
  