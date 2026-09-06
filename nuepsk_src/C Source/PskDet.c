/* Converted to C code by Milt Cram, W8NUE--June 2006   */
/* Last modified 5/1/07 */

/* ////////////////////////////////////////////////////////////////////      */
/* PskDet.c : implementation of the CPSKDet class.                          */
/* ////////////////////////////////////////////////////////////////////      */
/* ////////////////////////////////////////////////////////////////////      */
/* PSK31Core Library for transmission and reception of PSK31 signals         */
/*    using a PC soundcard  or .wav files.                                   */
/*	               Copyright 2000, Moe Wheatley, AE4JY                     */
/*                                                                           */
/* This library is free software; you can redistribute it and/or             */
/* modify it under the terms of the GNU Lesser General Public                */
/* License as published by the Free Software Foundation; either              */
/* version 2.1 of the License, or (at your option) any later version.        */
/*                                                                           */
/* This library is distributed in the hope that it will be useful,           */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         */
/* Lesser General Public License for more details.                           */
/*                                                                           */
/* You should have received a copy of the GNU Lesser General Public          */
/* License along with this library; if not, write to the Free Software       */
/* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */
/* http://www.gnu.org/copyleft/lesser.html                                   */
/* ////////////////////////////////////////////////////////////////////      */
/*                                                                           */

#include	"NUE_PSK_Header.h"
#include <math.h>
#include "PskDet.h"
#include "PskTables.h"
#include "Blackman.h"

/* define some constants  */
#define PHZ_180_BMIN	(0.0)			/* 0         */
#define PHZ_180_BMAX	(PI2/2.0)		/* Pi        */
#define PHZ_0_BMIN	(PI2/2.0)		/* Pi        */
#define PHZ_0_BMAX	(PI2)			/* 2Pi       */

#define SYMBOL_RATE31 	31.25	/* 31.25 Symbols per Second */
#define SYMBOL_RATE63 	62.5	/* PSK63 62.50 Symbols per Second */
#define SYMBOL_RATERTTY 45.45	/* RTTY 45.45 Symbols per Second */

/*                                                     */
#define PHZ_180_QMIN	(PI2/8.0)		/* Pi/4      */
#define PHZ_180_QMAX	(3.0*PI2/8.0)	/* 3Pi/4     */
#define PHZ_0_QMIN	(5.0*PI2/8.0)	/* 5Pi/4     */
#define PHZ_0_QMAX	(7.0*PI2/8.0)	/* 7Pi/4     */
#define Ts (.032+.00000)			/* Ts == symbol period */
/*#define Ts (.032+.000072)	*/	/* to simulate off freq soundcards */

/*define the PSK31 symbols  */
#define SYM_NOCHANGE 0	/*Stay the same phase  */
#define SYM_P90 1		/*Plus 90  deg  */
#define SYM_P180 2	/*Plus 180 deg  */
#define SYM_M90 3		/*Minus 90 deg  */

#define AFC_OFF 0
#define AFC_ON 1
#define AFC_TIMELIMIT 20	/* 2.5 seconds	*/
#define AFC_FTIMELIMIT 4	/* 0.5 seconds	*/

#define NLP_K (100.0)		/*narrow phase derived afc constans  */
#define FNLP_K (30.0)

/* //////////////////////////////////////////////////////////////////// */
/* Construction/Destruction                                             */
/* //////////////////////////////////////////////////////////////////// */

void ResetModem ( int mode ){

	extern int count, char_count;
	extern int m_PSKmode, m_PSK63Mode;
	extern Flag_type Flag;
	extern int m_TXState;
	extern int ProcRTTY;
	extern double m_PSKPeriodUpdate;
	extern double m_SymbolRate;

	SetRXPSKMode ( mode );
	Flag.ProcFFT = 0;

	InitPSK (8000);
	switch (mode) {
		case BPSK_MODE:
		case QPSKU_MODE:
		case QPSKL_MODE:
			Flag.ProcPSK = 0;
			CPSKInitDet ();
			PSKTXSetup ();
			InitPSKMod ();
			m_SymbolRate = SYMBOL_RATE31;
			break;
		case RTTY_MODE:
			ProcRTTY = 0;
			m_SymbolRate = SYMBOL_RATERTTY;
			Init_FIR_RTTY ();
			break;
	}
	m_PSKPeriodUpdate = 1/m_SymbolRate;
	count = 0;
	char_count = 0;
	m_TXState = TX_OFF_STATE;
	Reset_LCD_buffer ();
	Set_Display_XYAddr (7,0);
}
/* //////////////////////////////////////////////////////////////////// */

void CPSKInitDet( void )
{
	extern Flag_type Flag;
	extern double NCO_Frequency;
	extern float Sample_Frequency;
	extern int m_PSKmode;
	m_SampleClkAdj = 0;
	m_IQPhzIndex = 0;
	m_IMDValid = FALSE;
	m_FreqError = 0.0;
	m_SquelchSpeed = 75;
	m_SQThresh = 50;
	m_NCOphzinc = (PI2*(double)NCO_Frequency/(double)Sample_Frequency);
	m_AFClimit = 50.0*PI2/(double)Sample_Frequency;
	m_AFCmax =  m_NCOphzinc + m_AFClimit;
	m_AFCmin =  m_NCOphzinc - m_AFClimit;
	SetSampleClkAdj(0);
	m_TXState = TX_OFF_STATE;
	PTT_tris = 0;
	PTT_On	= 0; 
	Flag.DispFreq = 0;
	Nop();
}

