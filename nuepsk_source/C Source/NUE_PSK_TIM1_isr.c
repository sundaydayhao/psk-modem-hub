/*======================================================================

		NUE_PSK TIM1 isr

		Milton E. Cram	W8NUE
		Began On:		12/1/2006
		Current Date:	3/4/2007
		Last Mod		3/4/2007

======================================================================*/

#include	"NUE_PSK_Header.h"

/*=====================================================================*/		

void __attribute__((__interrupt__)) _T1Interrupt( void )	{

	extern Flag_type 	Flag;
	extern Status_type	Status;
	extern int			*pADC_Buffer;
	extern int			ADC_Bfr_Offset;
	extern int			Intrpt_Count;
	extern int			Encoder_Count;
	extern int			Seconds_Count;
	extern int			Timer_Count;
	extern int			ProcRTTY;
	extern int			m_PSKmode;

	if (++Seconds_Count == 8000) {
		Status.SECOND = 1;
		Seconds_Count = 0;
	}
	if (Flag.RXTune ==1) RXTuneCnt++;
	switch (m_TXState) {
		case TX_OFF_STATE:
			++Encoder_Count;
			if (!Flag.RXTune) {
				pADC_Buffer[Intrpt_Count] = Input_AD1();
				++Intrpt_Count;
				if (Intrpt_Count == ADC_BUFFER_LENGTH) {
					if (m_PSKmode == BPSK_MODE) Flag.ProcPSK = 1;
					else ProcRTTY = 1;
					ADC_Bfr_Offset = 0;
				}
				if (Intrpt_Count == ADC_BUFFER_LENGTH2) {
					Intrpt_Count = 0;
					if (m_PSKmode == BPSK_MODE) Flag.ProcPSK = 1;
					else ProcRTTY = 1;
					ADC_Bfr_Offset = ADC_BUFFER_LENGTH;
				}
			}
			if (Encoder_Count == ENCODER_TIME) {
				Encoder_Count = 0;
				Status.Fifty_ms = 1;
			}
			break;
		case TX_PREAMBLE_STATE:
		case TX_SENDING_STATE:
		case TX_CWID_STATE:
		case TX_TUNE_STATE:
		case TX_POSTAMBLE_STATE:
			 CalcPSK(); 
			break;
		case TX_MACRO_REC:
			break;
	/* do nothing if in Setup, or Macro Recording (TX_MACRO_REC) */
	}

	IFS0bits.T1IF = 0;  /* clear Timer1 Interrupt Flag */

}

/*=====================================================================*/		

void __attribute__((__interrupt__)) _T4Interrupt( void )	{

	extern int			Timer_Count;

	++Timer_Count; 

	IFS1bits.T4IF = 0;  /* clear Timer4 Interrupt Flag */
}

/*=====================================================================*/		

void __attribute__((__interrupt__)) _T7Interrupt( void )	{

	extern int 	RunBeacon;

	RunBeacon = 1;

	T67IF = 0;  /* clear Timer7 Interrupt Flag */
}

/*=====================================================================*/		
	
