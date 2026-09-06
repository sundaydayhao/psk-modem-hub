/*======================================================================

		NUE_PSK_LCD_single_1

		Milton E. Cram	W8NUE
		George L. Heron N2APB
		Began On:		09/24/2006
		Current Date:	7/28/2008

========================================================================
	                  LCD Routines
======================================================================*/

#include	"NUE_PSK_Header.h"
#include	"Char_Gen.h"
#include    <math.h>
 
/*======================================================================*/

	
	#define	LCD_D0		LATEbits.LATE0
	#define	LCD_D1		LATEbits.LATE1
	#define	LCD_D2		LATEbits.LATE2
	#define	LCD_D3		LATEbits.LATE3
	#define	LCD_D4		LATEbits.LATE4
	#define	LCD_D5		LATEbits.LATE5
	#define	LCD_D6		LATEbits.LATE6
	#define LCD_D7		LATEbits.LATE7
	#define	LCD_Data	LATE

	#define	LCD_CS1		LATDbits.LATD2
	#define	LCD_CS2		LATDbits.LATD3
	#define	LCD_RW		LATDbits.LATD4
	#define	LCD_DI		LATDbits.LATD5
	#define	LCD_E		LATDbits.LATD6

	#define TrisE0		TRISEbits.TRISE0
	#define TrisE1		TRISEbits.TRISE1
	#define TrisE2		TRISEbits.TRISE2
	#define TrisE3		TRISEbits.TRISE3
	#define TrisE4		TRISEbits.TRISE4
	#define TrisE5		TRISEbits.TRISE5
	#define TrisE6		TRISEbits.TRISE6
	#define TrisE7		TRISEbits.TRISE7

	#define LEFT	0
	#define RIGHT	1
	#define	BOTH	2
	

/*======================================================================*/

void PortE_In ( void ) {

	TrisE0 = 1;
	TrisE1 = 1;
	TrisE2 = 1;
	TrisE3 = 1;
	TrisE4 = 1;
	TrisE5 = 1;
	TrisE6 = 1;
	TrisE7 = 1; 
}

/*======================================================================*/

void PortE_Out ( void ) {

	TrisE0 = 0;
	TrisE1 = 0;
	TrisE2 = 0;
	TrisE3 = 0;
	TrisE4 = 0;
	TrisE5 = 0;
	TrisE6 = 0;
	TrisE7 = 0; 
}

/*======================================================================*/

void Delay_150ns(void)	{

	__asm__ ("repeat #6");
	__asm__	("nop");
	
	}

/*======================================================================*/

void Delay_300ns(void)	{

	__asm__ ("repeat #12");
	__asm__	("nop");
	
	}

/*======================================================================*/

void Delay_100us(void)	{

	__asm__ ("repeat #4000");
	__asm__	("nop");
	
	}

/*======================================================================*/

void Delay_25ms(void)	{
	
	int	i;
	
	for ( i = 0; i < 250 ; i++ ) Delay_100us();
	
	}

/*======================================================================*/

void Delay_5ms(void)	{
/*  used only in I2C routine */	
	int	i;
	
	for ( i = 0; i < 50 ; i++ ) Delay_100us();
	
	}

/*======================================================================*/

	void Clear_Display ( uchar StartLine ) {

			extern uchar Yloc;
			int	i, j ;
			/* set display start line to 0 (Z address) */
			Write_lcd_cntrl_regs (BOTH,0xC0);
			/* loop over line counter j */
			for (j=StartLine; j<8; j++) {
				/* set X (row) register */
				Write_lcd_cntrl_regs (BOTH,0xB8|j);
				/* set Y register to leftmost column */
				Write_lcd_cntrl_regs (BOTH,0x40);
					/* clear the current line */
					Yloc = 0;
					for (i=0; i<64; i++) {
						Write_Display_Data (0x0, 0);
					}
					for (i=0; i<64; i++) {
						Write_Display_Data (0x0, 0);
					}
			}
			Yloc = 0;
	}			
