//////////////////////////////////////////////////////////////////////
// Sound.cpp: implementation of the CSound class.
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

// This class implements an object that reads/writes to a soundcard.
//
// The following member functions are used to perform all the tasks:
//
//  UINT m_InOpen( WAVEFORMATEX* pWFX, DWORD BufSize, DWORD SampleLimit, INT card);
//	LONG m_InRead( double* pData, INT Length );
//	void InClose();
//	void RewindInputData(INT blocks);
//
//	UINT m_OutOpen( WAVEFORMATEX* pWFX,	DWORD BufSize, DWORD SampleLimit, INT card);
//	LONG m_OutWrite( double* pData, INT Length  );
//	void OutClose();
//
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

#include "stdafx.h"
#include "Sound.h"
#include "ErrorCodes.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//local defines
#define TIMELIMIT 2000			// time limit to wait on a new soundcard buffer
								// to become ready in milliSeconds.

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSound::CSound()
{	
	m_InputOpen = FALSE;
	m_OutputOpen = FALSE;
	m_hwvin = NULL;
	m_hwvout = NULL;
	m_InEventHandle = NULL;
	m_OutEventHandle = NULL;
	m_ErrorCode = 0;
	m_RewindActive = FALSE;
	m_RewindStart = FALSE;
	for( INT i=0; i<NUM_INPUT_SNDBUFFERS; i++ )
		m_pInputBuffer[i] = NULL;
	for( INT i=0; i<NUM_OUTPUT_SNDBUFFERS; i++ )
		m_pOutputBuffer[i] = NULL;
	InitializeCriticalSection(&m_CriticalSection);
}

CSound::~CSound()
{
	DeleteCriticalSection(&m_CriticalSection);
}

