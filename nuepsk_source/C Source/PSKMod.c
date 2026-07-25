/*==================================================================*/
/* PSKMod.cpp: implementation of the CPSKMod class.		    		*/
/*								   									*/
/*==================================================================*/
/*==================================================================*/
/* PSK31Core Library for transmission and reception of PSK31 	    */
/*  signals using a PC soundcard  or .wav files.		    		*/
/*	               Copyright 2000, Moe Wheatley, AE4JY	   			*/
/*																	*/
/* With modifications by Milt Cram, W8NUE  7/1/08                   */
/*								    								*/	
/*This library is free software; you can redistribute it and/or	    */
/*modify it under the terms of the GNU Lesser General Public	    */
/*License as published by the Free Software Foundation; either	    */
/*version 2.1 of the License, or (at your option) any later version.*/
/*								   									*/
/*This library is distributed in the hope that it will be useful,   */
/*but WITHOUT ANY WARRANTY; without even the implied warranty of    */
/*MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU */
/*Lesser General Public License for more details.		    		*/
/*								    								*/
/*You should have received a copy of the GNU Lesser General Public  */
/*License along with this library; if not, write to the Free 	    */
/*  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,  */
/*  MA  02111-1307  USA 					   						*/
/* http://www.gnu.org/copyleft/lesser.html			   				*/	
/*==================================================================*/

#include "NUE_PSK_Header.h"
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "PSKMod.h"
#include "PSK_TX_ShapeTable_1.h"

/* local defines................. */

#define BACK_SPACE_CODE 0x08	/* define some control codes for */

#define PHZ_0 0			/*specify various signal phase states */
#define PHZ_90 1
#define PHZ_180 2
#define PHZ_270 3
#define PHZ_OFF 4

#define SYM_NOCHANGE 0	/*Stay the same phase */
#define SYM_P90 1		/*Plus 90  deg */
#define SYM_P180 2		/*Plus 180 deg */
#define SYM_M90 3		/*Minus 90 deg */
#define SYM_OFF 4		/*No output */
#define SYM_ON 5		/*constant output */

#define TXOFF_CODE 2	/* control code that can be placed in the input by Ctrl-Q */
#define TXON_CODE 3		/* ditto for Ctrl-S */
#define TXTOG_CODE 1

#define TX_CNTRL_AUTOSTOP 4		/* Alt-S */
#define TX_CNTRL_ADDCWID 5		/* Alt-C */
#define TX_CNTRL_NOSQTAIL 6		/* Alt-Q */

#define TX_OFF_STATE 0			/* TX is off */
#define TX_SENDING_STATE 1		/* TX is sending text */
#define TX_PAUSED_STATE 2		/* TX is paused */
#define TX_PREAMBLE_STATE 3		/* TX sending starting preamble */
#define TX_POSTAMBLE_STATE 4	/* TX sending ending posteamble */
#define TX_CWID_STATE 5			/* TX sending CW ID */
#define TX_TUNE_STATE 6			/* TX is tuning mode */


#define SYMBOL_RATE31 31.25		/* 31.25 Symbols per Second */
#define SYMBOL_RATE63 62.5		/* PSK63 62.50 Symbols per Second */
#define SYMBOL_RATERTTY 45.45	/* RTTY 45.45 Symbols per Second */

#define MAXRAMP_SIZE 256 /* max number of envelope ramp steps per symbol */

#define TX_CONSTANT 22000.0		/* TX Amplitude Factor */
/*#define TX_CONSTANT 750.0		 TX Amplitude Factor(threshold if noise ==16384/5 */

#define CW_SPEED 2	/* wpm = 2.4*dotperiod(in msec) = 2.4*32/(2*CW_SPEED) = 38.4/CW_SPEED */

/* The use of static's here preclude having multiple instantiations */
/* of this class but should not be an issue since only one soundcard. */

/*Ramp shape tables that are loaded with cosine shaped functions into program memory */
// extern const float PSKShapeTbl[2049]; 
extern const unsigned int CW_TABLE[];
extern const unsigned int VARICODE_TABLE[];
extern const unsigned char ConvolutionCodeTable[];



