/********************************************************************
* FileName:		main.c
* Dependencies:    
* Processor:		dsPIC33F Family
* Hardware:		NUE_PSK Modem (Code modified by W8NUE)
* Compiler:		C30 2.01
* Company:		Microchip Technology, Inc.
* Version:		v1.10
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

#define COMMAND_NACK     0x00
#define COMMAND_ACK      0x01
#define COMMAND_READ_PM  0x02
#define COMMAND_WRITE_PM 0x03
#define COMMAND_WRITE_CM 0x07
#define COMMAND_RESET    0x08
#define COMMAND_READ_ID  0x09


#define PM_ROW_SIZE 64 * 8
#define CM_ROW_SIZE 8
#define CONFIG_WORD_SIZE 1

#define PM_ROW_ERASE 		0x4042
#define PM_ROW_WRITE 		0x4001
#define CONFIG_WORD_WRITE	0X4000

#define FCY   40000000
#define BAUDRATE        115200                   
#define BRGVAL          ((FCY/BAUDRATE)/16)-1 

#define PButton_tris    TRISGbits.TRISG9
#define PButton         PORTGbits.RG9
#define DELAYTIME		0x000A /* 10 second delay */


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
void PutChar(char);
void GetChar(char *);
void WriteBuffer(char *, int);
void ReadPM(char *, uReg32);
void WritePM(char *, uReg32);

char Buffer[PM_ROW_SIZE*3 + 1];

int main(void)

{

    uReg32 SourceAddr;
	uReg32 Delay;
	int i;
	
// Configure Oscillator to operate the device at 40Mhz
// Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
// Fosc= (10M*32/(2*2)=80Mhz for 10M input clock

// We are starting with a 10MHz oscillator.
// PLLPRE needs to be set to 2 for 5MHz into the PLL
// PLLDIV will be set to 32 so that VCO will run at 160MHz
// PLLPOST is set to 2 to provide an 80MHz Fosc
// Fcy will then be 40MHz

	
	PLLFBDbits.PLLDIV	=	30;	// M=32
	Nop();
	CLKDIVbits.PLLPOST=0;		// N1=2
	Nop();
	CLKDIVbits.PLLPRE=0;		// N2=2
	Nop();

	__builtin_write_OSCCONH(0x03);
	__builtin_write_OSCCONL(0x01);
	while(OSCCONbits.COSC != 0b011);

	RCONbits.SWDTEN=0;            /* Disable Watch Dog Timer*/
	Nop();
	PButton_tris=1;				  /* Set as input */	
	CNPU1bits.CN11PUE=1;          /* Enable pull-up on pushbutton */
	Nop();

	while(OSCCONbits.LOCK!=1) {}; /* Wait for PLL to lock*/
	Nop();
	
	/* 25 ms delay */
	for ( i = 0; i < 250 ; i++ ) {
	__asm__ ("repeat #4000");
	__asm__	("nop");
	} 


	SourceAddr.Val32 = 0x400; 

	Delay.Val32 = ReadLatch(SourceAddr.Word.HW, SourceAddr.Word.LW);

	if(Delay.Val[0] == 0)
	{
		ResetDevice();
	} 

	if (PButton) ResetDevice();

	T2CONbits.T32 = 1; /* to increment every instruction cycle */
	IFS0bits.T3IF = 0; /* Clear the Timer3 Interrupt Flag */
	IEC0bits.T3IE = 0; /* Disable Timer3 Interrup Service Routine */

/*	if((Delay.Val32 & 0x000000FF) != 0xFF)
	{
		 Convert seconds into timer count value */
		Delay.Val32 = ((UWord32)(FCY)) * ((UWord32)(Delay.Val[0]));

		PR3 = Delay.Word.HW;
		PR2 = Delay.Word.LW;

		/* Enable Timer */
		T2CONbits.TON=1;
/*	} */

	U2BRG = BRGVAL;      /*  BAUD Rate Setting of Uart2  (0XEF for 9600)*/


	U2MODE = 0x8000; /* Reset UART to 8-n-1, alt pins, and enable */
	U2STA  = 0x0400; /* Reset status register and enable TX */

	while(1)
	{
		char Command;

		GetChar(&Command);

		switch(Command)
		{
			case COMMAND_READ_PM:				/*tested*/
			{
				uReg32 SourceAddr;

				GetChar(&(SourceAddr.Val[0]));
				GetChar(&(SourceAddr.Val[1]));
				GetChar(&(SourceAddr.Val[2]));
				SourceAddr.Val[3]=0;

				ReadPM(Buffer, SourceAddr);

				WriteBuffer(Buffer, PM_ROW_SIZE*3);

				break;
			}

			case COMMAND_WRITE_PM:				/* tested */
			{
			    uReg32 SourceAddr;
				int    Size;

				GetChar(&(SourceAddr.Val[0]));
				GetChar(&(SourceAddr.Val[1]));
				GetChar(&(SourceAddr.Val[2]));
				SourceAddr.Val[3]=0;
				
				for(Size = 0; Size < PM_ROW_SIZE*3; Size++)
				{
				GetChar(&(Buffer[Size]));
				}

				Erase(SourceAddr.Word.HW,SourceAddr.Word.LW,PM_ROW_ERASE);
		
				WritePM(Buffer, SourceAddr);		/*program page */

				PutChar(COMMAND_ACK);				/*Send Acknowledgement */

 				break;
			}

			case COMMAND_READ_ID:
			{
				uReg32 SourceAddr;
				uReg32 Temp;

				SourceAddr.Val32 = 0xFF0000;

				Temp.Val32 = ReadLatch(SourceAddr.Word.HW, SourceAddr.Word.LW);

				WriteBuffer(&(Temp.Val[0]), 4);

				SourceAddr.Val32 = 0xFF0002;

				Temp.Val32 = ReadLatch(SourceAddr.Word.HW, SourceAddr.Word.LW);

				WriteBuffer(&(Temp.Val[0]), 4);

				break;
			}
			
			case COMMAND_WRITE_CM:	
			{
				int    Size;

				for(Size = 0; Size < CM_ROW_SIZE*3;)
				{
					GetChar(&(Buffer[Size++]));
					GetChar(&(Buffer[Size++]));
					GetChar(&(Buffer[Size++]));
				
					PutChar(COMMAND_ACK);				/*Send Acknowledgement */
				}

				
				break;
			}
				case COMMAND_RESET:
			{
				uReg32 SourceAddr;
				int    Size;
				uReg32 Temp;
				

				for(Size = 0, SourceAddr.Val32 = 0xF80000; Size < CM_ROW_SIZE*3; 
																Size +=3, SourceAddr.Val32 += 2)
				{
					if(Buffer[Size] == 0)
					{
						Temp.Val[0]=Buffer[Size+1];
						Temp.Val[1]=Buffer[Size+2];

						WriteLatch( SourceAddr.Word.HW,
										SourceAddr.Word.LW,
										Temp.Word.HW,
										Temp.Word.LW);

						WriteMem(CONFIG_WORD_WRITE);
					}
				}
				

				ResetDevice();

				break;
			}

			case COMMAND_NACK:
			{
				ResetDevice();

				break;
			}

			
			default:
				PutChar(COMMAND_NACK);
				break;
		}

	}

}

