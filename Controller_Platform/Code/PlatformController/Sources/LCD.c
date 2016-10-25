#include "LCD.h"
#include "Macros.h"

void msDelay( int k )
{
  int ix;			/* counter for number of ms delayed */

 	TC3 	= TCNT + (125*8);		/* preset TC0 for first OC event */
	TIOS   |= TIOS_IOS3_MASK;	/* ready to go - enable TC0 as OC */

	for (ix = 0; ix < k; ix++) {	/* for number of ms to delay? */
		while(!(TFLG1 & TFLG1_C3F_MASK));	/* wait for OC event */
          	TC3 += (125*8);		/* rearm the OC register, this clears TFLG1 */
   	}

   	TIOS  &= LOW(~TIOS_IOS3_MASK);  	/* all done ? turn-off OC on TC0 */
}


void msDelay_init(){
  TSCR2 	= 0x06; 	/* set prescaler to 64, no interrupt */
 	TSCR1 	= 0xB0; 	/* enable TCNT, fast timer flag clear, freeze */  
}

void LCDputc(unsigned char Character){
  LCDdata(Character);
}

void LCDputs(char *String){
  while(*String){
    LCDputc(*String++);
  }
}

void LCDprintf(char *fmt, ...){
  va_list args;
  
  char buffer[MAX_LCD_BUFSIZ];
  
  va_start(args, fmt);
  vsprintf(buffer,fmt,args);
  va_end(args);
  
  LCDputs(buffer);
} 

/********* LCD INITIALIZE ********/
void LCDinit(){
  LCD_PORT_DDR = (LCD_BUS_BITS | LCD_RS | LCD_RW | LCD_E);
  LCD_E_HI;
  LCD_BUS( 0x03 );      // wake up display & sync
  LCD_E_LO;
    
  msDelay( 5 );

  LCD_E_HI;
  LCD_BUS( 0x03 );      // wake up display & sync
  LCD_E_LO;

  msDelay( 1 );   
    
  LCD_E_HI;
  LCD_BUS( 0x03 );      // wake up display & sync
  LCD_E_LO;
    
  LCD_E_HI;
  LCD_BUS( 0x02 );      // wake up display & sync - go to 4-bit mode
  LCD_E_LO;

	msDelay( 2 );

// now that we're sync'd and in 4-bit mode, issue commands to configure the display
  LCDcmd( LCD_CMD_FUNCTION | LCD_FUNCTION_4BIT | LCD_FUNCTION_2LINES | LCD_FUNCTION_5X8FONT );
  LCDcmd( LCD_CMD_DISPLAY | LCD_DISPLAY_OFF );
  LCDclear();
  LCDcmd( LCD_CMD_ENTRY | LCD_ENTRY_MOVE_CURSOR | LCD_ENTRY_INC );
  LCDcmd( LCD_CMD_DISPLAY | LCD_DISPLAY_ON | LCD_DISPLAY_NOCURSOR | LCD_DISPLAY_NOBLINK );
}


/******** LCD CLEAR *******/
void LCDclear(){
  LCDcmd(LCD_CMD_CLEAR);
}

void LCDcmd(unsigned char Value){
  LCD_E_LO;
  LCD_CMD;
  CLR_BITS(LCD_PORT,LCD_RW);
  LCD_E_HI;
  LCD_BUS(HI_NYBBLE(Value));
  LCD_E_LO;
  LCD_E_HI;
  LCD_BUS(LO_NYBBLE(Value));
  LCD_E_LO;
  msDelay( 2 );  
}

void LCDdata(unsigned char Value){
  LCD_E_LO;
  SET_BITS(LCD_PORT,LCD_RS);
  CLR_BITS(LCD_PORT,LCD_RW);
  LCD_E_HI;
  LCD_BUS(HI_NYBBLE(Value));
  LCD_E_LO;
  LCD_E_HI;
  LCD_BUS(LO_NYBBLE(Value));
  LCD_E_LO;
  msDelay( 2 );   
}