/*==================================================================*/
/* Construction/Destruction */
/*==================================================================*/

void PSKTXSetup (void) {
	extern int Cw_On;
	extern int m_NeedCWid;
	m_NeedCWid = FALSE;
	m_NeedShutoff = FALSE;
	m_NoSquelchTail = FALSE;
	m_TempNeedCWid = FALSE;
	m_TempNeedShutoff = FALSE;
	m_TempNoSquelchTail = FALSE;
	m_AmblePtr = 0;
	m_TxFreq  = 1500.0;
	Cw_On = CWID;
}

/*==================================================================*/
/*/////////   P S K 3 1   M O D U L A T O R  //////////////// 	    */
/*==================================================================*/

/*==================================================================*/
/*	Initialize the PSK/CW modulator with the sample frequncy Fs,    */
/* 	the carrier frequency freq, and the PSK mode. 		    		*/
/*==================================================================*/
void InitPSKMod(void)
{
	int i;
	extern double m_TClkError;
	extern int m_CWSetSpeed;
	extern int m_Fs;
	char ch;  


	m_CWSetSpeed = CW_SPEED;
	m_Fs = 8000;
	m_Ramp = 0;
	m_pTail = 0;
	m_pHead = 0;
	m_TClkError = 1.0;
	m_SampleFreq = (double)m_Fs*m_TClkError;

	m_PSKPhaseInc = PI2 * m_TxFreq/m_SampleFreq;		/*carrier frequency */
	m_PSKSecPerSamp = 1.0/m_SampleFreq;
	m_Lastsymb = SYM_OFF;
	m_AddEndingZero = TRUE;
	m_CWState = 0;
	m_CWtimer = 0;
	SetTXMode(m_PSKmode);

	for(i=0; i<16; i++)
		m_IQPhaseArray[i] = 1;
	m_VectLookup[SYM_NOCHANGE][0] = 0;
	m_VectLookup[SYM_NOCHANGE][1] = 1000;
	m_VectLookup[SYM_P90][0] = 1000;
	m_VectLookup[SYM_P90][1] = 0;
	m_VectLookup[SYM_P180][0] = 0;
	m_VectLookup[SYM_P180][1] = -1000;
	m_VectLookup[SYM_M90][0] = -1000;
	m_VectLookup[SYM_M90][1] = 0;
	m_VectLookup[SYM_OFF][0] = 0;
	m_VectLookup[SYM_OFF][1] = 0;
	m_VectLookup[SYM_ON][0] = 0;
	m_VectLookup[SYM_ON][1] = 1000;
	i = 0;
	while(i<32)		/*create post/preamble tables */
	{
		m_Preamble[i] = TXTOG_CODE;
		m_Postamble[i++] = TXON_CODE;
	}
	m_Preamble[i] = 0;		/* null terminate these tables */
	m_Postamble[i] = 0;

	for (i=0;i<2;i++) RTTY_Preamble[i] = 0x1F; /* preamble for RTTY */
	RTTY_Preamble[i] = '\0';

	Itbl_num = 0;
	Qtbl_num = 0;
	m_PresentPhase = PHZ_OFF;
	m_TxShiftReg = 0;
	m_TxCodeWord = 0;
}

/*==================================================================*/
/* Called to set the transmit mode				    */
/*==================================================================*/
void SetTXMode(int mode)
{
	extern unsigned int RTTY_Shift;
	extern double deltaPHZ;
	m_PSKmode = mode;
	if(m_PSKmode&(1<<3) )
	{
		m_SymbolRate = SYMBOL_RATE63;
		m_CWIDSpeed = m_CWSetSpeed*2;
	}
	else
	{
		m_SymbolRate = SYMBOL_RATE31;
		m_CWIDSpeed = m_CWSetSpeed;
	}
	if (m_PSKmode == RTTY_MODE) {
		m_SymbolRate = SYMBOL_RATERTTY;
	}
	m_PSKPeriodUpdate = 1.0/m_SymbolRate;	/*symbol period */
	switch(m_PSKmode&0x07)
	{
		case BPSK_MODE:
		case QPSKU_MODE:
		case QPSKL_MODE:
		case RTTY_MODE:
			m_TXState = TX_PREAMBLE_STATE;
			deltaPHZ = PI2*RTTY_Shift/(2*Sample_Frequency);
			break;
		case TUNE_MODE:
			m_TXState = TX_TUNE_STATE;
			break;
		case TUNE_MODE_WID:
			m_TXState = TX_TUNE_STATE;
			m_NeedCWid = TRUE;
			break;
		case CW_MODE:
			m_TXState = TX_PREAMBLE_STATE;
			break;
		default:
			m_TXState = TX_PREAMBLE_STATE;
			m_PSKmode = BPSK_MODE;
			break;
	}

}