/* ///////////////////////////////////////////////////////////////  */
/*       Initialize PskDet buffers and pointers                     */
/*/////////////////////////////////////////////////////////////     */
void InitPSK( int Fs )
{
	unsigned int wTemp;
	int i;
	extern float Sample_Frequency;
	int j;
	Sample_Frequency = Fs;
	for( j=0; j<2048; j++ )	  /* init inverse varicode lookup decoder table */
	{
		m_VaricodeDecTbl[j] = 0;
		for( i=0; i<256;i++)
		{
			wTemp = VARICODE_TABLE[i];
			wTemp >>= 4;
			while( !(wTemp&1) )
				wTemp >>= 1;
			wTemp >>= 1;
			if( wTemp == j)
				m_VaricodeDecTbl[j] = (unsigned char) i;
		}
	} 
/*                                    */

	ResetDetector();
	SetSampleClkAdj(0);

	m_SQLevel = 50;
	m_BitPhasePos = 0.0;
	m_BitAcc = 0;
	m_LastBitZero = FALSE;
	m_SampCnt = 0;
	m_OnCount = 0;
	m_OffCount = 0;
/* Init a bunch of "static" variables used in various member fuctions */
	m_AGCave = 0.0;
	m_VcoPhz = 0.0;
	m_PkPos = 0;
	m_NewPkPos = 5;
	m_BitPos = 0;
	m_I1 = 0.0;
	m_I0 = 0.0;
	m_Q1 = 0.0;
	m_Q0 = 0.0;
	m_DevAve = .4;
	m_z1.x = 0.0; m_z1.y = 0.0;
	m_z2.x = 0.0; m_z2.y = 0.0;
	m_FferrAve = 0.0;
	m_FferrAve = 0.0;
	m_QFreqError = 0.0;
	m_LastPkPos = 0;
	m_SQOpen = FALSE;
	m_AFCTimer = 0;
	m_AFCCaptureOn = FALSE;
	m_FastAFCMode = FALSE;
	m_NLPk = NLP_K;
	Set_Display_XYAddr (7,0);
}

/* ////////////////////////////////////////////////////////////////////  */
/* Called to adjust the sample clock                                     */
/* ////////////////////////////////////////////////////////////////////  */
void SetSampleClkAdj(int ppm)
{
	extern float Sample_Frequency;
	extern double NCO_Frequency;
	m_SampleClkAdj = (Sample_Frequency*ppm)/1000000;
	m_SampleFreq = (double)(Sample_Frequency + m_SampleClkAdj);		/*adj sample rate */
	m_NCOphzinc = (double)(PI2/m_SampleFreq)*(double)NCO_Frequency; /*new center freq inc */
	m_BitPhaseInc = 16.0/m_SampleFreq;		/*bit oversampling period */
}

/* ////////////////////////////////////////////////////////////////////  */
/* Called to get the symbol vectors                                      */
/* ////////////////////////////////////////////////////////////////////  */
void GetVectorData(long *VectData)
{
int i;
	for(i=0; i<16; i++)
		VectData[i] = m_IQPhaseArray[i];
}

/* ////////////////////////////////////////////////////////////////////  */
/* Called to get the symbol vectors                                      */
/* ////////////////////////////////////////////////////////////////////  */
void GetSyncData(long *SyncData)
{
int i;
	for(i=0; i<16; i++)
		SyncData[i] = m_SyncArray[i];
}

/* ////////////////////////////////////////////////////////////////////  */
/* Called to change the Rx frequency                                     */
/* ////////////////////////////////////////////////////////////////////  */
void SetRXFrequency(double freq)
{
		extern double m_SampleFreq,m_NCOphzinc;

		m_AFCCaptureOn = TRUE; 
		m_NCOphzinc = PI2*freq/m_SampleFreq;
		m_FferrAve = 0.0;
		m_FperrAve = 0.0;
		if(m_FastAFCMode)
			m_AFCTimer = AFC_FTIMELIMIT; 
		else
			m_AFCTimer = AFC_TIMELIMIT; /* 2.5 sec countdown timer for AFC */
/* calculate new limits around new receive frequency                     */
		m_AFCmax =  m_NCOphzinc + m_AFClimit;
		m_AFCmin =  m_NCOphzinc - m_AFClimit;
		if(m_AFCmin<=0.0)
			m_AFCmin = 0.0;
		m_Pcnt = 0;
		m_Ncnt = 0;
}

/* ////////////////////////////////////////////////////////////////////  */
/* Called to change the AFC limit                                        */
/* ////////////////////////////////////////////////////////////////////  */
void SetAFCLimit(int limit)
{
	if(limit==0)
		m_AFCmode = AFC_OFF;
	else
		m_AFCmode = AFC_ON;
	if(limit==3000)
		m_FastAFCMode = TRUE;
	else
		m_FastAFCMode = FALSE;
	m_AFClimit = (double)limit*PI2/m_SampleFreq;
/* calculate new limits around current receive frequency                 */
	m_AFCmax =  m_NCOphzinc + m_FreqError + m_AFClimit;
	m_AFCmin =  m_NCOphzinc + m_FreqError - m_AFClimit;
	if(m_AFCmin<=0.0)
		m_AFCmin = 0.0;
	if(m_FastAFCMode)
	{
		m_NLPk = FNLP_K;
	}
	else
	{
		m_NLPk = NLP_K;
	}
}

/* ////////////////////////////////////////////////////////////////////  */
/* Called to reset the detector before going into xmit                   */
/* ////////////////////////////////////////////////////////////////////  */
void ResetDetector( void )
{
int i;

	for(i=0; i<16; i++)
	{
		m_SurvivorStates[i].Pathdistance = 0.0;
		m_SurvivorStates[i].BitEstimates = 0;
		m_IQPhaseArray[i] = 1;
	}
	for(i=0; i<21; i++)
	{
		m_SyncAve[i] = 0.0;				/* initialize the array */
		ViterbiDecode( 3.0*PI2/4.0);	/* init the Viterbi decoder      */
	}
	m_SQLevel = 10;
	m_ClkErrCounter = 0;
	m_ClkErrTimer = 0;
	m_ClkError = 0;
	m_DevAve = 90.0;
}