/*======================================================================*/

void Init_LCD ( void )	{

		LCD_D0	=	0;
		LCD_D1	=	0;
		LCD_D2	=	0;
		LCD_D3	=	0;
		LCD_D4	=	0;
		LCD_D5	=	0;
		LCD_D6	=	0;
		LCD_D7	=	0;
		LCD_CS1	=	1;
		LCD_CS2	=	1;
		LCD_RW	=	1;
		LCD_DI	=	1;
		LCD_E	=	0;

		BackLight_tris = 0;

		TRISEbits.TRISE0	=	0;	/*	LCD Data 0	*/
		TRISEbits.TRISE1	=	0;	/*	LCD Data 1	*/
		TRISEbits.TRISE2	=	0;	/*	LCD Data 2	*/
		TRISEbits.TRISE3	=	0;	/*	LCD Data 3	*/
		TRISEbits.TRISE4	=	0;	/*	LCD Data 4	*/
		TRISEbits.TRISE5	=	0;	/*	LCD Data 5	*/
		TRISEbits.TRISE6	=	0;	/*	LCD Data 6	*/
		TRISEbits.TRISE7	=	0;	/*	LCD Data 7	*/

									/*  vers 9  -> vers 11  */
		TRISDbits.TRISD0	=	0;	/*	LCD CS1 -> Spare 	*/
		TRISDbits.TRISD1	=	0;	/*	LCD CS2 -> Piezo 	*/
		TRISDbits.TRISD2	=	0;	/*	LCD RW -> LCD CS1	*/
		TRISDbits.TRISD3	=	0;	/*	LCD DI -> LCD CS2	*/
		TRISDbits.TRISD4	=	0;	/*	LCD E -> LCD RW 	*/
		TRISDbits.TRISD5	=	0;	/*	unused -> LCD DI	*/
		TRISDbits.TRISD6	=	0;	/*	unused -> LCD E 	*/		
		

		Write_lcd_cntrl_regs (BOTH,0x3F);		/* Turn display on */

		Clear_Display ( 0 );			/* Clear the single graphic display */
		BackLight = 1;		/* Turn on the backlight */
		}
		
/*======================================================================*/

void Clear_Text_Display ( void ) {
		Reset_LCD_buffer ();
		Clear_Display (4);
		Set_Display_XYAddr(7,0);
	}		
/*======================================================================*/

void Home_LCD_Display ( )	{
		/* set to leftmost column */
		Write_lcd_cntrl_regs (BOTH,0x40);				/* Set Y register to leftmost column */
		/* set to top row */
		Write_lcd_cntrl_regs (BOTH,0xB8);				/* Set X register to top row */

		}

/*======================================================================*/

void Reset_LCD_buffer ( void ) {
	extern unsigned char __attribute__ ((aligned)) LCD_buffer[];
	int i;
	extern int count;
	extern int char_count;

		for ( i=0; i<60; i++) {
			LCD_buffer[i] = ' ';
			}
		LCD_buffer[60] = '\0';
		count = 0;
		char_count = 0;

	}
/*======================================================================*/

void Write_String_Positioned_LCD1_Display ( uchar xposition, uchar yposition, char *string )	{

	char temp;
	Set_Display_XYAddr ( xposition, yposition );
	while ( ( temp = *string++ ) ) Write_Char ( temp, 0 ); 	
	}

/*======================================================================*/

void Write_Integer_Formatted_LCD1_Display ( uchar xposition, uchar yposition, float *input_number )	{
	
	char buffer[6] = "    ";

	int	index = 3, number;
	number	=	(int) *input_number;
	while ( number > 0 ) {
		buffer [ index-- ] = number % 10 + 0x30;
		number /= 10;
		Write_String_Positioned_LCD1_Display ( xposition, yposition, buffer );	
		}
	}

/*======================================================================*/

