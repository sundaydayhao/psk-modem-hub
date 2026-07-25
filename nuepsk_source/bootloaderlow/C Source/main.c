/********************************************************************
* FileName:		lowboot_main.c
* Dependencies:    
* Processor:		dsPIC33F Family
* Hardware:		NUE_PSK Modem (Code modified by W8NUE)
* Compiler:		C30 2.01
* Company:		Microchip Technology, Inc.
*
* Software License Agreement
*
* The software supplied herewith by Microchip Technology Incorporated
* (the “Company”) for its PICmicro® Microcontroller is intended and
* supplied to you, the Company’s customer, for use solely and
* exclusively on Microchip PICmicro Microcontroller products. The
* software is owned by the Company and/or its supplier, and is
* protected under applicable copyright laws. All rights are reserved.
* Any use in violation of the foregoing restrictions may subject the
* user to criminal sanctions under applicable laws, as well as to
* civil liability for the breach of the terms and conditions of this
* license.
*
* THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
* WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
* TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
* IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
* CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
*
*********************************************************************/

#include "p33FJ128MC706.h"
#include <stdlib.h>

#define COMMAND_NACK     0x00
#define COMMAND_ACK      0x01
#define COMMAND_READ_PM  0x02
#define COMMAND_WRITE_PM 0x03
#define COMMAND_WRITE_CM 0x07
#define COMMAND_RESET    0x08
#define COMMAND_READ_ID  0x09

#define EOF				 0x00


#define PM_ROW_SIZE 64 * 8
#define CM_ROW_SIZE 8
#define CONFIG_WORD_SIZE 1

#define PM_ROW_ERASE 		0x4042
#define PM_ROW_WRITE 		0x4001
#define CONFIG_WORD_WRITE	0X4000

#define FCY   40000000
#define BAUDRATE        115200                   
#define BRGVAL          ((FCY/BAUDRATE)/16)-1 

typedef short          Word16;
typedef unsigned short UWord16;
typedef long           Word32;
typedef unsigned long  UWord32;

typedef union tuReg32
{
	UWord32 Val32;

	struct
	{
		UWord16 LW;
		UWord16 HW;
	} Word;

	char Val[4];
} uReg32;

extern UWord32 ReadLatch(UWord16, UWord16);
void PutChar_Boot(char);
int  GetChar_Boot(char *);
void GetBootPrompt (void);
void PutUartString_Boot ( char *m);
void WritePM(char *, uReg32);
int  SyncUart ( void );
char Buffer[PM_ROW_SIZE*3+1];

int  main(void) 
{ 

// Configure Oscillator to operate the device at 40Mhz
// Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
// Fosc= (10M*32/(2*2)=80Mhz for 10M input clock

// We are starting with a 10MHz oscillator.
// PLLPRE needs to be set to 2 for 5MHz into the PLL
// PLLDIV will be set to 32 so that VCO will run at 160MHz
// PLLPOST is set to 2 to provide an 80MHz Fosc
// Fcy will then be 40MHz
	
	PLLFBDbits.PLLDIV	=	30;	// M=32
	Nop()
	CLKDIVbits.PLLPOST=0;		// N1=2
	Nop();
	CLKDIVbits.PLLPRE=0;		// N2=2
	Nop();
	OSCTUN=0;					// Tune FRC oscillator, if FRC is used
	Nop();

	RCONbits.SWDTEN=0;            /* Disable Watch Dog Timer*/
	Nop();

	while(OSCCONbits.LOCK!=1) {}; /* Wait for PLL to lock*/
	Nop();
	
    uReg32 SourceAddr;
	uReg32 Temp, TempAddr;
	char Command;
	int    Size;

/*	UWord32 tempdat; */	

	SyncUart();

	PutUartString_Boot("OPR mem.dat\r"); /* command to open file on flash drive for read */

	while (1) {
		PutUartString_Boot("RDF 1\r");		/* retrieve 1 byte */
		GetChar_Boot(&Command);		/* get command character */

		if(Command == COMMAND_WRITE_PM) {			
			PutUartString_Boot("RDF 1539\r");		/* retrieve 1539 bytes */
			GetChar_Boot(&(SourceAddr.Val[0]));		/* read 3 address bytes */
			GetChar_Boot(&(SourceAddr.Val[1]));
			GetChar_Boot(&(SourceAddr.Val[2]));
			SourceAddr.Val[3]=0;

			/* now read a page of data--1536 bytes */
			for(Size = 0; Size < PM_ROW_SIZE*3; Size++)
			{
				GetChar_Boot(&(Buffer[Size]));
			}
			/* if on first page, keep old Reset Vector */
			/* leave the following section of code in place */
			/* to prevent changes in the reset vector */
		/*	if (SourceAddr.Val32 == 0) {
				TempAddr.Val32 = 0;
				Temp.Val32 = ReadLatch(TempAddr.Word.HW, TempAddr.Word.LW);
				Buffer[0] = Temp.Val[0];
				Buffer[1] = Temp.Val[1];
				Buffer[2] = Temp.Val[2];
				TempAddr.Val32 += 2;
				Temp.Val32 = ReadLatch(TempAddr.Word.HW, TempAddr.Word.LW);
				Buffer[3] = Temp.Val[0];
				Buffer[4] = Temp.Val[1];
				Buffer[5] = Temp.Val[2];
			} */

			Erase(SourceAddr.Word.HW,SourceAddr.Word.LW,PM_ROW_ERASE);
			
			WritePM(Buffer, SourceAddr);		/*program page */
		}
		else break;
	}			
	PutUartString_Boot("CLF mem.dat\r");	/* close the file */
	ResetDevice(); 
	return 0;
}