/*==================================================================*/
/* generates one sample of waveform for each					    */
/* interrupt produced by Timer1 (8ksps, 125usec) 					*/
/*==================================================================*/

void CalcPSK( ) {
/* generate one output sample.  Will be called at Sample Frequency */
	static char symbol;
	static int v;
	static long TXPhase;
	extern double m_PSKTime;
	extern const int PSKPhaseLookupTable[6][5];
	extern int Itbl_num, Qtbl_num; 
//	extern const float PSKShapeTbl[2049]; 
	extern RTTY_type Rtty;
	float S1, S2;
	float TX_I, TX_Q;
	int Product1, Product2;
	int TXData;
	int tempdata;
	m_RMSConstant = 0.03;

		/*calculate one sample of tx data stream */
		TXPhase += (long) (KCONV*m_PSKPhaseInc);					/* increment radian phase count */
		TX_I = (float)(Sine_lookup ((TXPhase >> 4 ) & 0x0FFF));				/* Q local oscillator */
		TX_Q = (float)(Sine_lookup (((TXPhase >> 4 ) + 0x400) & 0x0FFF));	/* I local oscillator */

		/* create sample from sin/cos and shape tables */
		if (m_PSKmode == RTTY_MODE) {
			S1 = 1;
			S2 = 1;
		}
		else {
/*			S1 = PSKShapeTbl[Itbl_num*256+m_Ramp+1];  */
/*			S2 = PSKShapeTbl[Qtbl_num*256+m_Ramp+1];  */
			S1 = PSKEnv (Itbl_num,m_Ramp);
			S2 = PSKEnv (Qtbl_num,m_Ramp);
		}
		Product1 = (int)(m_RMSConstant * S1 * TX_I);
		Product2 = (int)(m_RMSConstant * S2 * TX_Q);

		TXData = ((Product1  + Product2) & 0x0FFF); 

		m_Ramp += 1; /* if PSK63, increment by two  */
		m_PSKTime += m_PSKSecPerSamp;
		if( m_PSKTime >= m_PSKPeriodUpdate )	/*if time to update symbol */
		{
			m_PSKTime -= m_PSKPeriodUpdate;		/*	keep time bounded */
			m_Ramp = 0;							/* time to update symbol */

			switch( m_PSKmode&0x07 )			/*get next symbol to send */
				{
				case CW_MODE:
					symbol = GetNextCWSymbol();
					break;
				case BPSK_MODE:
					symbol = GetNextBPSKSymbol(); 
					break;
				case QPSKU_MODE:
					symbol = GetNextQPSKSymbol();
					break;
				case QPSKL_MODE:
					symbol = GetNextQPSKSymbol();
					if(symbol==SYM_P90)			/*rotate vectors the opposite way */
						symbol = SYM_M90;
					else
						if(symbol==SYM_M90)
						symbol = SYM_P90;
					break;
				case RTTY_MODE:
					symbol = GetNextRTTYSymbol();
					break;
				case TUNE_MODE:
				case TUNE_MODE_WID:
					symbol = GetNextTuneSymbol();
					break;
				}

			if (m_PSKmode != RTTY_MODE) {
				/*get new I/Q ramp tables and next phase */
				tempdata = PSKPhaseLookupTable[symbol][m_PresentPhase]; 
				Itbl_num = (tempdata & 0x700)>>8;
				Qtbl_num = (tempdata & 0x070)>>4;
				m_PresentPhase = (tempdata & 0x7);
				m_IQPhaseArray[v++] = m_VectLookup[m_PresentPhase][0];
				m_IQPhaseArray[v++] = m_VectLookup[m_PresentPhase][1];
				v = v & 0x000F;	/* keep bounded to 16 */ 
			}
		}
		Output_DAC_1A ((TXData) +0x800);
}