/* ////////////////////////////////////////////////////////////////////  */
/* Main routine called to process the next block of data                 */
/*  We will use a block of length ADC_BUFFER_LENGTH						 */
/* ////////////////////////////////////////////////////////////////////  */
void ProcPSKDet()
{
	extern float Sample_Frequency; 
	extern double NCO_Frequency; 
	extern double m_NCOphzinc; 
	int  i, j;
	int	 mod16_8 = 16;
	/* extern int ADC_Buffer[ADC_BUFFER_LENGTH2]; */
	extern int *pADC_Buffer;
	extern int Temp_Bufr[];
	extern const float Blackman[];
	extern int ADC_Bfr_Offset;
	extern Flag_type Flag;
	static long			NCO_phz;
	static long			Product_I, Product_Q;
	static int			FFTcount;
	static int			FFT_Index;
	extern fractcomplex		FFT_buffer[FFT_BLOCK_LENGTH];
	int		*p_real = &FFT_buffer[0].real;
	int		*p_imag = &FFT_buffer[0].imag;
	int		*p_temp	= &Temp_Bufr[0];
	static fractional 	FIR_sample_I,
						FIR_sample_Q,
						FIR_Output_I,
						FIR_Output_Q,
						NCO_I, NCO_Q,
						ADC_I,
						temp_I, temp_Q,
						FIROut_I_bit,
						FIROut_Q_bit,
						FIROut_I_freq,
						FIROut_Q_freq,
						Moe_35_sample_I_1 [ 4 ],
						Moe_35_sample_I_2 [ 4 ],
						Moe_35_sample_Q_1 [ 4 ],
						Moe_35_sample_Q_2 [ 4 ];
	extern double 		m_NCOphzinc,
						m_FreqError;
	static int	Index_1, Index_2;

	if(m_PSK63Mode)
		mod16_8 = 8;

/*  AFC timer initialized in SetRXFrequency, decremented here at */
/*	.125ms * ADC_BUFFER_LENGTH intervals, (128msec interval) 	*/

	if(	m_AFCTimer )
	{
		if(--m_AFCTimer <= 0)
		{
			m_AFCTimer = 0;
			m_AFCCaptureOn = FALSE;

/* calculate new limits around latest receive frequency   */
			m_AFCmax =  m_NCOphzinc + m_AFClimit;
			m_AFCmin =  m_NCOphzinc - m_AFClimit;
			NCO_Frequency = m_NCOphzinc*(double)Sample_Frequency/PI2;
			if(m_AFCmin<=0.0)
				m_AFCmin = 0.0;
		}
		else
			m_AFCCaptureOn = TRUE;
	}

	/* Process a block of input data */
	for (i=0; i < ADC_BUFFER_LENGTH; i++) {

	/* ADC_I	=	ADC_Buffer[ADC_Bfr_Offset+i]; */
	ADC_I	= 	pADC_Buffer[ADC_Bfr_Offset+i];

	/*  If not busy processing/displaying FFT, get new FFT data */

	if (ADC_Bfr_Offset == 0) {
		if (i<512) {
			*(p_temp +i) = (int)(((float)(ADC_I/2))*Blackman[i]);
			*(p_real +i) = 0;
		}
		else *(p_temp +i -512) += (int)(((float)(ADC_I/2))*Blackman[i]);
	}
	else {
		if (i<512) *(p_temp+i) += (int)(((float)(ADC_I/2))*Blackman[1023-i]);
		else *(p_temp+i-512) += (int)(((float)(ADC_I/2))*Blackman[1023-i]);
		if (i == ADC_BUFFER_LENGTH -1) {
			Flag.ProcFFT = 1;
			for(j=0; j<FFT_BLOCK_LENGTH; j++) *(p_real+j) = *(p_temp+j);
		}
	}
	NCO_phz	+= (long)(KCONV*(m_NCOphzinc+m_FreqError)); 

/*	NCO_phz	= NCO_phz +(long)(KCONV*m_NCOphzinc); */

	NCO_Q =	( Sine_lookup ( (   NCO_phz >> 4 )           & 0xFFF ) ) ; 
	NCO_I =	( Sine_lookup ( ( ( NCO_phz >> 4 ) + 0x400 ) & 0xFFF ) ) ;
	
	Product_I	= (long)	ADC_I * (long) NCO_I;
	Product_Q	= (long)	ADC_I * (long) NCO_Q;

/*	IFS0bits.T1IF = 0; */

	FIR_sample_I =  (int) ( Product_I>>15 ); 	
	FIR_sample_Q = 	(int) ( Product_Q>>15 ); 

/*----------------------------------------------------*/

	Moe_35_sample_I_1 [ Index_1 ]	=	FIR_sample_I;	
	Moe_35_sample_Q_1 [ Index_1 ]	=	FIR_sample_Q;
	
	Index_1 = (Index_1+1) % 4;

	if (!Index_1)	{
		
		FIRDecimate ( 	1,
						&Moe_35_sample_I_2 [ Index_2 ],
						&Moe_35_sample_I_1 [ 0 ],
						&Filter_I1,
						4 );

		FIRDecimate ( 	1,
						&Moe_35_sample_Q_2 [ Index_2 ],
						&Moe_35_sample_Q_1 [ 0 ],
						&Filter_Q1,
						4 );

		/* need to decimate by 2 if in PSK63 mode */		
		if (m_PSKmode == PSK63_MODE) Index_2 = (Index_2+1) % 2; 
		else Index_2 = (Index_2+1) % 4;

		if (!Index_2)	{
		
			FIRDecimate ( 	1,
							&FIR_Output_I,
							&Moe_35_sample_I_2 [ 0 ],
							&Filter_I2,
							4 );

			FIRDecimate ( 	1,
							&FIR_Output_Q,
							&Moe_35_sample_Q_2 [ 0 ],
							&Filter_Q2,
							4 );
			temp_I = FIR_Output_I;
			temp_Q = FIR_Output_Q;				
			FIR(1,&FIROut_I_bit,  &FIR_Output_I, &Filter_I3);
			FIR(1,&FIROut_Q_bit,  &FIR_Output_Q, &Filter_Q3);
			FIR(1,&FIROut_I_freq, &temp_I, &Filter_I4);
			FIR(1,&FIROut_Q_freq, &temp_Q, &Filter_Q4);

	
/* here at Fs/16 == 500.0 Hz or 1000.0 Hz rate with latest sample in acc.  */
/* Matched Filter the I and Q data and also a frequency error filter       */
/*	filter outputs in variables m_FreqSignal and m_BitSignal.  */

				m_FreqSignal.x = FIROut_I_bit;
				m_FreqSignal.y = FIROut_Q_bit;
				m_BitSignal.x = FIROut_I_freq;
				m_BitSignal.y = FIROut_Q_freq;

/* Perform AGC operation  */
				CalcAGC( m_FreqSignal );

/* Calculate frequency error and tweak NCO */

					CalcFreqError(m_FreqSignal); 

/* Bit Timing synchronization  */
				if( SymbSync(m_BitSignal) )
					DecodeSymb( m_BitSignal);
			}
		}
	}
}

