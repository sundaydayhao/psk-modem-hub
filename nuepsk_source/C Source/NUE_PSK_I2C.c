/*======================================================================

		NUE_PSK I2C

		John H. Fisher	K5JHF
		Began On:		09/24/2006
		Current Date:	09/27/2006

======================================================================*/

#include	"NUE_PSK_Header.h"

/*=====================================================================*/		

void init_I2C1	( void )	{

	I2C1RCV					=	0;
	I2C1TRN					=	0;
	I2C1ADD					=	0; 
	I2C1MSK					=	0;

	I2C1BRG					=	363;

	I2C1CONbits.SEN			=	0;	
	Nop()
	I2C1CONbits.RSEN		=	0;
	Nop()
	I2C1CONbits.PEN			=	0;	
	Nop()
	I2C1CONbits.RCEN		=	0;	
	Nop()
	I2C1CONbits.ACKEN		=	0;	
	Nop()
	I2C1CONbits.ACKDT		=	0;	
	Nop()
	I2C1CONbits.STREN		=	0;	
	Nop()
	I2C1CONbits.GCEN		=	0;	
	Nop()
	I2C1CONbits.SMEN		=	0;	
	Nop()
	I2C1CONbits.DISSLW		=	0;	
	Nop()
	I2C1CONbits.A10M		=	0;	
	Nop()
	I2C1CONbits.IPMIEN		=	0;	
	Nop()
	I2C1CONbits.SCLREL		=	0;	
	Nop()
	I2C1CONbits.I2CSIDL		=	0;	
	Nop()
	I2C1CONbits.I2CEN		=	1;	

	I2C1STATbits.TBF		=	0;
	Nop()
	I2C1STATbits.RBF		=	0;
	Nop()
	I2C1STATbits.R_W		=	0;
	Nop()
	I2C1STATbits.S			=	0;
	Nop()
	I2C1STATbits.P			=	0;
	Nop()
	I2C1STATbits.D_A		=	0;
	Nop()
	I2C1STATbits.I2COV		=	0;
	Nop()
	I2C1STATbits.IWCOL		=	0;
	Nop()
	I2C1STATbits.ADD10		=	0;
	Nop()
	I2C1STATbits.GCSTAT		=	0;
	Nop()
	I2C1STATbits.BCL		=	0;
	Nop()
	I2C1STATbits.TRSTAT		=	0;
	Nop()
	I2C1STATbits.ACKSTAT	=	0;
	
	}

/*=====================================================================*/		

void	I2C1_Byte_Write	( int Write_Address, char Write_Data )	{

	I2C1CONbits.SEN	=	1;
	while ( I2C1CON & 0b11111 );
	
	I2C1TRN	=	EEPROM_Write_Address;
	while ( I2C1STATbits.TRSTAT );
	
	I2C1TRN	=	Write_Address>>8;
	while ( I2C1STATbits.TRSTAT );

	I2C1TRN	=	Write_Address;
	while ( I2C1STATbits.TRSTAT );

	I2C1TRN	=	Write_Data;
	while ( I2C1STATbits.TRSTAT );
	
	I2C1CONbits.PEN	=	1;
	while ( I2C1CONbits.PEN );

	Delay_5ms();
	Delay_5ms();
	}

/*=====================================================================*/		

char	I2C1_Byte_Read	( int Read_Address )	{

	char	Read_Data;
	
	I2C1CONbits.SEN	=	1;
	while ( I2C1CONbits.SEN );
	
	I2C1TRN	=	EEPROM_Write_Address;
	while ( I2C1STATbits.TRSTAT );
	
	I2C1TRN	=	Read_Address>>8;
	while ( I2C1STATbits.TRSTAT );

	I2C1TRN	=	Read_Address;
	while ( I2C1STATbits.TRSTAT );

	I2C1CONbits.RSEN	=	1;
	while ( I2C1CONbits.RSEN );
	
	I2C1TRN	=	EEPROM_Read_Address;
	while ( I2C1STATbits.TRSTAT );

	while ( I2C1STATbits.ACKSTAT );

	I2C1CONbits.RCEN	=	1;
	while ( I2C1CONbits.RCEN );

	Read_Data	=	I2C1RCV;

	I2C1CONbits.ACKDT	=	1;
	I2C1CONbits.ACKEN	=	1;
	while ( I2C1CONbits.ACKEN );

	I2C1CONbits.PEN	=	1;
	while ( I2C1CONbits.PEN );

	return	Read_Data;

	}

/*=====================================================================*/		
