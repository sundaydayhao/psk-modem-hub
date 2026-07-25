/*======================================================================

		NUE_PSK init ADC

		John H. Fisher	K5JHF
		Began On:		09/24/2006
		Current Date:	09/27/2006

======================================================================*/

#include	"NUE_PSK_Header.h"

/*=====================================================================*/		

void	init_ADC ( void ) {

/*----- AD1CON1 Bits -----------------------------------------------------*/

	AD1CON1bits.DONE	=	0;
	Nop()
	AD1CON1bits.SAMP	=	0;
	Nop()
	AD1CON1bits.ASAM	=	1;
	Nop()
	AD1CON1bits.SIMSAM	=	0;
	Nop()
	AD1CON1bits.SSRC	=	7;
	Nop()
	AD1CON1bits.FORM	=	3;
	Nop()
	AD1CON1bits.AD12B	=	1;
	Nop()
	AD1CON1bits.ADDMABM	=	1;
	Nop()
	AD1CON1bits.ADSIDL	=	0;
	Nop()
	AD1CON1bits.ADON	=	1;

/*=====================================================================*/		
/*----- AD1CON2 Bits -----------------------------------------------------*/

	AD1CON2bits.ALTS	=	0;
	Nop()
	AD1CON2bits.BUFM	=	0;
	Nop()
	AD1CON2bits.SMPI	=	0;
	Nop()
	AD1CON2bits.BUFS	=	0;
	Nop()
	AD1CON2bits.CHPS	=	0;
	Nop()
	AD1CON2bits.CSCNA	=	0;
	Nop()
	AD1CON2bits.VCFG	=	0;

/*=====================================================================*/		
/*----- AD1CON3 Bits -----------------------------------------------------*/

	AD1CON3bits.ADCS	=	5;
	Nop()
	AD1CON3bits.SAMC	=	1;
	Nop()
	AD1CON3bits.ADRC	=	0;

/*=====================================================================*/		
/*----- AD1CHS123 Bits -----------------------------------------------------*/

	AD1CHS123bits.CH123SA	=	0;
	Nop()
	AD1CHS123bits.CH123NA	=	0;
	Nop()
	AD1CHS123bits.CH123SB	=	0;
	Nop()
	AD1CHS123bits.CH123NB	=	0;

/*=====================================================================*/		
/*----- AD1CHS0 Bits -----------------------------------------------------*/

	AD1CHS0bits.CH0SA	=	0;
	Nop()
	AD1CHS0bits.CH0NA	=	0;
	Nop()
	AD1CHS0bits.CH0SB	=	0;
	Nop()
	AD1CHS0bits.CH0NB	=	0;

/*=====================================================================*/		
/*----- AD1PCFGL Bits -----------------------------------------------------*/

	AD1PCFGLbits.PCFG0	=	0;
	Nop()
	AD1PCFGLbits.PCFG1	=	1;
	Nop()
	AD1PCFGLbits.PCFG2	=	1;
	Nop()
	AD1PCFGLbits.PCFG3	=	1;
	Nop()
	AD1PCFGLbits.PCFG4	=	1;
	Nop()
	AD1PCFGLbits.PCFG5	=	1;
	Nop()
	AD1PCFGLbits.PCFG6	=	1;
	Nop()
	AD1PCFGLbits.PCFG7	=	1;
	Nop()
	AD1PCFGLbits.PCFG8	=	1;
	Nop()
	AD1PCFGLbits.PCFG9	=	1;
	Nop()
	AD1PCFGLbits.PCFG10	=	1;
	Nop()
	AD1PCFGLbits.PCFG11	=	1;
	Nop()
	AD1PCFGLbits.PCFG12	=	1;
	Nop()
	AD1PCFGLbits.PCFG13	=	1;
	Nop()
	AD1PCFGLbits.PCFG14	=	1;
	Nop()
	AD1PCFGLbits.PCFG15	=	1;

/*=====================================================================*/		
/*----- AD1CSSL Bits -----------------------------------------------------*/

	AD1CSSLbits.CSS0	=	1;
	Nop()
	AD1CSSLbits.CSS1	=	0;
	Nop()
	AD1CSSLbits.CSS2	=	0;
	Nop()
	AD1CSSLbits.CSS3	=	0;
	Nop()
	AD1CSSLbits.CSS4	=	0;
	Nop()
	AD1CSSLbits.CSS5	=	0;
	Nop()
	AD1CSSLbits.CSS6	=	0;
	Nop()
	AD1CSSLbits.CSS7	=	0;
	Nop()
	AD1CSSLbits.CSS8	=	0;
	Nop()
	AD1CSSLbits.CSS9	=	0;
	Nop()
	AD1CSSLbits.CSS10	=	0;
	Nop()
	AD1CSSLbits.CSS11	=	0;
	Nop()
	AD1CSSLbits.CSS12	=	0;
	Nop()
	AD1CSSLbits.CSS14	=	0;
	Nop()
	AD1CSSLbits.CSS15	=	0;

/*=====================================================================*/		
/*----- AD1CON4 Bits -----------------------------------------------------*/

	AD1CON4bits.DMABL	=	0;

/*=====================================================================*/		

/* init_AD2 */ 
/*----- AD2CON1 Bits -----------------------------------------------------*/

	AD2CON1bits.DONE	=	0;
	Nop()
	AD2CON1bits.SAMP	=	0;
	Nop()
	AD2CON1bits.ASAM	=	1;
	Nop()
	AD2CON1bits.SIMSAM	=	0;
	Nop()
	AD2CON1bits.SSRC	=	7;
	Nop()
	AD2CON1bits.FORM	=	0;
	Nop()
	AD2CON1bits.AD12B	=	1;
	Nop()
	AD2CON1bits.ADDMABM	=	1;
	Nop()
	AD2CON1bits.ADSIDL	=	0;
	Nop()
	AD2CON1bits.ADON	=	1; 

/*=====================================================================*/		
/*--- AD2CON2 Bits -----------------------------------------------------*/

	AD2CON2bits.ALTS	=	0;
	Nop()
	AD2CON2bits.BUFM	=	0;
	Nop()
	AD2CON2bits.SMPI	=	0;
	Nop()
	AD2CON2bits.BUFS	=	0;
	Nop()
	AD2CON2bits.CHPS	=	0;
	Nop()
	AD2CON2bits.CSCNA	=	0;
	Nop()
	AD2CON2bits.VCFG	=	0;

/*=====================================================================*/		
/*----- AD2CON3 Bits -----------------------------------------------------*/

	AD2CON3bits.ADCS	=	5;
	Nop()
	AD2CON3bits.SAMC	=	1;
	Nop()
	AD2CON3bits.ADRC	=	0;

/*=====================================================================*/		
/*----- AD2CHS123 Bits -----------------------------------------------------*/

	AD2CHS123bits.CH123SA	=	0;
	Nop()
	AD2CHS123bits.CH123NA	=	0;
	Nop()
	AD2CHS123bits.CH123SB	=	0;
	Nop()
	AD2CHS123bits.CH123NB	=	0;

/*=====================================================================*/		
/*----- AD2CHS0 Bits -----------------------------------------------------*/

	AD2CHS0bits.CH0SA	=	1;
	Nop()
	AD2CHS0bits.CH0NA	=	0;
	Nop()
	AD2CHS0bits.CH0SB	=	0;
	Nop()
	AD2CHS0bits.CH0NB	=	0;

/*=====================================================================*/		
/*----- AD2PCFGL Bits -----------------------------------------------------*/

	AD2PCFGLbits.PCFG0	=	1;
	Nop()
	AD2PCFGLbits.PCFG1	=	0;
	Nop()
	AD2PCFGLbits.PCFG2	=	0;
	Nop()
	AD2PCFGLbits.PCFG3	=	1;
	Nop()
	AD2PCFGLbits.PCFG4	=	1;
	Nop()
	AD2PCFGLbits.PCFG5	=	1;
	Nop()
	AD2PCFGLbits.PCFG6	=	1;
	Nop()
	AD2PCFGLbits.PCFG7	=	1;
	Nop()
	AD2PCFGLbits.PCFG8	=	1;
	Nop()
	AD2PCFGLbits.PCFG9	=	1;
	Nop()
	AD2PCFGLbits.PCFG10	=	1;
	Nop()
	AD2PCFGLbits.PCFG11	=	1;
	Nop()
	AD2PCFGLbits.PCFG12	=	1;
	Nop()
	AD2PCFGLbits.PCFG13	=	1;
	Nop()
	AD2PCFGLbits.PCFG14	=	1;
	Nop()
	AD2PCFGLbits.PCFG15	=	1;

/*=====================================================================*/		
/*----- AD2CSSL Bits -----------------------------------------------------*/

	AD2CSSLbits.CSS0	=	0;
	Nop()
	AD2CSSLbits.CSS1	=	0;
	Nop()
	AD2CSSLbits.CSS2	=	1;
	Nop()
	AD2CSSLbits.CSS3	=	0;
	Nop()
	AD2CSSLbits.CSS4	=	0;
	Nop()
	AD2CSSLbits.CSS5	=	0;
	Nop()
	AD2CSSLbits.CSS6	=	0;
	Nop()
	AD2CSSLbits.CSS7	=	0;
	Nop()
	AD2CSSLbits.CSS8	=	0;
	Nop()
	Nop()
	AD2CSSLbits.CSS9	=	0;
	Nop()
	AD2CSSLbits.CSS10	=	0;
	Nop()
	AD2CSSLbits.CSS11	=	0;
	Nop()
	AD2CSSLbits.CSS12	=	0;
	Nop()
	AD2CSSLbits.CSS14	=	0;
	Nop()
	AD2CSSLbits.CSS15	=	0;

/*=====================================================================*/		
/*----- AD2CON4 Bits -----------------------------------------------------*/

	AD2CON4bits.DMABL	=	0;

/*=====================================================================*/		
}
