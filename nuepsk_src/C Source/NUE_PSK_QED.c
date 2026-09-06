/*======================================================================

		NUE_PSK QED

		Milton E. Cram	W8NUE
		Began On:		11/16/2007
		Current Date:	11/16/2007
		Based on code from John Fisher, K5JHF

======================================================================*/

#include	"NUE_PSK_Header.h"

/*=====================================================================*/		

void 	Init_QED ( void ) {

	QEICONbits.UPDN_SRC	=	0;
	Nop()
	QEICONbits.TQCS		=	0;
	Nop()
	QEICONbits.POSRES	=	0;
	Nop()
	QEICONbits.TQCKPS	=	0;
	Nop()
	QEICONbits.TQGATE	=	0;
	Nop()
	QEICONbits.PCDOUT	=	0;
	Nop()
	QEICONbits.SWPAB	=	0;
	Nop()
	QEICONbits.QEIM		=	7; 
	Nop()
	QEICONbits.QEISIDL	=	0;
	Nop()
	QEICONbits.CNTERR	=	0;

	DFLTCONbits.QECK	=	7;
	Nop()
	DFLTCONbits.QEOUT	=	1;
	Nop()
	DFLTCONbits.CEID	=	1;
	Nop()
	DFLTCONbits.IMV		=	0;

	POSCNT				=	100;

	MAXCNT				=	0xFFFF;

	Button1_tris		=	1;  /* as input */

	CNPU1bits.CN11PUE	=	1; /* Button1 */
	Nop()
	CNPU1bits.CN6PUE	=	1; /* QED_1A */
	Nop()
	CNPU1bits.CN7PUE	=	1; /* QED_1B */

	IFS3bits.QEIIF		=	0;
/* 	don't enable QE interrupts */	
	IEC3bits.QEIIE		=	0;	/* set to 1 to enable QE interrupts */

	}	

/*=====================================================================*/		