/*==================================================================*/
/* called every symbol time to get next RTTY symbol and get the	    */
/* next character from the character Queue if no more symbols	    */
/* are left to send.						    */
/*==================================================================*/
char GetNextRTTYSymbol(void)
{
	char symb;
	extern RTTY_type Rtty;
	static uchar RTTYDataReg;
	static uchar storedchar;
	extern uchar OldFIGS;
	char ch;
	int i;
	extern double tempPHZ, deltaPHZ;
	symb = m_Lastsymb; 

	if (RegCount)	/* still have bits to be sent in the register */
	{
		if (!Rtty.REVERSE) {
			if( RTTYDataReg&0x01 )
				m_PSKPhaseInc = tempPHZ + deltaPHZ; /* set MARK frequency */
			else
				m_PSKPhaseInc = tempPHZ - deltaPHZ; /* set SPACE frequency */
		}
		else {
			if( RTTYDataReg&0x01 )
				m_PSKPhaseInc = tempPHZ - deltaPHZ; /* set MARK frequency */
			else
				m_PSKPhaseInc = tempPHZ + deltaPHZ; /* set SPACE frequency */
		}
		RTTYDataReg = RTTYDataReg >> 1;	/*point to next bit */
		RegCount--;
	}
	else		/* no more bits in register */
	{
		RegCount = 8;	/* Initialize counter for 8 bits */

		if (Rtty.DAV) {
			ch = storedchar; 
			Rtty.DAV = 0;
		} 
		else ch = GetChar();			/*get next character to xmit */
		if (isalpha(ch)) ch = toupper(ch); /* convert character to upper case */
		if ((ch == 1) || (ch >= 'A') || (ch == ' ') || (ch == '\r') || (ch == '\n')) Rtty.FIGURES = 0;
		else Rtty.FIGURES = 1; 
		if (Rtty.FIGURES != OldFIGS) {
			storedchar = ch;
			Rtty.DAV = 1;
			if (Rtty.FIGURES) i = 0x1B;
			else i = 0x1F;
		}
		else { 
	/*	Rtty.FIGURES = 0; */
			for (i=0; i<0x20; i++) {
				if (RTTY_table [i][Rtty.FIGURES] == ch) break;
			}
			if ((!i) || (i > 0x1F)) {
				i = 0x1F;
				OldFIGS = 0;
			} 
		}
		RTTYDataReg = i;
		RTTYDataReg <<= 1; /* make room for the START bit */
		RTTYDataReg &= 0xFE; /* clear the START bit */
		RTTYDataReg |= 0xC0; /* set the 2 MSBs--STOP bits */		
		OldFIGS = Rtty.FIGURES;
	}
	m_Lastsymb = symb;
	return symb;
}

