//////////////////////////////////////////////////////////////////////
// Sound.h: interface for the CSound class.
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// PSK31Core Library for transmission and reception of PSK31 signals
//    using a PC soundcard  or .wav files.
//	               Copyright 2000, Moe Wheatley, AE4JY
//
//This library is free software; you can redistribute it and/or
//modify it under the terms of the GNU Lesser General Public
//License as published by the Free Software Foundation; either
//version 2.1 of the License, or (at your option) any later version.
//
//This library is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//Lesser General Public License for more details.
//
//You should have received a copy of the GNU Lesser General Public
//License along with this library; if not, write to the Free Software
//Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// http://www.gnu.org/copyleft/lesser.html
//////////////////////////////////////////////////////////////////////
//

#if !defined(AFX_SOUND_H__0A83B9C2_549A_11D2_A141_00A0C996E7F5__INCLUDED_)
#define AFX_SOUND_H__0A83B9C2_549A_11D2_A141_00A0C996E7F5__INCLUDED_

#include <mmsystem.h>

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#define NUM_INPUT_SNDBUFFERS 101	// number of sound input buffers to allocate
#define NUM_OUTPUT_SNDBUFFERS 3		// number of sound output buffers to allocate

#define FIRDEC6_LENGTH 84
const double FIRDEC6[FIRDEC6_LENGTH*2] = {
   -0.001026086585,
   -0.002131424398,
   -0.003473651912,
   -0.005415991401,
   -0.007094896149,
   -0.008685456592,
   -0.009325598368,
   -0.009096149588,
   -0.007453841473,
   -0.004787743788,
   -0.001155271744,
    0.002594565142,
    0.005930466661,
    0.007902971637,
    0.008117998323,
    0.006209532708,
    0.002565394219,
   -0.002211033166,
   -0.006928668876,
   -0.010456837484,
   -0.011638240040,
   -0.009915015895,
   -0.005261804435,
    0.001469200215,
    0.008865519609,
    0.015027698257,
    0.018157576105,
    0.016869453853,
    0.010745489111,
    0.000474997075,
   -0.012015135186,
   -0.023901641616,
   -0.031878023716,
   -0.032876349281,
   -0.024689692215,
   -0.006586586958,
    0.020417464549,
    0.053543134171,
    0.088652482149,
    0.120891532319,
    0.145553464727,
    0.158910442030,
    0.158910442030,
    0.145553464727,
    0.120891532319,
    0.088652482149,
    0.053543134171,
    0.020417464549,
   -0.006586586958,
   -0.024689692215,
   -0.032876349281,
   -0.031878023716,
   -0.023901641616,
   -0.012015135186,
    0.000474997075,
    0.010745489111,
    0.016869453853,
    0.018157576105,
    0.015027698257,
    0.008865519609,
    0.001469200215,
   -0.005261804435,
   -0.009915015895,
   -0.011638240040,
   -0.010456837484,
   -0.006928668876,
   -0.002211033166,
    0.002565394219,
    0.006209532708,
    0.008117998323,
    0.007902971637,
    0.005930466661,
    0.002594565142,
   -0.001155271744,
   -0.004787743788,
   -0.007453841473,
   -0.009096149588,
   -0.009325598368,
   -0.008685456592,
   -0.007094896149,
   -0.005415991401,
   -0.003473651912,
   -0.002131424398,
   -0.001026086585,
//////
   -0.001026086585,
   -0.002131424398,
   -0.003473651912,
   -0.005415991401,
   -0.007094896149,
   -0.008685456592,
   -0.009325598368,
   -0.009096149588,
   -0.007453841473,
   -0.004787743788,
   -0.001155271744,
    0.002594565142,
    0.005930466661,
    0.007902971637,
    0.008117998323,
    0.006209532708,
    0.002565394219,
   -0.002211033166,
   -0.006928668876,
   -0.010456837484,
   -0.011638240040,
   -0.009915015895,
   -0.005261804435,
    0.001469200215,
    0.008865519609,
    0.015027698257,
    0.018157576105,
    0.016869453853,
    0.010745489111,
    0.000474997075,
   -0.012015135186,
   -0.023901641616,
   -0.031878023716,
   -0.032876349281,
   -0.024689692215,
   -0.006586586958,
    0.020417464549,
    0.053543134171,
    0.088652482149,
    0.120891532319,
    0.145553464727,
    0.158910442030,
    0.158910442030,
    0.145553464727,
    0.120891532319,
    0.088652482149,
    0.053543134171,
    0.020417464549,
   -0.006586586958,
   -0.024689692215,
   -0.032876349281,
   -0.031878023716,
   -0.023901641616,
   -0.012015135186,
    0.000474997075,
    0.010745489111,
    0.016869453853,
    0.018157576105,
    0.015027698257,
    0.008865519609,
    0.001469200215,
   -0.005261804435,
   -0.009915015895,
   -0.011638240040,
   -0.010456837484,
   -0.006928668876,
   -0.002211033166,
    0.002565394219,
    0.006209532708,
    0.008117998323,
    0.007902971637,
    0.005930466661,
    0.002594565142,
   -0.001155271744,
   -0.004787743788,
   -0.007453841473,
   -0.009096149588,
   -0.009325598368,
   -0.008685456592,
   -0.007094896149,
   -0.005415991401,
   -0.003473651912,
   -0.002131424398,
   -0.001026086585
};