//////////////////////////////////////////////////////////////////////
//  This function opens a Soundcard for input.  Sample size can be
//  1,2 or 4 bytes long depending on bits per sample and number of channels.
//( ie. a 1000 sample buffer for 16 bit stereo will be a 4000 byte buffer)
//   Sampling begins immediately so the thread must start calling m_InRead()
//       to prevent buffer overflow.
//  parameters:
//		pWFX		= WAVEFORMATEX structure with desired soundcard settings
//		BufSize		= DWORD specifies the soundcard buffer size number 
//						in number of samples to buffer.
//					  If this value is Zero, the soundcard is opened and
//						then closed. This is useful	for checking the
//						soundcard.
//		SampleLimit = limit on number of samples to be read. 0 signifies
//						continuous input stream.
//		card		= Which soundcard to use(0 to n-1) (-1 lets Windows decide)
//    returns:
//        0			if opened OK
//    ErrorCode		if not
//////////////////////////////////////////////////////////////////////
UINT CSound::InOpen( WAVEFORMATEX* pWFX, DWORD BufSize, DWORD SampleLimit, INT card)
{
INT i;
	m_InWaiting = FALSE;
	m_InOverflow = FALSE;
	m_InHeadPtr = 0;
	m_InTailPtr = 0;
	m_InSamplesRead = 0;
	m_InBufPosition = 0;
	m_RewindActive = FALSE;
	m_RewindStart = FALSE;
	m_InSampleLimit = SampleLimit;
	m_InFormat = *pWFX;
	m_InBytesPerSample = (m_InFormat.wBitsPerSample/8)*m_InFormat.nChannels;
//	event for callback function to notify new buffer available
	m_InEventHandle = CreateEvent(NULL, FALSE,FALSE,NULL);

//Try to Run input at x6 the requested rate
	m_InFormat.nAvgBytesPerSec *= 6;
	m_InFormat.nSamplesPerSec *=6;
	BufSize *= 6;
	m_InBufferSize = BufSize * m_InBytesPerSample;
	m_InOversample = TRUE;
//

// open sound card input and get handle(m_hwvin) to device
	if( (m_ErrorCode = waveInOpen( &m_hwvin, card , &m_InFormat,
			(DWORD)&WaveInCallback, (DWORD)this, CALLBACK_FUNCTION ) )
						!= MMSYSERR_NOERROR )
	{
		InClose();
		m_InEventHandle = CreateEvent(NULL, FALSE,FALSE,NULL);
		//Go back and try to Run input at the requested rate
		m_InFormat.nAvgBytesPerSec /= 6;
		m_InFormat.nSamplesPerSec /= 6;
		BufSize /= 6;
		m_InBufferSize = BufSize * m_InBytesPerSample;
		m_InOversample = FALSE;
		//
		if( (m_ErrorCode = waveInOpen( &m_hwvin, card , &m_InFormat,
				(DWORD)&WaveInCallback, (DWORD)this, CALLBACK_FUNCTION ) )
							!= MMSYSERR_NOERROR )
		{
			InClose();	// m_ErrorCode = MMSYSERR_xxx
			return m_ErrorCode;
		}
	}

	if( BufSize == 0 )  // see if just a soundcard check
	{
		InClose();	// if so close the soundcard input
		return 0;
	}

	for(i=0; i<NUM_INPUT_SNDBUFFERS; i++ )
	{
// allocate input buffer headers
		m_pInputBuffer[i] = NULL;
		if( ( m_pInputBuffer[i] = new WAVEHDR[ sizeof(WAVEHDR)] ) == NULL )
		{
			m_ErrorCode = MEMORY_ERROR;
			InClose();
			return m_ErrorCode;
		}
// allocate input data buffers
		m_pInputBuffer[i]->dwBufferLength = m_InBufferSize;
		m_pInputBuffer[i]->dwFlags = 0;
		m_pInputBuffer[i]->dwUser = NULL;
		m_pInputBuffer[i]->dwBytesRecorded = NULL;
		if( (m_pInputBuffer[i]->lpData = new char[m_InBufferSize] ) == NULL )
		{
			m_ErrorCode = MEMORY_ERROR;
			InClose();
			return m_ErrorCode;
		}
		if( (m_ErrorCode = waveInPrepareHeader(m_hwvin, m_pInputBuffer[i],
					sizeof *m_pInputBuffer[i])) != MMSYSERR_NOERROR )
		{
			InClose( );	// m_ErrorCode = MMSYSERR_xxx
			return m_ErrorCode;
		}

		if( (m_ErrorCode = waveInAddBuffer(m_hwvin, m_pInputBuffer[i], 
			sizeof *m_pInputBuffer[i]) )!= MMSYSERR_NOERROR )
		{
			InClose();	// m_ErrorCode = MMSYSERR_xxx
			return m_ErrorCode;
		}
	}
//
// start input capturing to buffer
	if( (m_ErrorCode = waveInStart( m_hwvin) )!= MMSYSERR_NOERROR )
	{
		InClose();	// m_ErrorCode = MMSYSERR_xxx
		return m_ErrorCode;
	}
	m_InSampCnt = 0;
	for(i=0; i<FIRDEC6_LENGTH; i++)
		m_pInQue[i] = 0.0;
	m_InState = FIRDEC6_LENGTH-1;

	m_InputOpen = TRUE;
	return 0;
}