void Buffered_LCD_Write ( uchar dat ) {
	extern uchar __attribute__ ((aligned)) LCD_buffer[];
	extern int count;		/* position within LCD buffer */ 
	extern int char_count; 	/* position on bottom line */
	extern int byte_count;	/* USB byte count */
	int i,j;
		if (dat == 8) { 	/* got a backspace */
			count--;
			Write_Char (0x20, 0);
			char_count--;
			if (char_count<0) { 
				Set_Display_XYAddr (6,6*(char_count + 20));
				Display_Text_Cursor ();
				Set_Display_XYAddr (6,6*(char_count + 20));
			}
			else {
				Set_Display_XYAddr (7,6*char_count);
				Display_Text_Cursor ();
				Set_Display_XYAddr (7,6*char_count);
			}
		}
		else {
			Write_Char ( dat, 0 );
			Display_Text_Cursor ();

			if (Status.LOGGING) {
				PutUartChar ( dat, CHK );
			}

			/* don't worry about buffer, if on bottom line */
			if (count >= 0) LCD_buffer[count] = dat;
			count++;
			count %= 60;
			char_count++;
			if (char_count == 0) Set_Display_XYAddr(7,0 ) ;
			if (char_count == 20) {
				char_count = 0;
				Clear_Display (4);
				if (Status.LOGGING) Display_Log (); /* refresh logging annunciator */
					for (j=0; j<3; j++) {
						Set_Display_XYAddr (j+4,0);
						for (i = 0 ; i < 20 ; i++) {
							Write_Char (LCD_buffer[(count+i+j*20) % 60], 0);
						}
 					}
				/* reset to beginning of line */
				Set_Display_XYAddr ( 7,0 ) ;
				Display_Text_Cursor ();
			}
		}
	}

/*======================================================================*/

void Set_Display_ON_OFF ( char data) {
		Write_lcd_cntrl_regs (BOTH,0x3E|data);
	}

/*======================================================================*/

void	Set_Display_Xaddr ( uchar Xpos ) {
		extern uchar Xold;
		if (Xpos>3) Xold = Xpos;
		Write_lcd_cntrl_regs (BOTH,0xB8|Xpos);
	}

/*======================================================================*/
void	Set_Display_Yaddr ( uchar Ypos ) {
		extern uchar Yloc;
		if (Ypos > 0x3F) Write_lcd_cntrl_regs (RIGHT,0x40|(Ypos&0x3F));
		else Write_lcd_cntrl_regs (LEFT,0x40|(Ypos&0x3F));
		Yloc = Ypos;
	}
/*======================================================================*/

void	Set_Display_XYAddr ( uchar Xpos, uchar Ypos ) {
		Set_Display_Xaddr ( Xpos );
		Set_Display_Yaddr ( Ypos );
	}

/*======================================================================*/

void	Write_Display_Data ( uchar data, uchar rev ) {
		/* Yloc is the absolute position for data writes */		
		extern uchar Yloc;
		if (Yloc == 0x40) { /* must write to right controller */
			Set_Display_Yaddr (Yloc);
			LCD_CS1 = 1;
			LCD_CS2 = 0;
		}
		if (Yloc == 0x80) {	/* must write to left controller */
			Yloc = 0;
			Set_Display_Yaddr (Yloc);
			LCD_CS1 = 0;
			LCD_CS2 = 1;
		} 
		/* check for busy */
		LCD_DI = 0;
		LCD_RW = 1;
		PortE_In();
		LCD_E = 1;
		Delay_300ns();
		while (PORTE&0x80); /* wait for bit to clear */
		Delay_150ns();
		LCD_E = 0;
		Delay_300ns();
		/* now output data */
		LCD_RW = 0;
		LCD_DI = 1;
		Delay_150ns();
		LCD_E = 1;
		if (rev) {
			LCD_Data = ~data;
		}
		else {
			LCD_Data = data;
		}
		PortE_Out();
		Delay_300ns();
		LCD_E = 0;
		Yloc++;		/* increment position marker */
}

/*======================================================================*/

