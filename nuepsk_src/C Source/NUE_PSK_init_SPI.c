/*======================================================================

		NUE_PSK init SPI

		John H. Fisher	K5JHF
		Began On:		09/24/2006
		Current Date:	09/27/2006

======================================================================*/

#include	"NUE_PSK_Header.h"

/*=====================================================================*/		

void 	init_SPI ( void ) {

/*=====================================================================*/		

	MCP6S21_CS1_tris	=	0;	
	Nop()
	MCP4922_CS1_tris	=	0; 
	Nop()
	MCP6S21_CS1			=	1;	
	Nop()
	MCP4922_CS1			=	1; 
	Nop()
	
/*=====================================================================*/		
/*----- SPI1CON1 Bits -----------------------------------------------------*/

	SPI1CON1bits.PPRE	=	2;
	Nop()
	SPI1CON1bits.SPRE	=	7;
	Nop()
	SPI1CON1bits.MSTEN	=	1;
	Nop()
	SPI1CON1bits.CKP	=	0;
	Nop()
	SPI1CON1bits.SSEN	=	0;
	Nop()
	SPI1CON1bits.CKE	=	1;
	Nop()
	SPI1CON1bits.SMP	=	0;
	Nop()
	SPI1CON1bits.MODE16	=	1;
	Nop()
	SPI1CON1bits.DISSDO	=	0;
	Nop()
	SPI1CON1bits.DISSCK	=	0;
	Nop()

/*=====================================================================*/		
/*----- SPI1CON2 Bits -----------------------------------------------------*/

	SPI1CON2bits.FRMDLY	=	1;
	Nop()
	SPI1CON2bits.FRMPOL	=	1;
	Nop()
	SPI1CON2bits.SPIFSD	=	0;
	Nop()
	SPI1CON2bits.FRMEN	=	0;
	Nop()

/*=====================================================================*/		
/*----- SPI1STAT Bits -----------------------------------------------------*/

	SPI1STATbits.SPISIDL	=	0;
	Nop()
	SPI1STATbits.SPIEN		=	1;
	Nop()
	SPI1STATbits.SPIROV		=	0;
	Nop()

/*=====================================================================*/		
/*----- SPI2CON1 Bits -----------------------------------------------------*/

	SPI2CON1bits.PPRE	=	2;
	Nop()
	SPI2CON1bits.SPRE	=	7;
	Nop()
	SPI2CON1bits.MSTEN	=	1;
	Nop()
	SPI2CON1bits.CKP	=	0;
	Nop()
	SPI2CON1bits.SSEN	=	0;
	Nop()
	SPI2CON1bits.CKE	=	1;
	Nop()
	SPI2CON1bits.SMP	=	0;
	Nop()
	SPI2CON1bits.MODE16	=	1;
	Nop()
	SPI2CON1bits.DISSDO	=	0;
	Nop()
	SPI2CON1bits.DISSCK	=	0;
	Nop()

/*=====================================================================*/		
/*----- SPI2CON2 Bits -----------------------------------------------------*/

	SPI2CON2bits.FRMDLY	=	1;
	Nop()
	SPI2CON2bits.FRMPOL	=	1;
	Nop()
	SPI2CON2bits.SPIFSD	=	0;
	Nop()
	SPI2CON2bits.FRMEN	=	0;
	Nop()

/*=====================================================================*/		
/*---- SPI2STAT Bits -----------------------------------------------------*/

	SPI2STATbits.SPISIDL	=	0;
	Nop()
	SPI2STATbits.SPIEN		=	1;
	Nop()
	SPI2STATbits.SPIROV		=	0;
	Nop()

	}

/*=====================================================================*/		