/* ////////////////////////////////////////////////////////////////////  */
/*  Process RTTY signal                                                  */
/* ////////////////////////////////////////////////////////////////////  */

void ProcRTTYDet ( void ){

	extern float Sample_Frequency; 
	extern double NCO_Frequency; 
	extern double m_NCOphzinc; 
	int  i, j;
	/* extern int ADC_Buffer[ADC_BUFFER_LENGTH2]; */
	extern int 			*pADC_Buffer;
	extern int 			ADC_Bfr_Offset;
	extern int			Temp_Bufr[];
	extern unsigned int RTTY_Shift;
	extern Flag_type 	Flag;
	extern RTTY_type	Rtty;
	static long			NCO_phz;
	static long			Fhi, Flo;
	static long			Product_I, Product_Q;
	static float		SQ, SGain;
	static int			FFTcount;
	static int			FFT_Index;
	static uchar		tempcount;
	uchar				ch;
	char 				buffer[16];
	extern fractcomplex		FFT_buffer[FFT_BLOCK_LENGTH];
	int		*p_real = &FFT_buffer[0].real;
	int		*p_imag = &FFT_buffer[0].imag;
	int		*p_temp	= &Temp_Bufr[0];
	static fractional 	FIR_sample_I,
						FIR_sample_Q,
						FIR_Output_I,
						FIR_Output_Q,
						NCO_I, NCO_Q,
						ADC_I,
						Sample_I_1 [ 4 ],
						Sample_Q_1 [ 4 ],
						Sample_I_2 [ 2 ],
						Sample_Q_2 [ 2 ];
	static float		mag;

	extern double 		m_NCOphzinc,
						m_FreqError;
	static int			Index_1, Index_2;
	static uchar		RTTY_Sync,
						Syncing,
						position_count,
						bit_count,
						datareg,
						space_count,
						sig;
	static float		R, M,
						temp_I,temp_Q,
						Iold,Qold,ftemp; 
	static int			freq;
	static long			sigma;
	SGain = .005; /* use 200 msec time constant on squelch */

/*  AFC timer initialized in SetRXFrequency, decremented here at */
/*	.125ms * ADC_BUFFER_LENGTH intervals, (128msec interval) 	*/

/*	if(	m_AFCTimer )
	{
		if(--m_AFCTimer <= 0)
		{
			m_AFCTimer = 0;
			m_AFCCaptureOn = FALSE; */

/* calculate new limits around latest receive frequency   */
		/*	m_AFCmax =  m_NCOphzinc + m_AFClimit;
			m_AFCmin =  m_NCOphzinc - m_AFClimit; */
			NCO_Frequency = m_NCOphzinc*(double)Sample_Frequency/PI2;
/*			if(m_AFCmin<=0.0)
				m_AFCmin = 0.0;
		}
		else
			m_AFCCaptureOn = TRUE;
	} */

	/* Process a block of input data */
	for (i=0; i < ADC_BUFFER_LENGTH; i++) {

	ADC_I	= 	pADC_Buffer[ADC_Bfr_Offset+i];

	/*  If not busy processing/displaying FFT, get new FFT data */

	if (ADC_Bfr_Offset == 0) {
		if (i<512) {
			*(p_temp +i) = (int)(((float)(ADC_I/2))*Blackman[i]);
			*(p_real +i) = 0;
		}
		else *(p_temp +i -512) += (int)(((float)(ADC_I/2))*Blackman[i]);
	}
	else {
		if (i<512) *(p_temp+i) += (int)(((float)(ADC_I/2))*Blackman[1023-i]);
		else *(p_temp+i-512) += (int)(((float)(ADC_I/2))*Blackman[1023-i]);
		if (i == ADC_BUFFER_LENGTH -1) {
			Flag.ProcFFT = 1;
			for(j=0; j<FFT_BLOCK_LENGTH; j++) *(p_real+j) = *(p_temp+j);
		}
	}
	NCO_phz	+= (long)(KCONV*(m_NCOphzinc+m_FreqError)); 

	NCO_Q =	( Sine_lookup ( (   NCO_phz >> 4 )           & 0xFFF ) ) ; 
	NCO_I =	( Sine_lookup ( ( ( NCO_phz >> 4 ) + 0x400 ) & 0xFFF ) ) ;
	
	Product_I	= (long)	ADC_I * (long) NCO_I;
	Product_Q	= (long)	ADC_I * (long) NCO_Q;

	FIR_sample_I =  (int) ( Product_I>>15 ); 	
	FIR_sample_Q = 	(int) ( Product_Q>>15 ); 

/*----------------------------------------------------*/

	Sample_I_1 [ Index_1 ]	=	FIR_sample_I;	
	Sample_Q_1 [ Index_1 ]	=	FIR_sample_Q;
	
	Index_1 = (Index_1+1) % 4;

	if (!Index_1)	{
		
		FIRDecimate ( 	1,
						&Sample_I_2 [ Index_2 ],
						&Sample_I_1 [ 0 ],
						&Filter_I3,
						4 );

		FIRDecimate ( 	1,
						&Sample_Q_2 [ Index_2 ],
						&Sample_Q_1 [ 0 ],
						&Filter_Q3,
						4 );

/* at this point we have 2000 samples per second sample rate */
		Index_2 = (Index_2+1) % 2;
		if (!Index_2)	{
		
			FIRDecimate 	( 1,
							&FIR_Output_I,
							&Sample_I_2 [ 0 ],
							&Filter_I4,
							2 );

			FIRDecimate 	( 1,
							&FIR_Output_Q,
							&Sample_Q_2 [ 0 ],
							&Filter_Q4,
							2 );

		temp_I = (float) FIR_Output_I;
		temp_Q = (float) FIR_Output_Q;
		
		M =  temp_I * temp_I + temp_Q * temp_Q;
		R =  KF*(temp_I*Qold - Iold*temp_Q);
		mag = (1-SGain)*mag +SGain*sqrt(M);


/* sampling rate = 1000sps (1.0ms per sample) */
/* 45.45 baud, 22msec per bit or 22 samples per bit */
		if (M !=0.) {
			ftemp = 0.6*ftemp + 0.4*(R/M);
		}
		freq = (int)ftemp;

		sigma = (abs(freq)-85)*(abs(freq)-85);
		SQ = (1-SGain)*SQ + SGain* ((float)sigma); 
		if (SQ < 2000) m_SQOpen = TRUE;
		else m_SQOpen = FALSE;

	/*	if (++tempcount == 50) {
			tempcount = 0;
			sprintf (buffer,"%10.0f",mag);
			Write_String_Positioned_LCD1_Display (5,2,buffer);
			sprintf (buffer,"%10.0f",SQ);
			Write_String_Positioned_LCD1_Display (6,2,buffer); 
		} */

		
		Iold = temp_I;
		Qold = temp_Q;

		/* convert frequency to Mark/Space Bits */
		if (!Rtty.REVERSE) {
			if (freq > 40) 	sig = 1;
			if (freq < -40) sig = 0;
		}
		else {
			if (freq > 40) 	sig = 0;
			if (freq < -40) sig = 1;
		}
		/* now decode bits into Baudot code */
		switch (RTTY_Sync) {
			case 0: /* Unsynchronized */
				if (!sig) {
					++space_count;  /* Initial SPACE detected */
					RTTY_Sync = 1;
					++position_count;
				}
				break;
			case 1: /* starting synchronization */
				if (!sig) ++space_count;
				if (position_count ==22) {
					if (space_count >11) { /* have enough SPACE samples */
						RTTY_Sync = 2;	/* have detected a START bit */
						datareg = 0xFF;
						++bit_count; /* START bit will be first count */
					}
					else {
						RTTY_Sync = 0; /* must not be START bit, so reset */
						bit_count = 0;
					}
					position_count = 0;
					space_count = 0;
				}
				else ++position_count;
				break;
			case 2: /* looking for data bits */
				if (!sig) ++space_count;
				if (position_count == 22) {
					datareg >>= 1; /* make room for new bit */
					datareg &= 0x7F; /* clear MSB */
					if (space_count < 11) datareg |= 0x80; /* set MSB if data = MARK */
					space_count = 0;
					++bit_count;
					position_count = 0;
				}
				else ++position_count;
				if (bit_count == 7) { /* this should be the STOP bit */
					if (datareg & 0x80) { /* yes, it is the STOP bit */
						datareg >>= 2; /* clear START bit */
						datareg &= 0x1F; /* strip STOP bits */
						if (m_SQOpen) {
							if (datareg == 0x1F) Rtty.FIGURES = FALSE;
							if (datareg == 0x1B) Rtty.FIGURES = TRUE;
							if ((Rtty.UOS) && (datareg == 0x04)) Rtty.FIGURES = FALSE;
							ch = RTTY_table [datareg][Rtty.FIGURES];
							if (ch) Buffered_LCD_Write (ch); 
						}
					}
					datareg = 0; 
					position_count = 0;
					bit_count = 0;
					space_count = 0;
					RTTY_Sync = 0;
				}
				break;
			} /* end of bit processing */

	   }  /* Index 2 */	

	  }	/* Index 1 */

	} /* FOR loop */

} /* ProcRTTYDet */


