#ifndef DCMOTORHEADER_H
#define DCMOTORHEADER_H

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
//DC motor timer channel defines
#define PI 314
#define GEARRATIO 225
#define NUMENCODERVANES 27
#define ECLOCK 8000000
#define MOTORPRESCALER 8  // divide by 8 prescaler
#define RWHEELDIAMETER 53
#define LWHEELDIAMETER 53

#define SENSORGAIN 613
#define FEEDBACK_SCALE_FACTOR 16777216  
//Motor Macros
#define   ENABLE_PWM_CHNL(chnl)       SET_BITS(PWME,1<<(chnl))
#define   MOTOR_ENABLE_PORT           PORTB
#define   MOTOR_ENABLE_PORT_DDR       DDRB
#define   MOTOR_R_FORWARD             0b00000001 
#define   MOTOR_R_BACKWARD            0b00000100
#define   MOTOR_L_FORWARD             0b00001000 
#define   MOTOR_L_BACKWARD            0b00000010
#define   STOP                        0b00000000
#define   MOTOR_R_DUTY                PWMDTY4   //0 - 100
#define   MOTOR_L_DUTY                PWMDTY5   //0 - 100        

#define SET_PERIOD    255 //test value
#define INIT_DUTY     0
#define INIT_PWMCTL   0b00000100
#define INIT_PWMPRCLK 0b00000000 //change back from SA
#define INIT_PWMCLK   0b00000000
#define INIT_PWMPOL   0b00110000
#define INIT_PWMCAE   0b00110000
#define INIT_PWME     0b00110000
#define INIT_PWMCNT   0

#define SETUP_MOTOR_ENABLE_DDR  SET_BITS(MOTOR_ENABLE_PORT_DDR,(MOTOR_R_BACKWARD|MOTOR_R_FORWARD|MOTOR_L_BACKWARD|MOTOR_L_FORWARD))
#define SET_MOTOR_R_FORWARD     FORCE_BITS(MOTOR_ENABLE_PORT,(MOTOR_R_FORWARD|MOTOR_R_BACKWARD),MOTOR_R_FORWARD)
#define SET_MOTOR_R_BACKWARD    FORCE_BITS(MOTOR_ENABLE_PORT,(MOTOR_R_FORWARD|MOTOR_R_BACKWARD),MOTOR_R_BACKWARD)
#define SET_MOTOR_R_STOP        FORCE_BITS(MOTOR_ENABLE_PORT,(MOTOR_R_FORWARD|MOTOR_R_BACKWARD),STOP)
#define SET_MOTOR_L_FORWARD     FORCE_BITS(MOTOR_ENABLE_PORT,(MOTOR_L_FORWARD|MOTOR_L_BACKWARD),MOTOR_L_FORWARD)
#define SET_MOTOR_L_BACKWARD    FORCE_BITS(MOTOR_ENABLE_PORT,(MOTOR_L_FORWARD|MOTOR_L_BACKWARD),MOTOR_L_BACKWARD)
#define SET_MOTOR_L_STOP        FORCE_BITS(MOTOR_ENABLE_PORT,(MOTOR_L_FORWARD|MOTOR_L_BACKWARD),STOP)

#define SETUP_PWMCTL    SET_BITS(PWMCTL,INIT_PWMCTL)
#define SETUP_PWMPRCLK  SET_BITS(PWMPRCLK,INIT_PWMPRCLK)
#define SETUP_PWMCLK    SET_BITS(PWMCLK,INIT_PWMCLK)
#define SETUP_PWMPOL    SET_BITS(PWMPOL,INIT_PWMPOL)
#define SETUP_PWMCAE    SET_BITS(PWMCAE,INIT_PWMCAE)
#define SETUP_PWME      SET_BITS(PWME,INIT_PWME)
#define SETUP_MR_PWMCNT PWMCNT4 = INIT_PWMCNT
#define SETUP_ML_PWMCNT PWMCNT5 = INIT_PWMCNT
#define SETUP_MR_PWMPER PWMPER4 = SET_PERIOD
#define SETUP_ML_PWMPER PWMPER5 = SET_PERIOD
#define SETUP_MR_DUTY   MOTOR_R_DUTY = INIT_DUTY
#define SETUP_ML_DUTY   MOTOR_L_DUTY = INIT_DUTY

//PROTOTYPES
 void DC_Motor_init(void);
 void setDCMotorSpeed(unsigned char *instructPtr);
 long int RMotorEncoder();
 long int LMotorEncoder();
 void RMotorPI();
 void LMotorPI();

//Globals
extern char leftmotorduty;
extern char rightmotorduty;

//Globals for DC motor Speed calculations
extern signed int volatile firstEdgeFlagR;
extern signed int volatile firstEdgeFlagL;
extern signed int volatile secondEdgeFlagR;
extern signed int volatile secondEdgeFlagL;
extern signed int volatile firstEdgeR;
extern signed int volatile firstEdgeL;
extern signed int volatile secondEdgeR;
extern signed int volatile secondEdgeL;
extern signed int volatile RMotorPeriod;
extern signed int volatile LMotorPeriod;
extern signed long int volatile RMotorFrequency;
extern signed long int volatile LMotorFrequency;
extern signed long int volatile RMotorSpeed;
extern signed long int volatile LMotorSpeed;
extern signed int volatile interruptCountR;
extern signed int volatile interruptCountL;
//extern signed char volatile RMotorOverflow;
//extern signed char volatile LMotorOverflow;
extern signed long int volatile REncoderSpeed;
extern signed long int volatile LEncoderSpeed;
extern signed long int volatile REncoderFrequency;
extern signed long int volatile LEncoderFrequency;
extern signed int volatile RMotorSetPoint;
extern signed int volatile LMotorSetPoint;
extern signed long int RMotorLocalPeriod;
extern signed long int LMotorLocalPeriod;

#endif