void	Write_lcd_cntrl_regs ( uchar regs, uchar data ) {
		/* test each controller for busy */
		LCD_DI = 0;
		LCD_RW = 1;
		PortE_In();
		if ((regs == LEFT)|(regs == BOTH)) {
			LCD_CS1 = 0; /* check left controller */
			LCD_CS2 = 1;
			Delay_150ns();
			LCD_E = 1;
			Delay_300ns();
			while (PORTE&0x80); /* wait for busy bit to clear */
			Delay_150ns();
			LCD_E = 0;
			Delay_300ns();
		}		
		if ((regs == RIGHT)|(regs == BOTH)) {		
			LCD_CS1 = 1;
			LCD_CS2 = 0; /* check right controller */
			Delay_150ns();
			LCD_E = 1;
			Delay_300ns();
			while (PORTE&0x80); /* wait for busy bit to clear */
			Delay_150ns();
			LCD_E = 0;
			Delay_300ns();
		}
		/* now write data to one or both controllers */
		switch (regs) {
			case LEFT:
				LCD_CS1 = 0;
				LCD_CS2 = 1;
				break;
			case RIGHT:
				LCD_CS1 = 1;
				LCD_CS2 = 0;
				break;
			case BOTH:
				LCD_CS1 = 0;
				LCD_CS2 = 0;
				break;
		}
		LCD_RW = 0;
		Delay_150ns();
		LCD_E = 1;
		LCD_Data = data; /* load data to be written */
		PortE_Out();
		Delay_300ns();
		LCD_E = 0;		
}
/*======================================================================*/

unsigned char Read_Display_Data ( uchar CNTRLR ) {

	}

/*======================================================================*/

void	Display_Cursor ( int cur_pos ) {
		int i,ypos;
		extern char Cursor_Slice[];
		extern uchar Xold,Yold,Yloc;
		Yold = Yloc;
		/* Clear_Cursor (); */
		Set_Display_Xaddr ( 0x3 );
		for (i=0;i<5;i++) {
			ypos = cur_pos + i - 2;
			if ((ypos >=0) && (ypos < 128)) {
				Set_Display_Yaddr ( (unsigned char) ypos );
				Write_Display_Data (Cursor_Slice[i], 0);
			}
		}
		Set_Display_XYAddr (Xold,Yold);
	}

/*======================================================================*/

void	Display_RTTYCursor ( void ) {
		extern double NCO_Frequency;
		extern int RTTY_Shift;
		extern int cursor_position;
		int P1,P2;
		
		P1 = cursor_position - (int)((RTTY_Shift/2)/DELTAFREQ + .5);
		P2 = cursor_position + (int)((RTTY_Shift/2)/DELTAFREQ + .5);
		Display_Cursor(P1);
		Display_Cursor(P2);
	}

/*======================================================================*/


void	Clear_Cursor ( void ) {
		int i;
		extern uchar Xold,Yold,Yloc;
		Yold = Yloc;
		Set_Display_XYAddr ( 0x3,0 );
		for (i=0;i<0x80;i++) Write_Display_Data (0x1, 0);
		Set_Display_XYAddr (Xold,Yold);
	}

/*======================================================================*/

void	Display_FFT ( void ) {
		int i,j,tempdata;
		extern unsigned char PIXDATA[];
		extern uint FLOB[];
		extern uchar Xold,Yold,Yloc;
		Yold = Yloc;
		for (i=0;i<3;i++) {
			Set_Display_Xaddr ( 2-i );
			Set_Display_Yaddr ( 0x0 );
			for (j=0;j<64;j++) {
				tempdata = FLOB[j] - i*8;
				if (tempdata<0) tempdata = 0;
				if (tempdata>8) Write_Display_Data ( 0xFF, 0 );
				else Write_Display_Data ( PIXDATA[ (unsigned char) tempdata ], 0);
			}
			Set_Display_Yaddr ( 0x40 );
			for (j=0;j<64;j++) {
				tempdata = FLOB[j+0x40] - i*8;
				if (tempdata<0) tempdata = 0;
				if (tempdata>8) Write_Display_Data ( 0xFF, 0 );
				else Write_Display_Data ( PIXDATA[ (unsigned char) tempdata] ,0);
			}
		}
		Set_Display_XYAddr (Xold,Yold);
	}