#define INTP6_FIR_SIZE 84
#define INTP6_VALUE 6
#define INTP6_QUE_SIZE (INTP6_FIR_SIZE/INTP6_VALUE)
const double FIRINTERP6[INTP6_FIR_SIZE*2] = {
   -0.005651386064,
   -0.011739264807,
   -0.019131863031,
   -0.029829703227,
   -0.039076621599,
   -0.047836965268,
   -0.051362679728,
   -0.050098942673,
   -0.041053587897,
   -0.026369498354,
   -0.006362900291,
    0.014290109134,
    0.032663283119,
    0.043527266032,
    0.044711570395,
    0.034200297629,
    0.014129444189,
   -0.012177726714,
   -0.038161090191,
   -0.057593215307,
   -0.064100036502,
   -0.054609019799,
   -0.028980486327,
    0.008091926882,
    0.048828699923,
    0.082768185178,
    0.100006640788,
    0.092912038591,
    0.059183024395,
    0.002616145544,
   -0.066175865192,
   -0.131643280662,
   -0.175574870143,
   -0.181073356597,
   -0.135983633839,
   -0.036277002619,
    0.112453448142,
    0.294899988562,
    0.488272051621,
    0.665835350323,
    0.801666091230,
    0.875232431994,
    0.875232431994,
    0.801666091230,
    0.665835350323,
    0.488272051621,
    0.294899988562,
    0.112453448142,
   -0.036277002619,
   -0.135983633839,
   -0.181073356597,
   -0.175574870143,
   -0.131643280662,
   -0.066175865192,
    0.002616145544,
    0.059183024395,
    0.092912038591,
    0.100006640788,
    0.082768185178,
    0.048828699923,
    0.008091926882,
   -0.028980486327,
   -0.054609019799,
   -0.064100036502,
   -0.057593215307,
   -0.038161090191,
   -0.012177726714,
    0.014129444189,
    0.034200297629,
    0.044711570395,
    0.043527266032,
    0.032663283119,
    0.014290109134,
   -0.006362900291,
   -0.026369498354,
   -0.041053587897,
   -0.050098942673,
   -0.051362679728,
   -0.047836965268,
   -0.039076621599,
   -0.029829703227,
   -0.019131863031,
   -0.011739264807,
   -0.005651386064,
//
   -0.005651386064,
   -0.011739264807,
   -0.019131863031,
   -0.029829703227,
   -0.039076621599,
   -0.047836965268,
   -0.051362679728,
   -0.050098942673,
   -0.041053587897,
   -0.026369498354,
   -0.006362900291,
    0.014290109134,
    0.032663283119,
    0.043527266032,
    0.044711570395,
    0.034200297629,
    0.014129444189,
   -0.012177726714,
   -0.038161090191,
   -0.057593215307,
   -0.064100036502,
   -0.054609019799,
   -0.028980486327,
    0.008091926882,
    0.048828699923,
    0.082768185178,
    0.100006640788,
    0.092912038591,
    0.059183024395,
    0.002616145544,
   -0.066175865192,
   -0.131643280662,
   -0.175574870143,
   -0.181073356597,
   -0.135983633839,
   -0.036277002619,
    0.112453448142,
    0.294899988562,
    0.488272051621,
    0.665835350323,
    0.801666091230,
    0.875232431994,
    0.875232431994,
    0.801666091230,
    0.665835350323,
    0.488272051621,
    0.294899988562,
    0.112453448142,
   -0.036277002619,
   -0.135983633839,
   -0.181073356597,
   -0.175574870143,
   -0.131643280662,
   -0.066175865192,
    0.002616145544,
    0.059183024395,
    0.092912038591,
    0.100006640788,
    0.082768185178,
    0.048828699923,
    0.008091926882,
   -0.028980486327,
   -0.054609019799,
   -0.064100036502,
   -0.057593215307,
   -0.038161090191,
   -0.012177726714,
    0.014129444189,
    0.034200297629,
    0.044711570395,
    0.043527266032,
    0.032663283119,
    0.014290109134,
   -0.006362900291,
   -0.026369498354,
   -0.041053587897,
   -0.050098942673,
   -0.051362679728,
   -0.047836965268,
   -0.039076621599,
   -0.029829703227,
   -0.019131863031,
   -0.011739264807,
   -0.005651386064
};