///////////////////////////////////////////////////////////////////////
// Reads 'Length' samples of double data from the opened soundcard input
//	returns:
//   Length	if 'Length' samples were succesfully read
//   0 =	if reaches the specified sample limit
//	-1 =	if there is an error ( use GetError() to retrieve error )
///////////////////////////////////////////////////////////////////////
LONG CSound::InRead( double* pData, INT Length)
{
INT i;
const double* Kptr;
double acc;
double* Firptr;
	if( !m_InputOpen )		// return -1 if no inputs are active
	{
		m_ErrorCode = SOUNDIN_ERR_NOTOPEN;
		return -1;
	}
	if( m_InOverflow )
	{
		m_ErrorCode = SOUNDIN_ERR_OVERFLOW;
		InClose();
		return -1;			// return -1 if overflow
	}

	EnterCriticalSection(&m_CriticalSection);
	if( m_InTailPtr == m_InHeadPtr )		//if no buffer is filled
	{
		// wait for mmsystem to fill up a new buffer
		m_InWaiting = TRUE;
		m_RewindActive = FALSE;
		LeaveCriticalSection(&m_CriticalSection);
		if( WaitForSingleObject( m_InEventHandle,TIMELIMIT )
									!= WAIT_OBJECT_0 )
		{
			m_ErrorCode = SOUNDIN_ERR_TIMEOUT; // if took too long error out
			InClose();
			return -1;
		}
	}
	else
		LeaveCriticalSection(&m_CriticalSection);
//here if there is data to retrieved
	if(m_InOversample)	//if running at 6x the requested rate
	{
		if( m_InFormat.wBitsPerSample == 16 )
		{
			i = 0;
			while( i<(Length*m_InFormat.nChannels) )
			{
				m_usTemp.bytes.lsb = *(m_pInputBuffer[m_InTailPtr]->lpData
										+ (m_InBufPosition++) );
				m_usTemp.bytes.msb = *(m_pInputBuffer[m_InTailPtr]->lpData
										+ (m_InBufPosition++) );
				//Decimate by 6 to get back to 8000Hz
				m_pInQue[m_InState] = (double)m_usTemp.both;	//place in FIR circular Queue
				if( ( (++m_InSampCnt)%6 ) == 0 )	//calc first decimation filter every 2 samples
				{
					acc = 0.0;
					Firptr = m_pInQue;
					Kptr = FIRDEC6 + FIRDEC6_LENGTH - m_InState;
					for(int j=0; j<	FIRDEC6_LENGTH; j++)	//do the MAC's
					{
						acc += ( (*Firptr++)*(*Kptr++) );
					}
					pData[i++] = acc;
				}
				if( --m_InState < 0)	//deal with FIR pointer wraparound
					m_InState = FIRDEC6_LENGTH-1;
			}
		}
		else
		{
			m_usTemp.bytes.lsb = 0;
			i = 0;
			while( i<(Length*m_InFormat.nChannels) )
			{
				m_usTemp.bytes.msb = *(m_pInputBuffer[m_InTailPtr]->lpData
										+ (m_InBufPosition++) ) - 128;
	//Decimate by 6 to get back to 8000Hz
				m_pInQue[m_InState] = (double)m_usTemp.both;	//place in FIR circular Queue
				if( ( (++m_InSampCnt)%6 ) == 0 )	//calc first decimation filter every 2 samples
				{
					acc = 0.0;
					Firptr = m_pInQue;
					Kptr = FIRDEC6 + FIRDEC6_LENGTH - m_InState;
					for(int j=0; j<	FIRDEC6_LENGTH; j++)	//do the MAC's
					{
						acc += ( (*Firptr++)*(*Kptr++) );
					}
					pData[i++] = acc;
				}
				if( --m_InState < 0)	//deal with FIR pointer wraparound
					m_InState = FIRDEC6_LENGTH-1;
			}
		}
	}
	else
	{
		if( m_InFormat.wBitsPerSample == 16 )
		{
			for( i=0; i < (Length*m_InFormat.nChannels); i++ )
			{
				m_usTemp.bytes.lsb = *(m_pInputBuffer[m_InTailPtr]->lpData
										+ (m_InBufPosition++) );
				m_usTemp.bytes.msb = *(m_pInputBuffer[m_InTailPtr]->lpData
										+ (m_InBufPosition++) );
				*(pData + i) = (double)m_usTemp.both;
			}
		}
		else
		{
			m_usTemp.bytes.lsb = 0;
			for( i=0; i < (Length*m_InFormat.nChannels); i++ )
			{
				m_usTemp.bytes.msb = *(m_pInputBuffer[m_InTailPtr]->lpData
										+ (m_InBufPosition++) ) - 128;
				*(pData + i) = (double)m_usTemp.both;
			}
		}
	}
	if( m_InBufPosition >= m_InBufferSize )		//finished with this buffer
	{											//so add it back in to the Queue
		if(	m_pInputBuffer[m_InTailPtr]->dwBytesRecorded != NULL)
		{
			m_pInputBuffer[m_InTailPtr]->dwBytesRecorded = NULL;
			waveInAddBuffer(m_hwvin, m_pInputBuffer[m_InTailPtr], sizeof *m_pInputBuffer[m_InTailPtr]);
		}
		m_InBufPosition = 0;
		if( ++m_InTailPtr >= NUM_INPUT_SNDBUFFERS)	// handle wrap around
			m_InTailPtr = 0;
		if(m_RewindStart)		//see if was a rewind request
		{
			EnterCriticalSection(&m_CriticalSection);
			do				//backup pointer
			{
				if( m_InTailPtr-- == 0 )
					m_InTailPtr = NUM_INPUT_SNDBUFFERS-1;
			}while( m_RewindBlockCount-- && (m_InTailPtr != m_InHeadPtr) );
			m_RewindStart = FALSE;
			m_RewindActive = TRUE;
			LeaveCriticalSection(&m_CriticalSection);
		}
	}
	m_InSamplesRead += Length;
	if( (m_InSampleLimit != 0) && (m_InSamplesRead >= m_InSampleLimit) )
	{
		InClose();
		return 0;
	}
	if(	m_RewindActive )	//delay a while if rewinding to ease the CPU load
		Sleep(5);
	return Length;
}