/* ////////////////////////////////////////////////////////////////////  */
/*  Frequency error calculator                                           */
/* calculates the derivative of the tan(I/Q).                            */
/* returns frequency error for mainNCO.                                  */
/* ////////////////////////////////////////////////////////////////////  */
void CalcFreqError( struct Complex IQ )
{
#define P_GN 0.001			/*AFC constants  */
#define I_GN 1.5E-6
#define P_CGN 0.0004
#define I_CGN 3.0E-6
#define WIDE_GN (1.0/.02)			/*gain to make error in Hz  */
#define WLP_K (200.0)
extern double m_FferrAve;
double freqerr;

	if(m_AFCmode == AFC_OFF)
	{
		m_FferrAve = 0.0;
		m_FperrAve = 0.0;
		m_FreqError = 0.0;
		return;
	}

	freqerr = (IQ.x - m_z2.x) * m_z1.y - (IQ.y - m_z2.y) * m_z1.x;
	m_z2.x = m_z1.x;
	m_z2.y = m_z1.y;
	m_z1.x = IQ.x;
	m_z1.y = IQ.y;
	/* error at this point is abt .02 per Hz error  */
	if( freqerr > .30 )		/*clamp range  */
		freqerr = .30;
	if( freqerr < -.30 )
		freqerr = -.30;
	m_FferrAve = (1.0-1.0/WLP_K)*m_FferrAve + ((1.0*WIDE_GN)/WLP_K)*freqerr;
	/* error is now approximately in Hz */

	if( m_AFCCaptureOn )
	{
		freqerr=m_FferrAve;
		if( (freqerr > 0.3) || (freqerr < -0.3 ) )

			m_NCOphzinc = m_NCOphzinc + (freqerr*I_CGN); 
			m_FreqError = freqerr*P_CGN; 
 
	} 

	else
	{
		if( (m_FferrAve*m_FperrAve)>0.0 )
			freqerr = m_FperrAve;
		else
			freqerr = 0.0;
		if( (freqerr > 0.3) || (freqerr < -0.3 ) )
			m_NCOphzinc = m_NCOphzinc + (freqerr*I_GN); 
			m_FreqError = freqerr*P_GN; 

	}
	
	/*clamp frequency within range  */
	if( (m_NCOphzinc+m_FreqError) > m_AFCmax )
	{
		m_NCOphzinc = m_AFCmax; 
		m_FreqError = 0.0; 
	}
	else if( (m_NCOphzinc+m_FreqError) < m_AFCmin )
	{
		m_NCOphzinc = m_AFCmin; 
		m_FreqError = 0.0; 
	}
  
}



