/*======================================================================

		NUE_PSK init clock

		John H. Fisher	K5JHF
		Began On:		09/24/2006
		Current Date:	09/27/2006

======================================================================*/

#include	"NUE_PSK_Header.h"
/*=====================================================================*/		

void	init_clock ( void )	{

/*----- PLLFBD Bits ---------------------------------------------------*/

	PLLFBDbits.PLLDIV	=	30;
	Nop()

/*		Setup the PLL for 80 MHz = 40 MIPS                             */
/*----- CLKDIV Bits ---------------------------------------------------*/

	CLKDIVbits.PLLPRE	=	0;
	Nop()
	CLKDIVbits.PLLPOST	=	0;
	Nop()
	CLKDIVbits.FRCDIV	=	4;
	Nop()
	CLKDIVbits.DOZEN	=	0;
	Nop()
	CLKDIVbits.DOZE		=	0;
	Nop()
	CLKDIVbits.ROI		=	0;
	Nop()
	

	while ( !OSCCONbits.LOCK );
	Nop(); 
	}
	
/*=====================================================================*/		