//////////////////////////////////////////////////////////////////////
//  This function "backs up" the input buffer pointer by "blocks" so
//  that that the old data can be replayed.
//  Syncronize rewind request to calling thread by just setting flags
// and blocks to backup.
//////////////////////////////////////////////////////////////////////
void CSound::RewindInputData(INT blocks)
{
	EnterCriticalSection(&m_CriticalSection);
	if( !m_RewindActive && !m_RewindStart)
	{
		m_RewindBlockCount = blocks;
		m_RewindStart = TRUE;
	}
	LeaveCriticalSection(&m_CriticalSection);
}


//////////////////////////////////////////////////////////////////////
//  Closes the Soundcard Input if open and free up resources.
//
//////////////////////////////////////////////////////////////////////
void CSound::InClose()
{
INT i;
	if(m_InWaiting)	//if user thread is waiting for buffer
	{
		m_InWaiting = FALSE;
		SetEvent( m_InEventHandle);	//signal it
	}
	m_InputOpen = FALSE;
	if (NULL != m_hwvin)
	{
		waveInReset(m_hwvin);
		Sleep(100);
		for( i=0; i<NUM_INPUT_SNDBUFFERS; i++ )
		{
			if( m_pInputBuffer[i] )
			{
				if( m_pInputBuffer[i]->lpData != NULL )
				{
					if( m_pInputBuffer[i]->dwFlags&WHDR_PREPARED )
					{
						waveInUnprepareHeader(m_hwvin, m_pInputBuffer[i],
							sizeof *m_pInputBuffer[i]);
					}
					if( m_pInputBuffer[i]->lpData )
					{
						delete m_pInputBuffer[i]->lpData;
						m_pInputBuffer[i]->lpData = NULL;
					}
				}
				if( m_pInputBuffer[i] )
				{
					delete m_pInputBuffer[i];
					m_pInputBuffer[i] = NULL;
				}
			}
		}
		Sleep(50);
		waveInClose(m_hwvin);
		m_hwvin = NULL;
	}
	if(m_InEventHandle)
	{
		CloseHandle(m_InEventHandle);
		m_InEventHandle = NULL;
	}
}