//+++++++++++++   WAVEFORMATEX  member variables     +++++++++++++++++++
//typedef struct { 
//    WORD  wFormatTag; 
//    WORD  nChannels; 
//    DWORD nSamplesPerSec; 
//    DWORD nAvgBytesPerSec; 
//    WORD  nBlockAlign; 
//    WORD  wBitsPerSample; 
//    WORD  cbSize; 
//} WAVEFORMATEX; 

class CSound  
{
public:
	CSound();
	virtual ~CSound();
	// Public member functions for object's users.
	UINT InOpen( WAVEFORMATEX* pWFX, DWORD BufSize, DWORD SampleLimit, INT card);
	LONG InRead( double* pData, INT Length );
	void InClose();
	UINT OutOpen( WAVEFORMATEX* pWFX, DWORD BufSize, DWORD SampleLimit, INT card);
	LONG OutWrite( double* pData, INT Length);
	void OutClose();
	UINT GetError(void){return m_ErrorCode;}
	void RewindInputData( INT blocks);

	// Public variables for use by callback functions only.
	HANDLE m_InEventHandle;
	HANDLE m_OutEventHandle;
	BOOL m_InWaiting;
	BOOL m_InOverflow;
	BOOL m_OutWaiting;
	BOOL m_OutUnderflow;
	BOOL m_InOversample;
	BOOL m_OutOversample;
	INT	m_InHeadPtr;
	INT m_InTailPtr;
	INT	m_OutHeadPtr;
	INT m_OutTailPtr;
	BOOL m_InputOpen;
	BOOL m_OutputOpen;
	CRITICAL_SECTION m_CriticalSection;	// use for keeping threads
										// from stomping on each other

private:
	UINT WaitForFlush();
	BOOL m_RewindActive;
	BOOL m_RewindStart;
	BOOL m_fOutputHoldoff;		//keeps output off until half the buffers are full
	INT m_InBytesTotal;
	INT m_InBytesPerSample;
	INT m_OutBytesPerSample;
	INT m_RewindBlockCount;
	INT m_InState;
	INT m_OutState;
	UINT m_InSampCnt;
	UINT m_OutSampCnt;
	UINT m_ErrorCode;
	LONG m_InBufferSize;		//bytes in each buffer
	LONG m_OutBufferSize;		//bytes in each buffer
	LONG m_InBufPosition;
	LONG m_OutBufPosition;
	DWORD m_InSampleLimit;
	DWORD m_InSamplesRead;
	DWORD m_OutSampleLimit;
	DWORD m_OutSamplesWritten;
	double m_pInQue[FIRDEC6_LENGTH];
	double m_pOutQue[INTP6_QUE_SIZE];
	HWAVEIN m_hwvin;
	HWAVEOUT m_hwvout;
	WAVEFORMATEX m_OutFormat;
	WAVEFORMATEX m_InFormat;
	volatile LPWAVEHDR m_pInputBuffer[NUM_INPUT_SNDBUFFERS];	// soundcard input buffers
	volatile LPWAVEHDR m_pOutputBuffer[NUM_OUTPUT_SNDBUFFERS];	// soundcard output buffer ptrs
	union uSWORD{
		struct xbytes{
			BYTE lsb;
			BYTE msb;
		}bytes;
		SHORT both;
	}m_usTemp;
};
	// public callback function for use in soundcard input/output functions
	void CALLBACK WaveInCallback( HWAVEIN, UINT, CSound*, DWORD, DWORD );
	void CALLBACK WaveOutCallback( HWAVEOUT, UINT, CSound*, DWORD, DWORD );
#endif // !defined(AFX_SOUND_H__0A83B9C2_549A_11D2_A141_00A0C996E7F5__INCLUDED_)