/* ////////////////////////////////////////////////////////////////////  */
/* Automatic gain control calculator                                     */
/* ////////////////////////////////////////////////////////////////////  */
void CalcAGC( struct Complex Samp)
{
double mag;

	mag = sqrt(Samp.x*Samp.x + Samp.y*Samp.y);

	if( mag > m_AGCave )
		m_AGCave = (1.0-1.0/200.0)*m_AGCave + (1.0/200.0)*mag;
	else
		m_AGCave = (1.0-1.0/500.0)*m_AGCave + (1.0/500.0)*mag;

	if( m_AGCave >= 1.0 )	/* divide signal by ave if not almost zero */
	{
		m_BitSignal.x /= m_AGCave;
		m_BitSignal.y /= m_AGCave;
		m_FreqSignal.x /= m_AGCave;
		m_FreqSignal.y /= m_AGCave;
	}

}

/* ////////////////////////////////////////////////////////////////////  */
/* Called at Fs/16 rate to calculate the symbol sync position            */
/* Returns TRUE if at center of symbol.                                  */
/* Sums up the energy at each sample time, averages it, and picks the    */
/*   sample time with the highest energy content.                        */
/* ////////////////////////////////////////////////////////////////////  */
int SymbSync(struct Complex sample)
{
int Trigger=FALSE;
double max;
double energy;
int BitPos = m_BitPos;
int i;
	if(BitPos<16)
	{
		energy = (sample.x*sample.x) + (sample.y*sample.y);
		if( energy > 4.0)		/*wait for AGC to settle down */
			energy = 1.0;
		m_SyncAve[BitPos] = (1.0-1.0/82.0)*m_SyncAve[BitPos] + (1.0/82.0)*energy;
		if( BitPos == m_PkPos )	/* see if at middle of symbol */
		{
			Trigger = TRUE;
			m_SyncArray[m_PkPos] = (int)(900.0*m_SyncAve[m_PkPos]);
		}
		else
		{
			Trigger = FALSE;
			m_SyncArray[BitPos] = (int)(750.0*m_SyncAve[BitPos]);
		}
		if( BitPos == HALF_TBL[m_NewPkPos] )	/*don't change pk pos until */
			m_PkPos = m_NewPkPos;			/* halfway into next bit. */
		BitPos++;
	}

	m_BitPhasePos += (m_BitPhaseInc);
	if( m_BitPhasePos >= Ts )
	{									/* here every symbol time */
		m_BitPhasePos = fmod(m_BitPhasePos, Ts);	/*keep phase bounded */
		if((BitPos==15) && (m_PkPos==15))	/*if missed the 15 bin before rollover */
			Trigger = TRUE;
		BitPos = 0;
		max = -1e10;

		for(i=0; i<16; i++)		/*find maximum energy pk */
		{
			energy = m_SyncAve[i];
			if( energy > max )
			{
				m_NewPkPos = i;
				max = energy;
			}
		}
		if(m_SQOpen)
		{
			if( m_PkPos == m_LastPkPos+1 )	/*calculate clock error */
				m_ClkErrCounter++;
			else 
				if( m_PkPos == m_LastPkPos-1 )
					m_ClkErrCounter--;
			if( m_ClkErrTimer++ > 313 )	/* every 10 seconds sample clk drift */
			{
				m_ClkError = m_ClkErrCounter*200;	/*each count is 200ppm */
				m_ClkErrCounter = 0;
				m_ClkErrTimer = 0;
			}
		}
		else
		{
			m_ClkError = 0;
			m_ClkErrCounter = 0;
			m_ClkErrTimer = 0;
		}
		m_LastPkPos = m_PkPos;
	}
	m_BitPos = BitPos;
	return Trigger;
}