//////////////////////////////////////////////////////////////////////
//  This function opens a Soundcard for writing.
//Sample size can be 1,2 or 4 bytes long depending on bits per sample
// and number of channels.( ie. a 1000 sample buffer for 16 bit stereo
// will be a 4000 byte buffer)
//  Output does not start until at least half the buffers are filled
//  or m_OutWrite() is called with a length of '0' to flush all buffers.
//    parameters:
//		pWFX	= WAVEFORMATEX structure with desired soundcard settings
//		BufSize		= DWORD specifies the soundcard buffer size number 
//						in number of samples to buffer.
//					  If this value is Zero, the soundcard is opened and
//						then closed. This is useful	for checking the
//						soundcard.
//		SampleLimit = limit on number of samples to be written. 0 signifies
//						continuous output stream.
//		card		= Which soundcard to use(0 to n-1) (-1 lets Windows decide)
//    returns:
//        0			if opened OK
//      ErrorCode	if not
//////////////////////////////////////////////////////////////////////
UINT CSound::OutOpen( WAVEFORMATEX* pWFX, DWORD BufSize, DWORD SampleLimit, INT card)
{
	m_OutWaiting = FALSE;
	m_OutUnderflow = FALSE;
	m_OutHeadPtr = 0;
	m_OutTailPtr = 0;
	m_OutSampleLimit = SampleLimit;
	m_OutSamplesWritten = 0;
	m_OutBufPosition = 0;
// Event for callback function to signal next buffer is free
	m_OutEventHandle = CreateEvent(NULL, FALSE,FALSE,NULL);
	m_OutFormat = *pWFX;
	m_OutBytesPerSample = (m_OutFormat.wBitsPerSample/8)*m_OutFormat.nChannels;

//Try to run input at x6 the requested rate
	m_OutFormat.nAvgBytesPerSec *= 6;
	m_OutFormat.nSamplesPerSec *=6;
	BufSize *= 6;
	m_OutBufferSize = BufSize * m_OutBytesPerSample;
	m_OutOversample = TRUE;
//
	if( (m_ErrorCode = waveOutOpen( &m_hwvout, card , &m_OutFormat, 
				(DWORD)&WaveOutCallback, (DWORD)this, CALLBACK_FUNCTION ) )
						!= MMSYSERR_NOERROR )
	{
		OutClose();
		m_OutEventHandle = CreateEvent(NULL, FALSE,FALSE,NULL);
		//Go back to the requested rate
		m_OutFormat.nAvgBytesPerSec /= 6;
		m_OutFormat.nSamplesPerSec /=6;
		BufSize /= 6;
		m_OutBufferSize = BufSize * m_OutBytesPerSample;
		m_OutOversample = FALSE;
		if( (m_ErrorCode = waveOutOpen( &m_hwvout, card , &m_OutFormat, 
					(DWORD)&WaveOutCallback, (DWORD)this, CALLBACK_FUNCTION ) )
							!= MMSYSERR_NOERROR )
		{
			OutClose();	// m_ErrorCode = MMSYSERR_xxx
			return m_ErrorCode;	
		}
	}

	if( BufSize == 0 )  // see if just a soundcard check
	{
		InClose();	// if so close the soundcard input
		return 0;
	}

// start out paused so don't let output begin until some buffers are filled.
	if( (m_ErrorCode = waveOutPause( m_hwvout ))!= MMSYSERR_NOERROR )
	{
		OutClose();	// m_ErrorCode = MMSYSERR_xxx
		return m_ErrorCode;
	}
// allocate and prepare all the output buffers
	for(INT i=0; i<NUM_OUTPUT_SNDBUFFERS; i++ )
	{
		// allocate output buffer headers
		m_pOutputBuffer[i] = NULL;
		if( (m_pOutputBuffer[i] = new WAVEHDR[ sizeof(WAVEHDR)] ) == NULL )
		{
			OutClose();
			m_ErrorCode = MEMORY_ERROR;
			return m_ErrorCode;	
		}
		// allocate output data buffers
		m_pOutputBuffer[i]->dwBufferLength = m_OutBufferSize;
		m_pOutputBuffer[i]->dwFlags = 0;
		m_pOutputBuffer[i]->dwUser = NULL;
		m_pOutputBuffer[i]->dwLoops = NULL;
		if( (m_pOutputBuffer[i]->lpData = new char[m_OutBufferSize] ) == NULL )
		{
			OutClose();
			m_ErrorCode = MEMORY_ERROR;
			return m_ErrorCode;	
		}
		if(	(m_ErrorCode = waveOutPrepareHeader(m_hwvout, m_pOutputBuffer[i],
						sizeof *m_pOutputBuffer[i]) ) != MMSYSERR_NOERROR )
		{
			OutClose();	// m_ErrorCode = MMSYSERR_xxx
			return m_ErrorCode;	
		}
	}
	m_OutSampCnt = 0;
	for(INT i=0; i<INTP6_QUE_SIZE; i++)
		m_pOutQue[i] = 0.0;
	m_OutState = INTP6_FIR_SIZE-1;
	m_OutputOpen = TRUE;
	m_fOutputHoldoff = TRUE;
	return(0);
}


