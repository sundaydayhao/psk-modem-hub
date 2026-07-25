/*======================================================================

		NUE_PSK TIM1 init

		John H. Fisher	K5JHF
		Began On:		09/24/2006
		Current Date:	09/27/2006

======================================================================*/

#include	"NUE_PSK_Header.h"

/*=====================================================================*/		

void init_TIM1 ( void )	{

/* T1CON: Timer 1 Control Register */

	T1CONbits.TCS	=	0;
	Nop()
	T1CONbits.TSYNC	=	0;
	Nop()
	T1CONbits.TCKPS	=	0;
	Nop()
	T1CONbits.TGATE	=	0;
	Nop()
	T1CONbits.TSIDL	=	0;

/* T1 Interrupt Priority 4 */
	IPC0bits.T1IP0 = 0;
	IPC0bits.T1IP1 = 0;
	IPC0bits.T1IP2 = 1;

/* PR1: Timer 1 Period Register */

	PR1				=	(int)(40000000.0/Sample_Frequency)-1;

	T1CONbits.TON	=	1;

	/* reset Timer 1 interrupt flag */
 	IFS0bits.T1IF = 0;
 	
	/* enable Timer 1 interrupt */
 	IEC0bits.T1IE = 1;

	}

/*======================================================================*/
