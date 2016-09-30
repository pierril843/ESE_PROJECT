#ifndef DCMOTORHEADER_H
#define DCMOTORHEADER_H

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

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

#define SET_PERIOD    100
#define INIT_DUTY     0
#define INIT_PWMCTL   0b00000000
#define INIT_PWMPRCLK 0b00000000
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

//Globals
extern char leftmotorduty;
extern char rightmotorduty;
#endif