///////////////////////////////////////////////////////////////////////
// Writes 'Length' double's to the soundcard output.
//    parameters:
//		pData	= pointer to block of 'Length' double's to output.
//		Length	= Number of samples to write from pData. If is zero
//					then the sound output is flushed and closed.
// Returns:
//		'Length' if data was succesfully placed in the output buffers.
//       0 if output has finished( reached the specified sample limit )
//      -1		 if error ( use GetError() to retrieve error )
///////////////////////////////////////////////////////////////////////
LONG CSound::OutWrite( double* pData, INT Length )
{
double acc;
const double* Kptr;
double* Firptr;
INT i;
	if( !m_OutputOpen )		// return -1 if output not running.
	{
		m_ErrorCode = SOUNDOUT_ERR_NOTOPEN;
		return -1;
	}
	if( m_OutUnderflow )
	{
		m_ErrorCode = SOUNDOUT_ERR_UNDERFLOW;
		OutClose();
		return -1;			// return -1 if underflow
	}

	if( Length == 0 )	// need to flush partially filled buffer
	{					// and exit
		OutClose();
		if(m_ErrorCode == NO_ERRORS )
			return Length;
		else
			return -1;
	}else	// here to add new data to soundcard buffer queue
	{
		if(m_OutOversample)
		{
			if(m_OutFormat.wBitsPerSample == 16)
			{
				i = 0;
				while( m_OutBufPosition < m_OutBufferSize )
				{
					//Interpolate by 6 to get back up to 48000Hz
					if( ( (m_OutSampCnt++)%6 ) == 0 )	//calc first decimation filter every 2 samples
					{
						m_pOutQue[m_OutState/INTP6_VALUE] = pData[i++]; //get next output sample
					}
					acc = 0.0;
					Firptr = m_pOutQue;
					Kptr = FIRINTERP6 + INTP6_FIR_SIZE - m_OutState;
					for(int j=0; j<INTP6_QUE_SIZE; j++)
					{
						acc += ( (*(Firptr++))*(*Kptr) );
						Kptr += INTP6_VALUE;
					}
					if( --m_OutState < 0)
						m_OutState = INTP6_FIR_SIZE-1;

					m_usTemp.both = (SHORT)(acc);
					*(m_pOutputBuffer[m_OutHeadPtr]->lpData
						+ (m_OutBufPosition++) ) = m_usTemp.bytes.lsb;
					*(m_pOutputBuffer[m_OutHeadPtr]->lpData
						+ (m_OutBufPosition++) ) = m_usTemp.bytes.msb;
				}
			}
			else
			{
				i = 0;
				while( m_OutBufPosition < m_OutBufferSize )
				{
					//Interpolate by 6 to get back up to 48000Hz
					if( ( (m_OutSampCnt++)%6 ) == 0 )	//calc first decimation filter every 2 samples
					{
						m_pOutQue[m_OutState/INTP6_VALUE] = pData[i++]; //get next output sample
					}
					acc = 0.0;
					Firptr = m_pOutQue;
					Kptr = FIRINTERP6 + INTP6_FIR_SIZE - m_OutState;
					for(int j=0; j<INTP6_QUE_SIZE; j++)
					{
						acc += ( (*(Firptr++))*(*Kptr) );
						Kptr += INTP6_VALUE;
					}
					if( --m_OutState < 0)
						m_OutState = INTP6_FIR_SIZE-1;
					m_usTemp.both = (SHORT)(acc + 32768.0);
					*( m_pOutputBuffer[m_OutHeadPtr]->lpData + (m_OutBufPosition++) )
											= m_usTemp.bytes.msb;
				}
			}
		}
		else
		{
			if(m_OutFormat.wBitsPerSample == 16)
			{
				for( i=0; i < (Length*m_OutFormat.nChannels); i++ )
				{
					m_usTemp.both = (SHORT)(*(pData + i));
					*(m_pOutputBuffer[m_OutHeadPtr]->lpData
						+ (m_OutBufPosition++) ) = m_usTemp.bytes.lsb;
					*(m_pOutputBuffer[m_OutHeadPtr]->lpData
						+ (m_OutBufPosition++) ) = m_usTemp.bytes.msb;
				}
			}
			else
			{
				for( i=0; i < (Length*m_OutFormat.nChannels); i++ )
				{
					m_usTemp.both = (SHORT)(*(pData + i) + 32768.0);
					*( m_pOutputBuffer[m_OutHeadPtr]->lpData + (m_OutBufPosition++) )
											= m_usTemp.bytes.msb;
				}
			}
		}
		if( m_OutBufPosition >= m_OutBufferSize )  //send it if full
		{
			waveOutWrite( m_hwvout, m_pOutputBuffer[m_OutHeadPtr],
							sizeof *m_pOutputBuffer[m_OutHeadPtr] );
			m_OutBufPosition = 0;
			if(	m_fOutputHoldoff )   // holdoff logic doesn't start sound
			{						// until half the buffers are full
				if( m_OutHeadPtr >= NUM_OUTPUT_SNDBUFFERS/2 )
				{
					m_fOutputHoldoff = FALSE;
					waveOutRestart( m_hwvout );
				}
			}
			EnterCriticalSection(&m_CriticalSection);
			if( ++m_OutHeadPtr >= NUM_OUTPUT_SNDBUFFERS)	// handle wrap around
				m_OutHeadPtr = 0;
			if( m_OutHeadPtr == m_OutTailPtr) //if all buffers full then need to wait
			{
				// wait for mmsystem to free up a new buffer
				m_OutWaiting = TRUE;
				LeaveCriticalSection(&m_CriticalSection);
				if( WaitForSingleObject( m_OutEventHandle,TIMELIMIT )
											!= WAIT_OBJECT_0 )
				{
					m_ErrorCode = SOUNDOUT_ERR_TIMEOUT;
					OutClose();
					return -1;
				}
			}
			else
				LeaveCriticalSection(&m_CriticalSection);
		}
		m_OutSamplesWritten += Length;
		if( (m_OutSampleLimit != 0) && (m_OutSamplesWritten >= m_OutSampleLimit) )
		{
			OutClose();
			if(m_ErrorCode == NO_ERRORS )
				return 0;
			else
				return -1;
		}
		return Length;	// return number Samples accepted OK
	}
}