/*======================================================================*/

void	Write_Char (unsigned char byte, uchar rev) {
		int i;
		if (byte ==13) byte = 23;
		byte = (byte > 0x7F) ? 0 : byte;
		for (i=0;i<5;i++) Write_Display_Data (Char_Gen[byte][i], rev);
		Write_Display_Data(0x0, rev);
	}

/*======================================================================*/

void	Set_Display_TX ( void ) {
		Set_Display_XYAddr (1,5);
		Write_Char ('T', 0);
		Write_Char ('X', 0);
	}

/*======================================================================*/

void 	Display_Text_Cursor ( void ) {
		int i;
		extern uchar Yloc;
		for (i=0;i<6;i++) Write_Display_Data (0x80, 0);
		/*	 each write increments Yloc */
		Yloc -= 6;
		Set_Display_Yaddr(Yloc); 
	}

/*======================================================================*/

void	Display_Tune ( void ) {
		Set_Display_XYAddr (1,5);
		Write_Char ('T', 0);
		Write_Char ('U', 0);
		Write_Char ('N', 0);
		Write_Char ('E', 0);
	}

/*======================================================================*/

void	Display_Log (void ) {
		extern int char_count;
		Set_Display_XYAddr (4,121);
		Write_Display_Data (0x0, 1);
		Write_Char ('R', 1);
		Set_Display_XYAddr (5,121);
		Write_Display_Data (0x0, 1);
		Write_Char ('E', 1);
		Set_Display_XYAddr (6,121);
		Write_Display_Data (0x0, 1);
		Write_Char ('C', 1);
		Set_Display_XYAddr (7,121);
		Write_Display_Data (0x0, 1);
		Write_Char (' ', 1);
		Set_Display_XYAddr (7,6*char_count);
}	

/*======================================================================*/

void	Display_Bcn (void ) {
		extern int char_count;
		Set_Display_XYAddr (4,121);
		Write_Display_Data (0x0, 1);
		Write_Char ('B', 1);
		Set_Display_XYAddr (5,121);
		Write_Display_Data (0x0, 1);
		Write_Char ('C', 1);
		Set_Display_XYAddr (6,121);
		Write_Display_Data (0x0, 1);
		Write_Char ('N', 1);
		Set_Display_XYAddr (7,121);
		Write_Display_Data (0x0, 1);
		Write_Char (' ', 1);
		Set_Display_XYAddr (7,6*char_count);
}	

/*======================================================================*/

void	Display_Beacon_Count ( void ) {
		char buffer[20];
		extern unsigned int Beacon_Count;
		sprintf (buffer, "BeaconCount= %d", Beacon_Count);
		Write_String_Positioned_LCD1_Display ( 0,0, buffer );
		Set_Display_XYAddr (7,0);

}
/*======================================================================*/

void	Clear_Log ( void ) {
		extern int char_count;
		int i;
		for (i=0;i<4;i++) {
			Set_Display_XYAddr (4+i,121);
			Write_Display_Data (0x0, 0);
			Write_Char (' ', 0);
		}
		Set_Display_XYAddr (7,6*char_count);
}	

/*======================================================================*/

void	Display_RTTY_Info ( void ) {
		extern uchar Xold,Yold,Yloc;
		Set_Display_XYAddr (0,108);
		if (Rtty.FIGURES) Write_Char('F',0);
		else Write_Char('L',0);
		Set_Display_XYAddr (0,120);		
		if (Rtty.REVERSE) Write_Char('R',0);
		else Write_Char('N',0);
		Set_Display_XYAddr(Xold,Yold);
}
/*======================================================================*/