/*==================================================================*/
/* called every symbol time to get next CW symbol and get the	    */
/* next character from the character Queue if no more symbols	    */
/* are left to send.						    */
/*==================================================================*/
char GetNextCWSymbol(void)
{
	char symb;
	char ch;
	symb = m_Lastsymb;		/*use last symbol unless it needs to change */
	if( (m_TxShiftReg == 0 ) && (m_CWState == 0) )
	{
		ch = GetChar();			/*get next character to xmit */
		switch (ch) {
			case TXON_CODE:
				symb = SYM_ON;
				return symb;
			case TXOFF_CODE:
				symb = SYM_OFF;
				return symb;
			default:
				ch &= 0xFF;		
				/* ch = (int)toupper( (int)ch );	make upper case */
				ch = toupper( ch );
				if( ch>=' ' && ch<='Z')
					m_TxShiftReg = CW_TABLE[ ch-' '];	/*look up pattern */
		}
		m_CWState = 0;
	}
	switch( m_CWState )		/* CW timing state machine */
	{
		case 0:			/*get next cw symbol state from pattern */
			switch( m_TxShiftReg&0xC000 )
			{
				case 0x4000:			/*dot */
					m_CWState = 1;
					m_CWtimer = 1*m_CWIDSpeed;
					symb = SYM_ON;
					break;
				case 0x8000:			/*dash */
					m_CWState = 1;
					m_CWtimer = 3*m_CWIDSpeed;
					symb = SYM_ON;
					break;
				case 0xC000:			/*inter char space */ 
					m_CWState = 2;
					m_CWtimer = 2*m_CWIDSpeed-1;
					symb = SYM_OFF;
					break;
				default:
					symb = SYM_OFF;
					break;
			}
			m_TxShiftReg = m_TxShiftReg<<2;	
			break;
		case 1:						/*On time state */
			if( --m_CWtimer <= 0 )
			{
				m_CWtimer = 1*m_CWIDSpeed - 1;	/*inter symbol time */
				if(m_CWtimer>=1)
					m_CWState = 2;
				else
					m_CWState = 0;
				symb = SYM_OFF;
			}
			else
				symb = SYM_NOCHANGE;
			break;
		case 2:						/*Off time state */
			if( --m_CWtimer <= 0 )
				m_CWState = 0;
			break;
	}
	m_Lastsymb = symb;
	return symb;
}

/*==================================================================*/
/* called every symbol time to get next BPSK symbol and get the	    */
/* next character from the character Queue if no more symbols	    */
/* are left to send.						    */
/*==================================================================*/
char GetNextBPSKSymbol(void)
{
	char symb;
	char ch;
	symb = m_Lastsymb; 

	if( m_TxShiftReg == 0 )
	{
		if( m_AddEndingZero )		/* if is end of code */
		{
			symb = SYM_P180;		/* end with a zero */
			m_AddEndingZero = FALSE;
		}
		else
		{

			ch = GetChar();			/*get next character to xmit */
			switch( ch ) {
				case TXON_CODE:
					symb = SYM_ON;
					break;
				case TXTOG_CODE:
					symb = SYM_P180;
					break;
				case TXOFF_CODE:
					symb = SYM_OFF;
					break;
				default: /*get next VARICODE codeword to send */
					m_TxShiftReg = VARICODE_TABLE[ ch&0xFF ];
					symb = SYM_P180;	/*Start with a zero */
					break;
			}
		}
	}
	else			/* is not end of code word so send next bit */
	{
		if( m_TxShiftReg&0x8000 )
			symb = SYM_NOCHANGE;
		else
			symb = SYM_P180;
		m_TxShiftReg = m_TxShiftReg<<1;	/*point to next bit */
		if( m_TxShiftReg == 0 )			/* if at end of codeword */
			m_AddEndingZero = TRUE;		/* need to send a zero nextime */
	}
	m_Lastsymb = symb;
	return symb;
}

		
/*==================================================================*/
/* called every symbol time to get next QPSK symbol and get the	    */
/* next character from the character Queue if no more symbols	    */
/* are left to send.						    					*/
/*==================================================================*/
char GetNextQPSKSymbol(void)
{
	char symb;
	extern const unsigned char ConvolutionCodeTable[];
	char ch;
	symb = ConvolutionCodeTable[m_TxShiftReg&0x1F];	/*get next convolution code */
	m_TxShiftReg = m_TxShiftReg<<1;
	if( m_TxCodeWord == 0 )			/*need to get next codeword */
	{
		if( m_AddEndingZero )		/*if need to add a zero */
		{
			m_AddEndingZero = FALSE;	/*end with a zero */
		}
		else
		{
			ch = GetChar();			/*get next character to xmit */
			switch( ch ) {
				case TXON_CODE:
					symb = SYM_ON;
					break;
				case TXTOG_CODE:
					m_TxCodeWord = 0;
					break;
				case TXOFF_CODE:
					symb = SYM_OFF;
					break;
				default:
					/*get next VARICODE codeword to send */
					m_TxCodeWord = VARICODE_TABLE[ ch&0xFF ];
			}
		}
	}
	else
	{
		if(m_TxCodeWord&0x8000 )
		{
			m_TxShiftReg |= 1;
		}
		m_TxCodeWord = m_TxCodeWord<<1;
		if(m_TxCodeWord == 0)
			m_AddEndingZero = TRUE;	/*need to add another zero */
	}
	return symb;
}