//////////////////////////////////////////////////////////////////////
//  Private function that waits for all the output buffers to finish
//   outputing.  Returns zero if OK else error code.
//////////////////////////////////////////////////////////////////////
UINT CSound::WaitForFlush()
{
	EnterCriticalSection(&m_CriticalSection);
	while( m_OutHeadPtr != m_OutTailPtr )
	{	// wait for all buffers to finish
		// wait for mmsystem to free up a new buffer
		m_OutWaiting = TRUE;
		LeaveCriticalSection(&m_CriticalSection);
		if( WaitForSingleObject( m_OutEventHandle,TIMELIMIT )
									!= WAIT_OBJECT_0 )
		{
			m_ErrorCode = SOUNDOUT_ERR_TIMEOUT;
			return m_ErrorCode;
		}
		EnterCriticalSection(&m_CriticalSection);
	}
	LeaveCriticalSection(&m_CriticalSection);
	return m_ErrorCode;
}

//////////////////////////////////////////////////////////////////////
//  Closes the Soundcard Output if open.
//////////////////////////////////////////////////////////////////////
void CSound::OutClose()
{
	m_OutputOpen = FALSE;
	if(m_hwvout != NULL)
	{
		if( !m_ErrorCode && (m_OutBufPosition > 0) )
		{		// flush out all remaining data in local buffers
			waveOutRestart( m_hwvout );	//make sure soundcard is running
			m_pOutputBuffer[m_OutHeadPtr]->dwBufferLength = m_OutBufPosition;
			waveOutWrite( m_hwvout, m_pOutputBuffer[m_OutHeadPtr],
								sizeof *m_pOutputBuffer[m_OutHeadPtr] );
			if( ++m_OutHeadPtr >= NUM_OUTPUT_SNDBUFFERS)	// handle wrap around
				m_OutHeadPtr = 0;
		}
		m_ErrorCode = WaitForFlush();		//wait to finish
		waveOutReset(m_hwvout);
		Sleep(100);
		for(INT i=0; i<NUM_OUTPUT_SNDBUFFERS; i++ )
		{
			if( m_pOutputBuffer[i] )
			{
				if( m_pOutputBuffer[i]->lpData != NULL )
				{
					if( m_pOutputBuffer[i]->dwFlags&WHDR_PREPARED )
					{
						waveOutUnprepareHeader(m_hwvout, m_pOutputBuffer[i],
							sizeof *m_pOutputBuffer[i]);
					}
					if( m_pOutputBuffer[i]->lpData )
					{
						delete m_pOutputBuffer[i]->lpData;
						m_pOutputBuffer[i]->lpData = NULL;
					}
				}
				if( m_pOutputBuffer[i] )
				{
					delete m_pOutputBuffer[i];
					m_pOutputBuffer[i] = NULL;
				}
			}
		}
		waveOutClose(m_hwvout);
		Sleep(50);
		m_hwvout = NULL;
	}
	if(m_OutEventHandle)
	{
		CloseHandle(m_OutEventHandle);
		m_OutEventHandle = NULL;
	}
}

