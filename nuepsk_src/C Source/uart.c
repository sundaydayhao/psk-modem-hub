/********************************************************************
* uart.c
*
*********************************************************************/

#include "p33FJ128MC706.h"
#include "NUE_PSK_Header.h"

#define FCY   			40000000
#define BAUDRATE        115200                   
#define BRGVAL          ((FCY/BAUDRATE)/16)-1 

/******************************************************************************/
void InitUart2 (int baud)
{
	int	i;

 	if (baud == FAST) U2BRG = BRGVAL;   /* BAUD Rate Setting of Uart2  (for 115200)*/
	else U2BRG = 0x103; /* setting for 9600 baud */
	U2MODE = 0x8000; /* Reset UART to 8-n-1, alt pins, and enable */
	U2STA  = 0x0400; /* Reset status register and enable TX */
}

/******************************************************************************/
int GetUartChar(char * ptrChar)
{
	extern int Timer_Count;
	Timer_Count = 0;
	while(1)
	{	
		if (Timer_Count > 4000) {
			Timer_Count = 0;
			return 3;
		} 

		/* check for receive errors */
		if(U2STAbits.FERR == 1)
		{
			continue;
			return 2;
		}
			
		/* must clear the overrun error to keep uart receiving */
		if(U2STAbits.OERR == 1)
		{
			U2STAbits.OERR = 0;
			continue;
			return 1;
		}

		/* get the data */
		if(U2STAbits.URXDA == 1)
		{
			* ptrChar = U2RXREG;
			break;
		}
	}
	Timer_Count = 0; 
	return 0;
}

/******************************************************************************/

/* parse a string and send characters to uart */
void PutUartString ( char *m, char Check_Count ) {
	char x;
	while (x=*m++) PutUartChar ( x, Check_Count );
}

/******************************************************************************/

void PutUartChar(char Char, char Check_Count) {
	extern int byte_count;
	int i;

	while(!U2STAbits.TRMT);
	while (USB_CTS); /* added 1/27/10 */
	U2TXREG = Char;

	if (Check_Count) {
		if (++byte_count == 20) {
			byte_count = 0;
			Delay_5ms();
			PutUartString ("WRF 20\r", NCHK);
			Delay_5ms();
		}
	}
}

/******************************************************************************/

void GetPrompt ( void ) {
	char UartDat;
	while (UartDat != '>') GetUartChar(&UartDat); 
	while (UartDat != 0x0D) GetUartChar(&UartDat);
}

/******************************************************************************/