/******************************************************************************/
void GetChar(char * ptrChar)
{
	while(1)
	{	
		/* if timer expired, signal to application to jump to user code */
		if(IFS0bits.T3IF == 1)
		{
			* ptrChar = COMMAND_NACK;
			break;
		}
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
			T2CONbits.TON=0; /* Disable timer countdown */
			* ptrChar = U2RXREG;
			break;
		}
	}
}


/******************************************************************************/
void ReadPM(char * ptrData, uReg32 SourceAddr)
{
	int    Size;
	uReg32 Temp;

	for(Size = 0; Size < PM_ROW_SIZE; Size++)
	{
		Temp.Val32 = ReadLatch(SourceAddr.Word.HW, SourceAddr.Word.LW);

		ptrData[0] = Temp.Val[2];;
		ptrData[1] = Temp.Val[1];;
		ptrData[2] = Temp.Val[0];;

		ptrData = ptrData + 3;

		SourceAddr.Val32 = SourceAddr.Val32 + 2;
	}
}
/******************************************************************************/

void WriteBuffer(char * ptrData, int Size)
{
	int DataCount;
	
	for(DataCount = 0; DataCount < Size; DataCount++)
	{
		PutChar(ptrData[DataCount]);
	}
}
/******************************************************************************/
void PutChar(char Char)
{
	while(!U2STAbits.TRMT);
	
	U2TXREG = Char;
}
/******************************************************************************/

void WritePM(char * ptrData, uReg32 SourceAddr)
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