////////////////////////////////////////////////////////////
//  callback Routine called by mmsystem when a buffer becomes full.
//      no system calls except SetEvent may be called from here.
//  ( not a member function of CSound Class )
////////////////////////////////////////////////////////////
void CALLBACK WaveInCallback( HWAVEIN m_hwvin, UINT uMsg, CSound* pCSound, 
							DWORD dwParam1, DWORD dwParam2 )
{
	if( uMsg == MM_WIM_DATA )	// ignore all but buff full messages
	{
		EnterCriticalSection(&pCSound->m_CriticalSection);
		if( ++pCSound->m_InHeadPtr >= NUM_INPUT_SNDBUFFERS)	//inc ptr
			pCSound->m_InHeadPtr = 0;	// handle wrap around
		if( pCSound->m_InHeadPtr == pCSound->m_InTailPtr )	//chk for overflo
			pCSound->m_InOverflow = TRUE;
		if(pCSound->m_InWaiting)	//if user thread is waiting for buffer
		{
			pCSound->m_InWaiting = FALSE;
			SetEvent( pCSound->m_InEventHandle);	//signal it
		}
		LeaveCriticalSection(&pCSound->m_CriticalSection);
	}
}

////////////////////////////////////////////////////////////
//   callback Routine called by mmsystem when a buffer becomes empty.
//      no system calls except SetEvent may be called from here.
//  ( not a member function of CSound Class )
////////////////////////////////////////////////////////////
void CALLBACK WaveOutCallback( HWAVEOUT m_hwvout, UINT uMsg, CSound* pCSound, 
							DWORD dwParam1, DWORD dwParam2 )
{
	if( uMsg == WOM_DONE )	// ignore all but buffer empty messages
	{
		EnterCriticalSection(&pCSound->m_CriticalSection);
		int tmptail = pCSound->m_OutTailPtr + 2;
		if( tmptail >= NUM_OUTPUT_SNDBUFFERS)	//inc ptr
			tmptail = tmptail - NUM_OUTPUT_SNDBUFFERS;	// handle wrap around

		if( pCSound->m_OutHeadPtr == tmptail ) //chk for underflo
			pCSound->m_OutUnderflow = TRUE;
		
		if( ++pCSound->m_OutTailPtr >= NUM_OUTPUT_SNDBUFFERS)	//inc ptr
			pCSound->m_OutTailPtr = 0;	// handle wrap around

		if(pCSound->m_OutWaiting)	//if user thread is waiting for buffer
		{
			pCSound->m_OutWaiting = FALSE;
			SetEvent( pCSound->m_OutEventHandle);
		}
		LeaveCriticalSection(&pCSound->m_CriticalSection);
	}
}

///////////////////////////////////////////////////////


