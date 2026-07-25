/********************************************************************
* FileName:		bootkernel.c
* Dependencies:    
* Processor:		dsPIC33F Family
* Hardware:		Explorer 16
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
#include "p33fxxxx.h"

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



int Flag=0;


int main(void)
{
UWord32  Size;

uReg32 Temp;

uReg32 SourceAddr;

TRISAbits.TRISA0 = 0;	//make I/O output

SourceAddr.Val32 = 0xe00;


for(Size=0;Size<30720;Size++)
{

Temp.Val32 = ReadLatch(SourceAddr.Word.HW, SourceAddr.Word.LW);

if ( Size != Temp.Val32)
{
	Flag=0;
	break;
}
else
{
	Flag=1;
	
}
SourceAddr.Val32 = SourceAddr.Val32 + 2;

}
/*----------------------------------------*/
SourceAddr.Val32 = 0xfe00;

for(Size=0;Size<30720;Size++)
{

Temp.Val32 = ReadLatch(SourceAddr.Word.HW, SourceAddr.Word.LW);

if ( Size != Temp.Val32)
{
	Flag=0;
	break;
}
else
{
	Flag=1;
	
}
SourceAddr.Val32 = SourceAddr.Val32 + 2;

}
/*----------------------------------------*/
SourceAddr.Val32 = 0x1ee00;
for(Size=0;Size<24320;Size++)				
{

Temp.Val32 = ReadLatch(SourceAddr.Word.HW, SourceAddr.Word.LW);

if ( Size != Temp.Val32)
{
	Flag=0;
	break;
}
else
{
	Flag=1;
	
}
SourceAddr.Val32 = SourceAddr.Val32 + 2;

}
/*----------------------------------------*/


while(1)
{
	if(Flag==1)
   _ToggleLED();

}

}

void _ToggleLED(void)
{

static int count = 0;
	
	
		if(++count == 0)
		{
			LATAbits.LATA0 = ~LATAbits.LATA0;
		}
}