/* ////////////////////////////////////////////////////////////////////  */
/*  Decode the new symbol sample                                         */
/* ////////////////////////////////////////////////////////////////////  */
void DecodeSymb(struct Complex newsamp)
{
struct Complex vect;
double angle;
double energy;
extern Flag_type Flag;
unsigned char ch = 0;
extern unsigned char VaricodeDecTbl[];
int bit;
extern int m_PSKmode;
int GotChar = FALSE;

	m_I1 = m_I0;		/*form the multi delayed symbol samples  */
	m_Q1 = m_Q0;
	m_I0 = newsamp.x;
	m_Q0 = newsamp.y;
/* calculate difference angle for QPSK, BPSK, and IQPSK decoding  */
/* create vector whose angle is the difference angle by multiplying the */
/* current sample by the complex conjugate of the previous sample. */
/* swap I and Q axis to keep away from  the +/-Pi discontinuity and */
/*  add Pi to make make range from 0 to 2Pi. */
/* 180 deg phase changes center at Pi/4 */
/* 0 deg phase changes center at 3Pi/2 */
/* +90 deg phase changes center at 2Pi or 0 */
/* -90 deg phase changes center at Pi */
/*  if using lower sideband must flip sign of difference angle. */
/*                                                              */
/* first calculate normalized vectors for vector display        */
	vect.y = (m_I1*m_I0 + m_Q1*m_Q0);
	vect.x = (m_I1*m_Q0 - m_I0*m_Q1);
	energy = sqrt(vect.x*vect.x + vect.y*vect.y)/1.0E3;

	if( m_AGCave > 10.0 )
	{
		m_IQPhaseArray[m_IQPhzIndex++] = (long)(vect.x/energy);
		m_IQPhaseArray[m_IQPhzIndex++] = (long)(vect.y/energy);
	}
	else
	{
		m_IQPhaseArray[m_IQPhzIndex++] = 2;
		m_IQPhaseArray[m_IQPhzIndex++] = 2;
	}
	m_IQPhzIndex &= 0x000F;		/*mod 16 index */
	if(m_PSKmode == QPSKL_MODE)
		angle = (PI2/2) + atan2( vect.y, -vect.x); /*QPSK lower sideband; */
	else
		angle = (PI2/2) + atan2( vect.y, vect.x); /*QPSK upper sideband or BPSK */
	CalcQuality( angle);
	if(m_PSKmode == BPSK_MODE)
	{
		/*calc BPSK symbol over 2 chips */
		vect.y = m_I1 * m_I0 +  m_Q1 * m_Q0;
		bit = (int)(vect.y > 0.0);
	}
	else
		bit = ViterbiDecode( angle );
	if( (bit==0) && m_LastBitZero )	/*if character delimiter */
	{
		if(m_BitAcc != 0 )
		{
			m_BitAcc >>= 2;				/*get rid of last zero and one */
			m_BitAcc &= 0x07FF;
			ch = m_VaricodeDecTbl[m_BitAcc];
			m_BitAcc = 0;
			GotChar = TRUE;
		}
	}
	else
	{
		m_BitAcc <<= 1;
		m_BitAcc |= bit;
		if(bit==0)
			m_LastBitZero = TRUE;
		else
			m_LastBitZero = FALSE;
	}

	if(GotChar && (ch!=0) && m_SQOpen )
	{
		Buffered_LCD_Write (ch); /* send character to scrolling character LCD */
		GotChar = FALSE;
	}
}

/* ////////////////////////////////////////////////////////////////////  */
/* Calculate signal quality based on the statistics of the phase         */
/*	difference angle.  The more dispersion of the "0" and "180" degree   */
/*  phase shifts, the worse the signal quality.  This information is used  */
/*  to activate the squelch control.  If 20 consecutive "180" degree shifts */
/*  occur, the squelch is forced on, and if 20 consecutive "0" degree    */
/*  shifts occur, the squelch is forced off quickly.                     */
/* ////////////////////////////////////////////////////////////////////  */
void CalcQuality( double angle )
{
#define ELIMIT 5
#define PHZDERIVED_GN (1.0/.2)		/*gain to make error in Hz */
double temp;
double SqTimeK;
	SqTimeK = (double)m_SquelchSpeed;
	if( (m_PSKmode && ((angle >= PHZ_180_QMIN) && (angle <= PHZ_180_QMAX) ) ) ||
		( !m_PSKmode && ((angle >= PHZ_180_BMIN) && (angle <= PHZ_180_BMAX))) )
	{	/*look +/-45 or +/-180 deg. around 180 deg. */
		if(m_PSKmode == QPSKL_MODE )
			temp = PI2/4.0 - angle;
		else
			temp = angle - PI2/4.0;
		m_QFreqError = temp;
		if( m_PSKmode ) /*if QPSK */
			temp = 280.0*fabs(temp);
		else
			temp = 150.0*fabs(temp);
		if( temp < m_DevAve)
			m_DevAve=  (1.0-1.0/SqTimeK)*m_DevAve + (1.0/SqTimeK)*temp;
		else
			m_DevAve=  (1.0-1.0/(SqTimeK*2.0))*m_DevAve + (1.0/(SqTimeK*2.0))*temp;
		if(m_OnCount > 20 )		/* fast squelch counter */
			m_DevAve = 100.0-75.0;	/*set to 75% */
		else
			m_OnCount++;
		m_OffCount = 0;
		if( m_QFreqError >= 0.0 )
		{
			m_Pcnt++;
			m_Ncnt = 0;
		}
		else
		{
			m_Ncnt++;
			m_Pcnt = 0;
		}
		if( (m_Pcnt<ELIMIT) && (m_Ncnt<ELIMIT) )
			m_QFreqError = 0.0;
	}
	else
	{
		if( (m_PSKmode && ((angle >= PHZ_0_QMIN) && (angle <= PHZ_0_QMAX) ) ) ||
			(!m_PSKmode && ((angle >= PHZ_0_BMIN) && (angle <= PHZ_0_BMAX) ) ) )

		{		/*look +/-45 or +/- 180 deg. around 0 deg. */
			if(QPSKL_MODE==m_PSKmode)
				temp = 3*PI2/4.0 - angle;
			else
				temp = angle - 3*PI2/4.0;
			m_QFreqError = temp;
			if( m_PSKmode ) /*if QPSK */
				temp = 280.0*fabs(temp);
			else
				temp = 150.0*fabs(temp);
			if( temp < m_DevAve)
				m_DevAve=  (1.0-1.0/SqTimeK)*m_DevAve + (1.0/SqTimeK)*temp;
			else
				m_DevAve=  (1.0-1.0/(SqTimeK*2.0))*m_DevAve + (1.0/(SqTimeK*2.0))*temp;
			if((m_OffCount > 20) ) {	/* fast squelch counter */
				if( BPSK_MODE==m_PSKmode ) { /*if BPSK */
					m_DevAve = 100.0 - 0.0;		/*set to 0% */
				}
			}
			else m_OffCount++;
			m_OnCount = 0;
			if( m_QFreqError >= 0.0 )
			{
				m_Pcnt++;
				m_Ncnt = 0;
			}
			else
			{
				m_Ncnt++;
				m_Pcnt = 0;
			}
			if( (m_Pcnt<ELIMIT) && (m_Ncnt<ELIMIT) )
				m_QFreqError = 0.0;
		}

	}
	if(m_OnCount >2)
		m_IMDValid = TRUE;
	else
		m_IMDValid = FALSE;
/*Debugint = m_Pcnt+m_Ncnt; */

	if( m_AGCave > 10.0 )
	{
		if( m_PSKmode ) /*if QPSK */
			m_SQLevel = 100 - (int)m_DevAve;
		else
			m_SQLevel = 100 - (int)m_DevAve;
		if( m_SQLevel >= m_SQThresh )
			m_SQOpen = TRUE;
		else
			m_SQOpen = FALSE;
	}
	else
	{
		m_SQLevel = 0;
		m_SQOpen = FALSE;
	}
	if(m_PSKmode)
	{
		if( m_QFreqError > .6 )/*  clamp range to +/- 3 Hz */
			m_QFreqError = .6;
		if( m_QFreqError < -.6 )
			m_QFreqError = -.6;
	}
	else
	{
		if( m_QFreqError > 1.0 )/*  clamp range to +/- 5 Hz */
			m_QFreqError = 1.0;
		if( m_QFreqError < -1.0 )
			m_QFreqError = -1.0;
	}
	m_FperrAve = (1.0-1.0/m_NLPk)*m_FperrAve + 
					( (1.0*PHZDERIVED_GN)/m_NLPk)*m_QFreqError;

}

