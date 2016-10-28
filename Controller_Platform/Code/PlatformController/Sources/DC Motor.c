#include "DC Motor.h"
#include "LCD.h"
#include "Macros.h"

char leftmotorduty;
char rightmotorduty;

//Globals for DC motor Speed calculations
unsigned int volatile firstEdgeFlagR = 0;
unsigned int volatile firstEdgeFlagL = 0;
unsigned int volatile secondEdgeFlagR = 0;
unsigned int volatile secondEdgeFlagL = 0;
unsigned long int volatile firstEdgeR = 0;
unsigned long int volatile firstEdgeL = 0;
unsigned long int volatile secondEdgeR = 0;
unsigned long int volatile secondEdgeL = 0;
signed long int volatile RMotorPeriod = 0;
signed long int volatile LMotorPeriod = 0;
signed long int volatile RMotorFrequency = 0;
signed long int volatile LMotorFrequency = 0;
signed long int volatile RMotorSpeed = 0;
signed long int volatile LMotorSpeed = 0;
unsigned long int volatile interruptCountR = 0;
unsigned long int volatile interruptCountL = 0;
unsigned char volatile RMotorOverflow = 0;
unsigned char volatile LMotorOverflow = 0;
signed long int volatile REncoderSpeed = 0;
signed long int volatile LEncoderSpeed = 0;
signed long int volatile localRMotorPeriod = 0;
signed long int volatile localLMotorPeriod = 0;
signed long int volatile REncoderFrequency = 0;
signed long int volatile LEncoderFrequency = 0;

void setDCMotorSpeed(unsigned char *instructPtr) {
  if (instructPtr[0] == 'L')
  {
    //LCDprintf("L");
    if (instructPtr[4] == '1')
    {
      SET_MOTOR_L_FORWARD;
      LCDclear();
      LCDprintf("L Motor For");
      
    }
    else if (instructPtr[4] == '2')
    {
      SET_MOTOR_L_BACKWARD;
      LCDclear();
      LCDprintf("L Motor Back");
    }
    else
    {
      SET_MOTOR_L_STOP;
      LCDclear();
      LCDprintf("L Motor Stop");
    }
    
    leftmotorduty = ((instructPtr[2]-48)*10); //ascii 
    MOTOR_L_DUTY = ((instructPtr[2]-48)*10); //change later (not likely) number between 0 - 100 
  }
   else if (instructPtr[0] == 'R')
   {
      //LCDprintf("R");
     if (instructPtr[4] == '1')
    {
      SET_MOTOR_R_FORWARD;
      LCDclear();
      LCDprintf("R Motor For"); 
    }
    else if (instructPtr[4] == '2')
    {
      SET_MOTOR_R_BACKWARD;
      LCDclear();
      LCDprintf("R Motor Back");
    }
    else
    {
      SET_MOTOR_R_STOP;
      LCDclear();
      LCDprintf("R Motor Stop");
    }
     rightmotorduty = ((instructPtr[2]-48)*10);
     MOTOR_R_DUTY = ((instructPtr[2]-48)*10); //change later (not likely) number between 0 - 100
   }
    
	
	
	//MOTOR_R_DUTY = number between 0 - 100

  
  
  //MOTOR_L_DUTY = number between 0 - 100
}

void DC_Motor_init(){
  
  //LCDclear();
  //LCDprintf("DC Motor init");

  SETUP_MOTOR_ENABLE_DDR;

  SETUP_PWMCTL;
  SETUP_PWMPRCLK;
	SETUP_PWMCLK;
	SETUP_PWMPOL;
	SETUP_PWMCAE;
	SETUP_PWME;
	
	SETUP_MR_PWMCNT;
	SETUP_MR_PWMPER;
	SETUP_MR_DUTY;
	
	SETUP_ML_PWMCNT;
	SETUP_ML_PWMPER;
	SETUP_ML_DUTY;
}

void RMotorEncoder()
  {
    //signed long int localRMotorPeriod = 0; 

    
    DisableInterrupts;
    localRMotorPeriod = RMotorPeriod;
    EnableInterrupts;
    
    REncoderFrequency = ECLOCK / (localRMotorPeriod * MOTORPRESCALER);
    REncoderSpeed = ((REncoderFrequency * 100) / NUMENCODERVANES);
    RMotorFrequency = ((REncoderSpeed * 100) / GEARRATIO);
    RMotorSpeed = ((PI * RWHEELDIAMETER * RMotorFrequency)/100000); 
      
  }
  
void LMotorEncoder()
  {
  
    DisableInterrupts;
    localLMotorPeriod = LMotorPeriod;
    EnableInterrupts;
    
    LEncoderFrequency = ECLOCK / (localLMotorPeriod * MOTORPRESCALER);
    LEncoderSpeed = ((LEncoderFrequency * 100) / NUMENCODERVANES);
    LMotorFrequency = ((LEncoderSpeed * 100) / GEARRATIO);
    LMotorSpeed = ((PI * LWHEELDIAMETER * LMotorFrequency)/100000); 
      

  }
  