/* Modified by Milt Cram, W8NUE--June 2006  */

/* //////////////////////////////////////////////////////////////////// */
/* PskDet.h: interface for the CPSKDet class.                           */
/* //////////////////////////////////////////////////////////////////// */
/* //////////////////////////////////////////////////////////////////// */
/* PSK31Core Library for transmission and reception of PSK31 signals    */
/*    using a PC soundcard  or .wav files.                              */
/*	               Copyright 2000, Moe Wheatley, AE4JY                  */
/*                                                                      */
/* This library is free software; you can redistribute it and/or        */
/* modify it under the terms of the GNU Lesser General Public           */
/* License as published by the Free Software Foundation; either         */
/* version 2.1 of the License, or (at your option) any later version.   */
/*                                                                      */
/* This library is distributed in the hope that it will be useful,      */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU    */
/* Lesser General Public License for more details.                      */
/*                                                                      */
/* You should have received a copy of the GNU Lesser General Public     */
/* License along with this library; if not, write to the Free Software  */
/* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */
/* http://www.gnu.org/copyleft/lesser.html                              */
/* ///////////////////////////////////////////////////////////////////  */


#include <math.h>
#include <PskDetTbl.h>
#include <dsp.h>

/* Complex number   */
struct Complex {
	double	x;	/* Real part */
	double	y;	/* Imaginary part */
};

#define BLOCKSIZE 128
#define BLOCKSIZE2 (BLOCKSIZE*2)
#define FFT_BLOCK_LENGTH 512
#define LOG2_BLOCK_LENGTH 9
#define BPSK_MODE 0
#define QPSKU_MODE 1
#define QPSKL_MODE 2
#define RTTY_MODE 6
#define TRUE 1
#define FALSE 0

#define PSK63_MODE (1<<3)

#define SQMODEFAST 0
#define SQMODESLOW 1

struct SurvivorStates  {
	double Pathdistance;	/* sum of all metrics for a given survivor path */
	long BitEstimates;		/* the bit pattern estimate associated with given */
                            /* survivor path */
};

/* methods */

	struct Complex Samp;
	struct Complex newsamp;
	struct Complex sample;
	struct Complex newsymb;
	struct Complex IQ;
	void CalcBitFilter( struct Complex Samp);
	void CalcAGC( struct Complex Samp );
	void DecodeSymb( struct Complex newsamp );
	void CalcQuality(double angle );
	int ViterbiDecode( double newangle);
	int SymbSync( struct Complex sample);
	double CalcDistance( struct Complex newsymb, char symb);
	void CalcFreqError( struct Complex IQ );
/* variables */
	extern unsigned int FFT_Output_Buffer [];
	extern unsigned int FLOB [];
	extern fractcomplex	FFT_buffer[]
	__attribute__ ((space ( ymemory), aligned (FFT_BLOCK_LENGTH * 2 * 2),section ("FFT_buffer"))); 
	int m_FastAFCMode;
	int m_AFCCaptureOn;
	int m_IMDValid;
	int m_SQOpen;
	int m_LastBitZero;
	int m_PSK63Mode;
	unsigned char m_VaricodeDecTbl[2048]; 
	int m_BitAcc;
	long m_IQPhaseArray[20];
	long m_SyncArray[20];
	int m_AFCTimer;
	int m_AFCmode;
	extern int m_PSKmode;
	int m_SampleClkAdj;
	int m_AFCcounter;
	int m_IQPhzIndex;
	int m_SquelchSpeed;
	int m_SQLevel;
	int m_SQThresh;
	int m_ClkErrTimer;
	int m_ClkErrCounter;
	int m_ClkError;
	int m_LastPkPos;
	int m_OnCount;
	int m_OffCount;
	int m_SampCnt;
	int m_BlockSize;
	double m_FreqError;
	double m_QPSKprob[4];
	double m_DevAve;
	double m_I0;		/* 4 stage I/Q delay line variables */
	double m_I1;
	double m_Q0;
	double m_Q1;
	double m_BitPhaseInc;
	double m_BitPhasePos;
	double m_SyncAve[21];
	double m_NCOphzinc;
	double m_SampleFreq;
	double m_AFClimit;
	double m_AFCmax;
	double m_AFCmin;
	double m_NLPk;

	struct Complex m_FreqSignal;
	struct Complex m_BitSignal;

	struct SurvivorStates m_SurvivorStates[16]; /* Survivor path trellis */

/* Local variables for various functions that need to be saved between calls */
	int	m_PkPos;
	int	m_NewPkPos;
	int m_BitPos;
	int m_Pcnt;
	int m_Ncnt;
	double m_AGCave;
	double m_FperrAve;
	double m_FferrAve;
	double m_QFreqError;
	double m_VcoPhz;
	struct Complex m_z1;
	struct Complex m_z2;

	void Init(int Fs );
	void ProcPSKDet();
	void ProcRTTYDet();
	void Proc_FFT(void);
	void GetVectorData(long* VectData);
	void GetSyncData(long* SyncData);
	void ResetDetector();
	void SetRXFrequency(double freq);
	void SetRXPSKMode(int mode) 
	{
		if(mode&PSK63_MODE)
			m_PSK63Mode = TRUE;
		else
			m_PSK63Mode = FALSE;
		m_PSKmode = mode & ~PSK63_MODE;
	}
	void SetAFCLimit(int limit);
	void SetSampleClkAdj(int ppm);
/*	int GetRXFrequency(){return  NCO_Frequency;}; */
	int GetSignalLevel()
	{ 
		if(m_SQLevel>0)
			return m_SQLevel;
		else
			return 0;
	}
	void SetSquelchThresh(int thresh,int speed)
	{
		m_SQThresh = thresh;
		if( speed == SQMODEFAST )
			m_SquelchSpeed = 20;
		else
			if( speed == SQMODESLOW )
				m_SquelchSpeed = 75;
			else
				m_SquelchSpeed = speed;
	}