/*/////////////////////////////////////////////////////////////////////  */
/* Soft-decision Viterbi decoder function.                               */
/* ///////////////////////////////////////////////////////////////////// */
int ViterbiDecode( double newangle)
{
double pathdist[32];
double min;
int bitestimates[32];
long ones;
int i;
const double* pAngleTbl;
	min = 1.0e37;		/* make sure can find a minimum value */
	if( newangle >= PI2/2 )		/*deal with ambiguity at +/- 2PI */
		pAngleTbl = ANGLE_TBL2;	/* by using two different tables */
	else
		pAngleTbl = ANGLE_TBL1;
	for(i = 0; i < 32; i++)		/* calculate all possible distances */
	{							/*lsb of 'i' is newest bit estimate */
		pathdist[i] = m_SurvivorStates[i / 2].Pathdistance + 
				fabs(newangle - pAngleTbl[ ConvolutionCodeTable[i] ]);
		if(pathdist[i] < min)	/* keep track of minimum distance */
			min = pathdist[i];
		/* shift in newest bit estimates */
		bitestimates[i] = ((m_SurvivorStates[i / 2].BitEstimates) << 1) + (i & 1);
	}
	for(i = 0; i < 16; i++)	/*compare path lengths with the same end state */
					/* and keep only the smallest path in m_SurvivorStates[]. */
	{
		if(pathdist[i] < pathdist[16 + i])
		{
			m_SurvivorStates[i].Pathdistance = pathdist[i] - min;
			m_SurvivorStates[i].BitEstimates = bitestimates[i];
		}
		else
		{
			m_SurvivorStates[i].Pathdistance = pathdist[16 + i] - min;
			m_SurvivorStates[i].BitEstimates = bitestimates[16 + i];
		}
	}
	ones = 0;
	for(i = 0; i < 16; i++)		/* find if more ones than zeros at bit 20 position */
		ones += (m_SurvivorStates[i].BitEstimates&(1L << 20));
	if( ones == (8L << 20 ) )
		return ( rand() & 0x1000 );	/*if a tie then guess */
	else
		return(ones > (8L << 20) );	/*else return most likely bit value */
}

/* 	Process the contents of the FFT buffer (length = FFT_BLOCK_LENGTH */
void Proc_FFT ( void ) {
	int	i;
	extern Flag_type Flag;
	extern unsigned int FFT_Output_Buffer [];
	extern unsigned int FLOB [];
	extern const fractcomplex 	twiddleFactors[FFT_BLOCK_LENGTH/2]
	__attribute__ ((space(auto_psv), aligned (FFT_BLOCK_LENGTH*2))); 

/*	Do "in-place" complex FFT on contents of FFT_buffer */
	FFTComplexIP (LOG2_BLOCK_LENGTH, &FFT_buffer[0], (fractcomplex *)
	__builtin_psvoffset(&twiddleFactors[0]), (int)
	__builtin_psvpage(&twiddleFactors[0]));

/*  Do necessary bit reversals */
	BitReverseComplex (LOG2_BLOCK_LENGTH, &FFT_buffer[0]); 
	
/*	Compute the square of the magnitude of the complex FFT so we have*/
/*	a Real output vector */
	SquareMagnitudeCplx(FFT_BLOCK_LENGTH, &FFT_buffer[0], &FFT_buffer[0].real);

/*  Do a simple IIR filter on the output signal */
	for (i=0; i<FFT_BLOCK_LENGTH/2; i++) {
	FFT_Output_Buffer [i] =	((3*FFT_Output_Buffer [i] + 
			(unsigned int)FFT_buffer[FFT_BLOCK_LENGTH/2 - i].real)>>2);
/*	I haven't figured out why the FFT is not in the correct order */
	}
/* 	Convert the data to a db scale 	*/
	for (i=0; i<FFT_BLOCK_LENGTH/4; i++) {
/*	FLOB [i] = (int) ( (float) 6.*log10((float) FFT_Output_Buffer [i+32]) +.5); */
	FLOB [i] = (int) 6.*log10((float) FFT_Output_Buffer [i+32] + 1.);
	}
	FLOB [0] = 0;

/*	Now send the cursor (frequency) and FFT data to the graphics LCD */
/*	Write_NCO_Cursor_LCD2 (); */ 
	Display_FFT ();
	if (m_PSKmode == RTTY_MODE) {
		Display_RTTYCursor();
		Display_RTTY_Info();
	}
	/* clear the FFT_buffer */
	for (i=0; i<FFT_BLOCK_LENGTH; i++) {
	FFT_buffer[i].real = 0.;
	FFT_buffer[i].imag = 0.; 
	}
}

