//////////////////////////////////////////////////////////////////////
// Wave.h: interface for the CWave class.
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

#if !defined(AFX_WAVE_H__43BFB6C1_EBE9_11D1_B48C_444553540000__INCLUDED_)
#define AFX_WAVE_H__43BFB6C1_EBE9_11D1_B48C_444553540000__INCLUDED_

#include <mmsystem.h>
#include "ErrorCodes.h"


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


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



class CWave  
{
public:
	CWave();
	virtual ~CWave();
	// Public member functions for object's users.
	LONGLONG GetDriveFreeSpace(CString* pPathName);
	UINT InOpen( CString* pPathName, WAVEFORMATEX* pWFX, DWORD BufSize,
						PDWORD pFileSize, DWORD Offset);
	LONG InRead( double* pData , INT Length );
	BOOL RewindInputData(INT blocks);
	void InClose();
	UINT OutOpen( CString* pPathName, WAVEFORMATEX* pWFX, DWORD BufSize,
					DWORD SampleLimit, BOOL Overwrite);
	LONG OutWrite( double* pData , INT Length );
	void OutClose();
	UINT GetError(void){return m_ErrorCode;}
	BOOL IsOutOpen(){ return m_OutputOpen;}
	BOOL IsInOpen(){ return m_InputOpen;}

private:
	BOOL m_InputOpen;
	BOOL m_OutputOpen;
	INT m_InBytesPerSample;
	INT m_OutBytesPerSample;
	INT m_InBlocksRead;
	UINT m_ErrorCode;
	LONG m_InBufferSize;		// internal byte buffer
	LONG m_OutBufferSize;		// internal byte buffer
	LONG m_InBufPosition;		// current byte inbuffer position
	LONG m_OutBufPosition;		// current byte outbuffer position
	DWORD m_OutSampleLimit;
	DWORD m_OutSamplesWritten;
	HMMIO m_INhmmio;
	HMMIO m_OUThmmio;
	WAVEFORMATEX m_InFormat;
	WAVEFORMATEX m_OutFormat;
	MMCKINFO m_mmcktSubchunk;
	MMCKINFO m_mmcktRIFF;
	CRITICAL_SECTION m_CriticalSection;	// use for keeping threads
										// from stomping on each other
	char * m_pInBuffer;
	char * m_pOutBuffer;
	union uSWORD{
		struct{
			BYTE lsb;
			BYTE msb;
		}bytes;
		SHORT both;
	}m_usTemp;
};

#endif // !defined(AFX_WAVE_H__43BFB6C1_EBE9_11D1_B48C_444553540000__INCLUDED_)