/******************************************************************************/
int  GetChar_Boot(char * ptrChar) 
{
	while(1)
	{	
		/* check for receive errors */
		if(U2STAbits.FERR == 1)
		{
			continue;
		}
			
		/* must clear the overrun error to keep uart receiving */
		if(U2STAbits.OERR == 1)
		{
			U2STAbits.OERR = 0;
			continue;
		}

		/* get the data */
		if(U2STAbits.URXDA == 1)
		{
			* ptrChar = U2RXREG;
			break;
		}
	}
	return 0;
}


/******************************************************************************/

void  PutChar_Boot(char Char)
{
	while(!U2STAbits.TRMT);
	
	U2TXREG = Char;
}
/******************************************************************************/

/* parse a string and send characters to uart */
void  PutUartString_Boot ( char *m) {
	char x;
	while ((x=*m++)) PutChar_Boot ( x );
}

/******************************************************************************/

void  WritePM(char * ptrData, uReg32 SourceAddr)
{
	int    Size,Size1;
	uReg32 Temp;
	uReg32 TempAddr;
	uReg32 TempData;

	for(Size = 0,Size1=0; Size < PM_ROW_SIZE; Size++)
	{
		
		Temp.Val[0]=ptrData[Size1+0];
		Temp.Val[1]=ptrData[Size1+1];
		Temp.Val[2]=ptrData[Size1+2];
		Temp.Val[3]=0;
		Size1+=3;

	   WriteLatch(SourceAddr.Word.HW, SourceAddr.Word.LW,Temp.Word.HW,Temp.Word.LW);

		/* Device ID errata workaround: Save data at any address that has LSB 0x18 */
		if((SourceAddr.Val32 & 0x0000001F) == 0x18)
		{
			TempAddr.Val32 = SourceAddr.Val32;
			TempData.Val32 = Temp.Val32;
		}

		if((Size !=0) && (((Size + 1) % 64) == 0))
		{
			/* Device ID errata workaround: Reload data at address with LSB of 0x18 */
	      WriteLatch(TempAddr.Word.HW, TempAddr.Word.LW,TempData.Word.HW,TempData.Word.LW);

			WriteMem(PM_ROW_WRITE);
		}

		SourceAddr.Val32 = SourceAddr.Val32 + 2;
	}

   
}

/******************************************************************************/

void GetBootPrompt( void ) {
	char UartDat;

	while (1)
	{
		GetChar_Boot(&UartDat);
		if (UartDat == '>') break;
	}

	while (1)
	{
		GetChar_Boot(&UartDat);
		if (UartDat == 0x0D) break;
	}
	return;  
}

/******************************************************************************/

int SyncUart ( void ) {
	char UartDat;

	while (UartDat != 'e') {
		PutUartString_Boot ("E\r");
		while (UartDat != 'E') GetChar_Boot (&UartDat);
		GetChar_Boot (&UartDat); /* flush out the CR */
		PutUartString_Boot ("e\r");
		GetChar_Boot (&UartDat);
	}
	GetChar_Boot (&UartDat); /* flush out the CR */
	return 0;	
}

/******************************************************************************/


