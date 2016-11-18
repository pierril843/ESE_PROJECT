#include "DC Motor.h"
#include "LCD.h"
#include "Macros.h"

char leftmotorduty;
char rightmotorduty;
/*/Globals for DC motor Speed calculations
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
*/
//Setpoint Variables
//signed int RMotorSetPoint = 0;
//signed int RMotorError = 0; 

signed long int RMotorLocalPeriod = 0;
signed long int LMotorLocalPeriod = 0;

signed long int volatile RiState;
signed long int volatile LiState;
signed int volatile RiMax = 500;
signed int volatile RiMin = -500;
signed int volatile RiGain = 2;
signed int volatile RpGain = 2;
signed int volatile LiMax = 500;
signed int volatile LiMin = -500;
signed int volatile LiGain = 2;
signed int volatile LpGain = 2;
//signed int volatile dGain;
//signed int volatile error;
//signed int volatile position;
signed long int volatile RpTerm = 0;
signed long int volatile LpTerm = 0;
//signed int volatile dTerm;
signed long int volatile RiTerm = 0;
signed long int volatile LiTerm = 0;
signed long int volatile Rerror = 0;
signed long int volatile Lerror = 0;
signed long long int volatile RtempPWMDuty = 0;
signed long long int volatile LtempPWMDuty = 0;
 

void setDCMotorSpeed(unsigned char *instructPtr)
{    
  if (instructPtr[0] == 'L') 
	{
		if (instructPtr[4] == '2')
		{
			if (instructPtr[2] == '1')
			{
				DisableInterrupts;
				LMotorSetPoint = 208;
				//MOTOR_L_DUTY = 150;      
				EnableInterrupts;
				LCDclear();
				LCDprintf("L Motor For");
				SET_MOTOR_L_FORWARD;				
			}						
			else if (instructPtr[2] == '2')
			{
				DisableInterrupts;
				LMotorSetPoint = 388;
				//MOTOR_L_DUTY = 225;
				EnableInterrupts;
				LCDclear();
				LCDprintf("L Motor For");
				SET_MOTOR_L_FORWARD;				
			}
		}	
		else if (instructPtr[4] == '1')
		{
			if (instructPtr[2] == '1')
			{		
				DisableInterrupts;
				LMotorSetPoint = 208;
				//MOTOR_L_DUTY = 150;
				EnableInterrupts;
				LCDclear();
				LCDprintf("L Motor For");
				SET_MOTOR_L_BACKWARD;				
			}			
			else if (instructPtr[2] == '2')
			{
				DisableInterrupts;
				LMotorSetPoint = 388;
				//MOTOR_L_DUTY = 225;
				EnableInterrupts;
				LCDclear();
				LCDprintf("L Motor For");
				SET_MOTOR_L_BACKWARD;				
			}
		}	
		else
		{
			DisableInterrupts;
			LMotorSetPoint = 0;
			EnableInterrupts;
			LCDclear();
			LCDprintf("L Motor Stop");
			SET_MOTOR_L_STOP;
		}
	}
  else if (instructPtr[0] == 'R')
  {
    if (instructPtr[4] == '2')
		{
			if (instructPtr[2] == '1')
			{				
				DisableInterrupts;
				RMotorSetPoint = 208;
				//MOTOR_R_DUTY = 150;
				EnableInterrupts;
				LCDclear();
				LCDprintf("L Motor For");
				SET_MOTOR_R_FORWARD; 				
			}		
			else if (instructPtr[2] == '2')
			{
				DisableInterrupts;
				RMotorSetPoint = 388;
				//MOTOR_R_DUTY = 225;
				EnableInterrupts;
				LCDclear();
				LCDprintf("L Motor For");
				SET_MOTOR_R_FORWARD;
				
			}
		}	
		else if (instructPtr[4] == '1')
		{
			if (instructPtr[2] == '1')
			{ 				
				DisableInterrupts;
				RMotorSetPoint = 208;
				//MOTOR_R_DUTY = 150;
				EnableInterrupts;
				LCDclear();
				LCDprintf("L Motor For");
				SET_MOTOR_R_BACKWARD;
				
			}			
			else if (instructPtr[2] == '2')
			{
				DisableInterrupts;
				RMotorSetPoint = 388;
				//MOTOR_R_DUTY = 225;
				EnableInterrupts;
				LCDclear();
				LCDprintf("L Motor For");
				SET_MOTOR_R_BACKWARD;				
			}
		}	
		else
		{
			DisableInterrupts;
			RMotorSetPoint = 0;
			EnableInterrupts;
			LCDclear();
			LCDprintf("L Motor Stop");
			SET_MOTOR_R_STOP;
		}
	}    
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
	
	
	SETUP_MR_PWMCNT;
	SETUP_MR_PWMPER;
	SETUP_MR_DUTY;
	
	SETUP_ML_PWMCNT;
	SETUP_ML_PWMPER;
	SETUP_ML_DUTY;
	
	SETUP_PWME;
}

void RMotorPI()
  {
    DisableInterrupts;
    RMotorLocalPeriod = RMotorPeriod;
    EnableInterrupts;
    
    REncoderFrequency = ECLOCK / (RMotorLocalPeriod * MOTORPRESCALER);
    REncoderSpeed = ((REncoderFrequency * 100) / NUMENCODERVANES);
    RMotorFrequency = ((REncoderSpeed * 100) / GEARRATIO);
    RMotorSpeed = ((PI * RWHEELDIAMETER * RMotorFrequency)/100000);
    
   Rerror = (RMotorSetPoint - RMotorSpeed);
   
   RpTerm = (RpGain * Rerror);
   
   RiState = (RiState + Rerror);
   if (RiState > RiMax)
    {
      RiState = RiMax;     
    }
   else if ( RiState < RiMin)
    {
      RiState = RiMin; 
    }
   RiTerm = (RiGain * RiState); 
   //dTerm = (dGain * (position - dState));
   //dState = position; 
   
   //assign duty with (pTerm+iTerm - dTerm)
   
    
    RtempPWMDuty = ((((RpTerm + RiTerm) * 100)/59)/10); 
    MOTOR_R_DUTY = RtempPWMDuty;
  
  
  
  }
void LMotorPI()
  {
    DisableInterrupts;
    LMotorLocalPeriod = LMotorPeriod;
    EnableInterrupts;
    
      
    LEncoderFrequency = ECLOCK / (LMotorLocalPeriod * MOTORPRESCALER);
    LEncoderSpeed = ((LEncoderFrequency * 100) / NUMENCODERVANES);
    LMotorFrequency = ((LEncoderSpeed * 100) / GEARRATIO);
    LMotorSpeed = ((PI * LWHEELDIAMETER * LMotorFrequency)/100000);
    
   Lerror = (LMotorSetPoint - LMotorSpeed);
   
   LpTerm = (LpGain * Lerror);
   
   LiState = (LiState + Lerror);
   if (LiState > LiMax)
    {
      LiState = LiMax;     
    }
   else if ( LiState < LiMin)
    {
      LiState = LiMin; 
    }
   LiTerm = (LiGain * LiState); 
   //dTerm = (dGain * (position - dState));
   //dState = position; 
   
   //assign duty with (pTerm+iTerm - dTerm)
   
    
    LtempPWMDuty = ((((LpTerm + LiTerm) * 100)/59)/10); 
    MOTOR_L_DUTY = LtempPWMDuty;
  
  
  
  
  
  }