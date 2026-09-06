/*======================================================================

		NUE_PSK FIR

		John H. Fisher	K5JHF
		Began On:		09/24/2006
		Current Date:	04/23/2009
		Mods by W8NUE:	04/23/2009

======================================================================*/

#include	"NUE_PSK_Header.h"
#include	"dsp.h"

/*======================================================================*/

void Init_FIR_PSK	( void )	{ 

	FIRStructInit (	&Filter_I1,
					35,
					Dec4LPCoef,
					__builtin_psvpage(Dec4LPCoef),
					Filter_delay_I1 );

/*======================================================================*/

	FIRStructInit (	&Filter_I2,
					35,
					Dec4LPCoef,
					__builtin_psvpage(Dec4LPCoef),
					Filter_delay_I2 );

/*======================================================================*/

	FIRStructInit (	&Filter_Q1,
					35,
					Dec4LPCoef,
					__builtin_psvpage(Dec4LPCoef),
					Filter_delay_Q1 );

/*======================================================================*/

	FIRStructInit (	&Filter_Q2,
					35,
					Dec4LPCoef,
					__builtin_psvpage(Dec4LPCoef),
					Filter_delay_Q2 );

/*======================================================================*/

	FIRStructInit (	&Filter_I3,
					65,
					FIRBitCoef,
					__builtin_psvpage(FIRBitCoef),
					Filter_delay_I3 );

/*======================================================================*/

	FIRStructInit (	&Filter_Q3,
					65,
					FIRBitCoef,
					__builtin_psvpage(FIRBitCoef),
					Filter_delay_Q3 );

/*======================================================================*/

	FIRStructInit (	&Filter_I4,
					65,
					FIRFreqCoef,
					__builtin_psvpage(FIRFreqCoef),
					Filter_delay_I4 );

/*======================================================================*/

	FIRStructInit (	&Filter_Q4,
					65,
					FIRFreqCoef,
					__builtin_psvpage(FIRFreqCoef),
					Filter_delay_Q4 );
	}
/*======================================================================*/
void Init_FIR_RTTY ( void ) {

	FIRStructInit (	&Filter_I3,
					65,
					RTTY1Coef,
					__builtin_psvpage(RTTY1Coef),
					Filter_delay_I3 );

/*======================================================================*/

	FIRStructInit (	&Filter_Q3,
					65,
					RTTY1Coef,
					__builtin_psvpage(RTTY1Coef),
					Filter_delay_Q3 );

/*======================================================================*/

	FIRStructInit (	&Filter_I4,
					65,
					RTTY2Coef,
					__builtin_psvpage(RTTY2Coef),
					Filter_delay_I4 );

/*======================================================================*/

	FIRStructInit (	&Filter_Q4,
					65,
					RTTY2Coef,
					__builtin_psvpage(RTTY2Coef),
					Filter_delay_Q4 );
	}
/*======================================================================*/