/*==================================================================*/
/* called every symbol time to get next Tune symbol		    		*/
/*==================================================================*/
char GetNextTuneSymbol(void)
{
	char symb;
	char ch;
	ch = GetChar();			/*get next character to xmit */
	switch( ch )
	{
		case TXON_CODE:
			symb = SYM_ON;
			break;
		default:
			symb = SYM_OFF;
			break;
	}
	return symb;
}

/*==================================================================*/
/*get next character/symbol depending on TX state.		    		*/ 
/*==================================================================*/
char GetChar()
{
	char ch, tempchar;
	extern char CharBfr;
	extern Flag_type Flag;
	extern uchar OldFIGS;
	Nop();
	switch( m_TXState )
	{
		case TX_OFF_STATE:		/*is receiving */
			ch = TXOFF_CODE;	/*else turn off */
			m_NeedShutoff = FALSE;
			break;
		case TX_TUNE_STATE:
			ch = TXON_CODE;			/* steady carrier */
			if(	m_NeedShutoff)
			{
				if(	m_NeedCWid  )
				{
					m_TXState = TX_CWID_STATE;
					m_SavedMode = m_PSKmode;
					m_PSKmode = CW_MODE;
					m_NeedCWid = FALSE;
					m_AmblePtr = 0;
					ch = TXOFF_CODE;
				}
				else
				{
					m_TXState = TX_OFF_STATE;
					m_AmblePtr = 0;
					ch = TXOFF_CODE;
					m_NeedShutoff = FALSE;
				}
			}
			break;
		case TX_POSTAMBLE_STATE:		/* ending sequence */
			if (m_PSKmode != RTTY_MODE) {
				if( !(ch = m_Postamble[m_AmblePtr++] ) || m_NoSquelchTail)
				{
					m_NoSquelchTail = FALSE;
					if(	m_NeedCWid  )
					{
						m_TXState = TX_CWID_STATE;
						m_SavedMode = m_PSKmode;
						m_PSKmode = CW_MODE;
						m_NeedCWid = FALSE;
						m_AmblePtr = 0;
						ch = TXOFF_CODE;
					}
					else
					{
						m_TXState = TX_OFF_STATE;
						m_AmblePtr = 0;
						ch = TXOFF_CODE;
						m_NeedShutoff = FALSE;
						PTT_On = 0; 
					}
				}
				break;
			}
			else {
 				ch = 0x1F;
				m_TXState = TX_OFF_STATE;
				m_AmblePtr = 0;
				PTT_On = 0; 
				break;
			}
		case TX_PREAMBLE_STATE:			/*starting sequence */
			if (m_PSKmode == RTTY_MODE) {
				ch = RTTY_Preamble[m_AmblePtr++];
				Nop();
			}
			else {
			ch = m_Preamble[m_AmblePtr++];
			}
			if( !ch ) 	/* at end of preamble */
			{
				m_TXState = TX_SENDING_STATE;
				m_AmblePtr = 0;
				if (m_PSKmode != RTTY_MODE) ch = TXTOG_CODE; 
			} 
			break;
		case TX_CWID_STATE:				/* if sending CW ID */
			if( m_AmblePtr >= strlen(m_CWIdString) ) /* not sure this is working */
			{
				m_PSKmode = m_SavedMode;
				m_TXState = TX_OFF_STATE;
				m_AmblePtr = 0;
				ch = TXOFF_CODE;
				m_NeedShutoff = FALSE;
				PTT_On = 0;
			}
			else
			{
				m_PSKmode = CW_MODE;
				ch = m_CWIdString[m_AmblePtr++];
			}
			break;
		case TX_SENDING_STATE:		/*if sending text from TX window */
			ch = GetTxChar();	/* get next character from XmitQue */
			Nop();				
			switch (ch)
			{
				case GET_MYCALL:
					Insert_MyCall();
					ch = 0;
					break;
				case GET_THEIRCALL:
					Insert_TheirCall();
					ch = 0;
					break;
				case GET_BEACON_COUNT:
					Insert_Beacon_Count();
					ch = 0;
					break;
				case TXOFF_CODE:
					Flag.TglRnT = TRUE;
					ch = 0;
					break;
				case TXTOG_CODE:
					if (m_NeedShutoff) {
						m_TXState = TX_POSTAMBLE_STATE;
						m_AmblePtr = 0;
					}
					break;
				default:
					CharBfr = ch;
					Flag.DAV = TRUE;
					break;
			}
			break;
		case TX_PAUSED_STATE:
			ch = TXTOG_CODE;
			break;
	}
	Nop();
	return( ch );
}
/*==================================================================*/
/*		TX Queueing routines				    					*/
/*==================================================================*/
void PutTxQue(char txchar, int cntrl)
{
	if( cntrl )	/*is a tx control code */
	{
		switch( txchar )
		{
			case TX_CNTRL_AUTOSTOP:
				m_TempNeedShutoff = TRUE;
				if( m_TXState==TX_TUNE_STATE )
					m_NeedShutoff = TRUE;
				break;
			case TX_CNTRL_ADDCWID:
				m_TempNeedCWid = TRUE;
				break;
			case TX_CNTRL_NOSQTAIL:
				m_TempNoSquelchTail = TRUE;
				break;
		}
	}
	else		/*is a character to xmit */
	{
		if( (txchar != BACK_SPACE_CODE) || (m_pHead==m_pTail) )
		{
			XmitBuffer[m_pHead++] = txchar;	
			if( m_pHead >= TX_BUF_SIZE )
				m_pHead = 0;
		}
		else	/*see if is a backspace and if can delete it in the queue  */
		{
			if(--m_pHead < 0 ) m_pHead = 0;		/*look at last character in queue */
				
			if( XmitBuffer[m_pHead] == BACK_SPACE_CODE)
			{				/*if another backspace, leave it there */
				if(++m_pHead >= TX_BUF_SIZE )
					m_pHead = 0;
				XmitBuffer[m_pHead++] = txchar;
				if( m_pHead >= TX_BUF_SIZE )
					m_pHead = 0;
			}
		}
	}

}

