#ifndef LCDHEADER_H
#define LCDHEADER_H
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <stdio.h>


//#define   LOW(arg)                    (arg&(0xFF))
//#define   SET_BITS(port,mask)         (port)|=(mask)
//#define   CLR_BITS(port,mask)         (port)&=LOW(~(mask))
//#define   FLIP_BITS(port,mask)        (port)^=(mask) 
//#define   FORCE_BITS(port,mask,phase) (port)=((port)&LOW(~(mask))|(phase)&(mask))
#define   ENABLE_PWM_CHNL(chnl)       SET_BITS(PWME,1<<(chnl))
//LCD Macros
#define   LCD_PORT          PORTA
#define   LCD_PORT_DDR      DDRA
#define   LCD_E             PORTA_BIT4_MASK       
#define   LCD_RS            PORTA_BIT5_MASK
#define   LCD_RW            PORTA_BIT6_MASK
#define   LCD_BUS_BITS      0x0F
#define   LO_NYBBLE(value)  ((value)&0x0F)
#define   HI_NYBBLE(value)  (((value)>>4)&0x0F)
#define   LCD_E_HI          SET_BITS(LCD_PORT,LCD_E)
#define   LCD_E_LO          CLR_BITS(LCD_PORT,LCD_E)
#define   LCD_CMD           CLR_BITS(LCD_PORT,LCD_RS)
#define   LCD_DATA          SET_BITS(LCD_PORT,LCD_RS)
#define   LCD_BUS(value)    FORCE_BITS(LCD_PORT,LCD_BUS_BITS,value)
//CLEAR FUNCTION
#define	  LCD_CMD_CLEAR			    0b00000001
//RETURN FUNCTION
#define	  LCD_CMD_RETURN		    0b00000010
//ENTRY FUNCTION
#define   LCD_CMD_ENTRY			    0b00000100
#define   LCD_ENTRY_MOVE_CURSOR	0b00000000
#define   LCD_ENTRY_INC			    0b00000010
//DISPLAY FUNCTION
#define   LCD_CMD_DISPLAY 		  0b00001000
#define   LCD_DISPLAY_OFF  		  0b00000000
#define   LCD_DISPLAY_ON		    0b00000100
#define   LCD_DISPLAY_NOCURSOR	0b00000000
#define   LCD_DISPLAY_CURSOR	  0b00000010
#define   LCD_DISPLAY_NOBLINK 	0b00000000
#define   LCD_DISPLAY_BLINK    	0b00000001
//SHIFT FUNCTION
#define	  LCD_CMD_C_D_SHIFT		  0b00010000
#define   LCD_SHIFT_DISPLAY		  0b00001000
#define   LCD_MOVE_CURSOR		    0b00000000
#define   LCD_C_D_RIGHT			    0b00000100
#define   LCD_C_D_LEFT			    0b00000000

#define   LCD_CMD_FUNCTION      0b00100000 
#define   LCD_FUNCTION_8BIT	    0b00010000
#define   LCD_FUNCTION_4BIT 	  0b00000000
#define   LCD_FUNCTION_2LINES 	0b00001000
#define   LCD_FUNCTION_1LINE 	  0b00000000
#define   LCD_FUNCTION_5X10FONT	0b00000100
#define   LCD_FUNCTION_5X8FONT	0b00000000

#define   MAX_LCD_BUFSIZ        16

//PROTOTYPES
void msDelay (int);
void msDelay_init(void);
void LCDcmd(unsigned char);
void LCDdata(unsigned char);
void LCDclear(void);
void LCDinit(void);
void LCDputc(unsigned char);
void LCDputs(char*);
void LCDprintf(char *, ...);
#endif