/*==================================================================*/
char GetTxChar()
{
	char ch;

	if( m_pHead != m_pTail )	/*if something in Queue */
	{
		ch = XmitBuffer[m_pTail++] & 0x00FF;
		if( m_pTail >= TX_BUF_SIZE )
			m_pTail = 0;
	}
	else
		ch = TXTOG_CODE;		/* if que is empty return TXTOG_CODE */
	if(m_TempNeedShutoff)
	{
		m_TempNeedShutoff = FALSE;
		m_NeedShutoff = TRUE;
	}
	if(m_TempNeedCWid)
	{
		m_TempNeedCWid = FALSE;
		m_NeedCWid = TRUE;
	}
	if(m_TempNoSquelchTail)
	{
		m_TempNoSquelchTail = FALSE;
		m_NoSquelchTail = TRUE;
	}
	return ch; 
}

/*==================================================================*/
void ClrQue( void )
{
	m_pTail = m_pHead = 0;
	m_NoSquelchTail = FALSE;
	m_TempNeedCWid = FALSE;
	m_TempNeedShutoff = FALSE;
	m_TempNoSquelchTail = FALSE;
}



int GetTXCharsRemaining( void )
{
	int num = m_pHead - m_pTail;
	if( num < 0 )
		num = num + TX_BUF_SIZE;
	return num;
}

void SetCWIDSpeed(long speed)
{
	if( (speed>=1) && (speed<=4) )
		m_CWSetSpeed = speed;
	else
		m_CWSetSpeed = CW_SPEED;
}

/*==================================================================*/
char PullTXQue ()
{
	char ch;

	if( m_pHead != m_pTail )	/*if something in Queue */
	{
		ch = XmitBuffer[m_pTail++] & 0x00FF;
		if( m_pTail >= TX_BUF_SIZE )
			m_pTail = 0;
	}
	else
		ch = 0;		/* if que is empty return 0 */
		return ch;
}
