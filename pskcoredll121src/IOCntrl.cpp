//////////////////////////////////////////////////////////////////////
// IOCntrl.cpp: implementation of the CIOCntrl class.
//
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

#include <float.h>
#include <afxmt.h>
#include "stdafx.h"
#include "Errorcodes.h"
#include "IOCntrl.h"
#include "PSKCore.h"
#include "perform.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAXTIMER 50		//wait for 5 seconds for thread to start

#define IOM_NORMAL 0		//Use soundcard only
#define IOM_WAVEIN (1<<0)	//1==use input wave file, 0==use soundcard
#define IOM_WAVELOG (1<<1)	//1==copy soundcard in to outwave file, 0==don't
#define IOM_WAVETX (1<<2)	//1==Save Tx to outwave file, 0==don't
#define IOM_NOTXSOUND (1<<3)//1==No Tx to soundcard, 0==Tx Output to soundcard
#define IOM_ECHOINPUT (1<<4)//1==Echo input to SoundcarOut 0== No Echo

#define PERCENTMODE_OFF 0
#define PERCENTMODE_IN 1
#define PERCENTMODE_OUT 2

double DebugFP1=0.0;
double DebugFP2=0.0;
INT DebugINT = 0;
//#define USE_PERFORMANCE 1		//displays performance info
//#define USE_NOISEGEN 1		//internal additive noise


// Non Class routine for launching the worker thread

static UINT ThreadLauncher( CIOCntrl* pIO)
{
	pIO->ProcessLoop();
	return 4;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIOCntrl::CIOCntrl()
{
	m_DLLRevision = 121;	//DLL revision number.(100 = 1.00) AA6YQ
	m_fProcThreadQuit = FALSE;
	m_RXState = TRUE;
	m_DLLStatus = DLLSTAT_OFF;
	for(INT i=0; i<MAX_CHANNELS; i++)
		m_pPSKDet[i] = NULL;
	m_pProcThread = NULL;
	m_TxMode = BPSK_MODE;
	m_hComPort = INVALID_HANDLE_VALUE;
	m_PTTPort = 0;
	m_ComPortMode = 0;
	m_InputCardNum = -1;
	m_OutputCardNum = -1;  //AA6YQ 1.20
	m_MaxNumRXChannels = MAX_CHANNELS;
	m_AllChannelsActive = TRUE;
	m_ErrorMessage = _T("");
	m_InWavePath = _T("");
	m_OutWavePath = _T("");
	m_ThreadActive = FALSE;
	m_hWnd = NULL;
	m_SatMode = FALSE;
	m_TXRunning = FALSE;
	m_IOMode = IOM_NORMAL;
	m_InFilePercentage = 0;
	m_OutFilePercentage = 0;
	m_InTotalSamples = 0;
	m_OutTotalSamples = 0;
	m_WaveOutLimit = 0;
	m_ClockError = 1.0;
// Setup Soundcard values
	m_WaveFormatEx.wFormatTag = WAVE_FORMAT_PCM;
	m_WaveFormatEx.nChannels = 1;
	m_WaveFormatEx.wBitsPerSample = 16;
	m_WaveFormatEx.nSamplesPerSec = SAMP_RATE;
	m_WaveFormatEx.nBlockAlign = 
			m_WaveFormatEx.nChannels *(m_WaveFormatEx.wBitsPerSample/8);
	m_WaveFormatEx.nAvgBytesPerSec = 
			m_WaveFormatEx.nSamplesPerSec *
			m_WaveFormatEx.nBlockAlign;
	m_WaveFormatEx.cbSize = 0;
	InitializeCriticalSection(&m_CriticalSection);
	m_PSKMod.InitPSKMod(m_WaveFormatEx.nSamplesPerSec, NULL,FALSE);
}

CIOCntrl::~CIOCntrl()
{
	StopIO();
	if( m_hComPort != INVALID_HANDLE_VALUE )
	{
		EscapeCommFunction(m_hComPort, CLRRTS);
		EscapeCommFunction(m_hComPort, CLRDTR);
		CloseHandle( m_hComPort );
		m_hComPort = INVALID_HANDLE_VALUE;
	}
	DeleteResources();
	for(INT i=0; i<MAX_CHANNELS; i++)
	{
		if(m_pPSKDet[i])
		{
			delete m_pPSKDet[i];
			m_pPSKDet[i] = NULL;
		}
	}
	DeleteCriticalSection(&m_CriticalSection);
}

//*************************************************************************
//**************  routines exposed to the outside  ************************
//*************************************************************************

///////////////////////////////////////////////////////////////////////////
//  Stop process thread proceedure and wait for it to finish
//  Called by main program thread to signal data processing thread to stop
//	and then wait for it to stop before returning.
///////////////////////////////////////////////////////////////////////////
void CIOCntrl::StopIO()
{
	if( m_pProcThread != NULL )		//if thread is already running
	{
		AbortTX();
		m_ThreadActive = FALSE;
		m_fProcThreadQuit = TRUE;	// stop it
		::WaitForSingleObject(m_pProcThread->m_hThread,	2500);
		delete m_pProcThread;
		m_pProcThread = NULL;
		m_DLLStatus = DLLSTAT_OFF;

#ifdef USE_PERFORMANCE
	ReadPerformance();
#endif
	}
}


///////////////////////////////////////////////////////////////////////////
//   Called to start the I/O thread running
//  maxchannels is the maximum number of RX channels that will be active at
// any one time.  If the caller has not specifically disabled any channels,
// they will all be active at the start.
///////////////////////////////////////////////////////////////////////////
INT CIOCntrl::StartIO(HWND h_Wnd, INT maxchannels, INT IOMode)
{
INT Timer;
	if( m_pProcThread == NULL)
	{
		m_hWnd = h_Wnd;
		m_IOMode = IOMode;
		if(maxchannels>MAX_CHANNELS)
			return DLLERR_MEMORY;
		m_MaxNumRXChannels = maxchannels;
		if( m_AllChannelsActive )	//user has not modified active channel list
		{							//so create all and make all active
			for( INT i=0; i<m_MaxNumRXChannels; i++)
			{
				if( !ChkDetExists(i) )
					return DLLERR_MEMORY;
			}
			// delete any previously created channels greater than the new max
			for(int  i = m_MaxNumRXChannels; i<MAX_CHANNELS; i++)
			{
				if( m_pPSKDet[i] != NULL )
				{
					delete m_pPSKDet[i];
					m_pPSKDet[i] = NULL;
				}
			}
		}
		m_PSKMod.m_hWnd = m_hWnd;
		m_ThreadActive = FALSE;
		m_DLLStatus = DLLSTAT_OFF;
		m_fProcThreadQuit = FALSE;
		m_pProcThread = AfxBeginThread(
			(AFX_THREADPROC)ThreadLauncher,	//thread function
			(LPVOID)this,					// ptr to this class
			THREAD_PRIORITY_NORMAL, 		//give worker thread priority
			0,								// same stack size
			CREATE_SUSPENDED,				// don't let it start yet
			NULL );							// same security attributes
		m_pProcThread->m_bAutoDelete = FALSE;	// keep thread object around
		m_pProcThread->ResumeThread ();	//let er rip
		Timer = MAXTIMER;
		while( (m_DLLStatus == DLLSTAT_OFF) && !m_fProcThreadQuit && (Timer-- > 0) )
			::Sleep(100);
	}
	::Sleep(100);
	if(m_DLLStatus >= 0)
		return m_DLLStatus;
	else
		return DLLERR_NONE;
}

///////////////////////////////////////////////////////////////////////////
// Check to see if det object exists and create if not
//  returns FALSE if can't create object, TRUE if it now exists
///////////////////////////////////////////////////////////////////////////
BOOL CIOCntrl::ChkDetExists(INT chan)
{
BOOL res = TRUE;
	if( (chan > (MAX_CHANNELS-1) ) || ( chan < 0) )
	{
		res = FALSE;		//error if channel num out of range
	}
	else
	{
		if( m_pPSKDet[chan] == NULL )	//if NULL then must create it
		{
			EnterCriticalSection(&m_CriticalSection);
			m_pPSKDet[chan] = new CPSKDet;	//create PSK Detector object
			ASSERT( m_pPSKDet[chan]);
			if( m_pPSKDet[chan] == NULL )
			{
				res = FALSE;
			}
			else	//initialize the new detector object
			{
				m_pPSKDet[chan]->Init(SAMP_RATE, BUF_SIZE, NULL, chan);
				m_pPSKDet[chan]->ResetDetector();
				m_pPSKDet[chan]->m_hWnd = m_hWnd;	//give object user's msg handle
			}
			LeaveCriticalSection(&m_CriticalSection);
		}
	}
	return res;
}

///////////////////////////////////////////////////////////////////////////
// Call to Enable or disable a receiver channel either before or after the 
// worker thread is started. Returns FALSE if can't create/delete a channel
// TRUE if ok
///////////////////////////////////////////////////////////////////////////
BOOL CIOCntrl::EnableRXChannel(INT chan, BOOL enable)
{
BOOL ret = TRUE;
	m_AllChannelsActive = FALSE;	//user is modifying active channel list
	if(enable)
	{
		ret = ChkDetExists(chan);	//create if doesn't already exist
	}
	else	//delete detector object if it exists
	{
		EnterCriticalSection(&m_CriticalSection);
		if( m_pPSKDet[chan] != NULL )
		{
			delete m_pPSKDet[chan];
			m_pPSKDet[chan] = NULL;
		}
		LeaveCriticalSection(&m_CriticalSection);
	}
	return ret;
}



///////////////////////////////////////////////////////////////////////////
// Call to see if te specified channel is Active.
// Returns TRUE if the channel is active
// FALSE if not active
///////////////////////////////////////////////////////////////////////////
BOOL CIOCntrl::IsRxChannelActive(long chan)
{
	return( m_pPSKDet[chan] != NULL );
}

//(dec) 20-Dec-01 Added this back in after Moe dropped it in V1.14
///////////////////////////////////////////////////////////////////////////
// Call to retrieve the number of active receive channels
///////////////////////////////////////////////////////////////////////////
INT CIOCntrl::GetNumActiveRXChannels()
{
INT num=0;
	for(INT i=0; i<m_MaxNumRXChannels; i++)
	{
		if( IsRxChannelActive(i) )
			num++;
	}
	return num;
}

///////////////////////////////////////////////////////////////////////////
// Starts the TX process running and stops the receive process
///////////////////////////////////////////////////////////////////////////
void CIOCntrl::StartTX()
{
	if(	m_DLLStatus != DLLSTAT_OFF)
		m_RXState = FALSE;
}

///////////////////////////////////////////////////////////////////////////
// Stops the TX process after all characters have been sent and starts the
//      receive process.
///////////////////////////////////////////////////////////////////////////
void CIOCntrl::StopTX()
{
	if(!m_RXState)
	{
		m_PSKMod.SetAutoShutoff(TRUE);
		StatusChange(DLLSTAT_TXFINISHING);
	}
}

///////////////////////////////////////////////////////////////////////////
// Aborts the TX process without waiting for all characters to be sent 
//    and starts the receive process.
//  If it is in the RX mode, the TX character buffer is cleared.
///////////////////////////////////////////////////////////////////////////
void CIOCntrl::AbortTX()
{
	m_PSKMod.ClrQue();
	m_PSKMod.SetAutoShutoff(FALSE);
	m_PSKMod.SetAutoCWID(FALSE);
	if(!m_RXState )
		m_RXState = TRUE;
}

///////////////////////////////////////////////////////////////////////////
// Initializes the com port 
///////////////////////////////////////////////////////////////////////////
LONG CIOCntrl::SetComPort(long portnum, long mode)
{
char szPort[10];
HANDLE hComPort = INVALID_HANDLE_VALUE;
BOOL ret = FALSE;
	if( portnum == 0)
	{
		m_PTTPort = 0;
		m_ComPortMode = 0;
		EnterCriticalSection(&m_CriticalSection);
		if( m_hComPort != INVALID_HANDLE_VALUE )
		{
			EscapeCommFunction(m_hComPort, CLRRTS);
			EscapeCommFunction(m_hComPort, CLRDTR);
			CloseHandle( m_hComPort );
			m_hComPort = INVALID_HANDLE_VALUE;
		}
		LeaveCriticalSection(&m_CriticalSection);
	}
	else
	{
		EnterCriticalSection(&m_CriticalSection);
		if( m_hComPort != INVALID_HANDLE_VALUE )
		{
			EscapeCommFunction(m_hComPort, CLRRTS);
			EscapeCommFunction(m_hComPort, CLRDTR);
			CloseHandle( m_hComPort );
			m_hComPort = INVALID_HANDLE_VALUE;
		}
		wsprintf( szPort, "COM%d", portnum );
		hComPort = CreateFile( szPort, 0, 0, NULL,
							OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
		if( hComPort == INVALID_HANDLE_VALUE )
		{
			m_PTTPort = 0;
			m_ComPortMode = 0;
		}
		else
		{
			EscapeCommFunction(hComPort, CLRRTS);
			EscapeCommFunction(hComPort, CLRDTR);
			CloseHandle( hComPort );
			m_hComPort = INVALID_HANDLE_VALUE;
			m_ComPortMode = mode&0x0003;
			m_PTTPort = portnum;
			ret = TRUE;
		}
		LeaveCriticalSection(&m_CriticalSection);
	}
	return (LONG)ret;
}

///////////////////////////////////////////////////////////////////////////
// Initializes the FFT 
///////////////////////////////////////////////////////////////////////////
void CIOCntrl::SetFFTParams(INT ave,double gain, INT type )
{
	m_Cfft.SetFFTParams( ave, gain, type, m_ClockError);
}

///////////////////////////////////////////////////////////////////////////
// calculates FFT on latest data and put's it in users array
///////////////////////////////////////////////////////////////////////////
BOOL CIOCntrl::GetFFTData(LONG* pData, LONG start, LONG end)
{
BOOL overflo = FALSE;
	if( !m_fProcThreadQuit )
	{
		EnterCriticalSection(&m_CriticalSection);
		overflo = m_Cfft.GetFFTData( start, end, pData, m_RXState||m_SatMode);
		LeaveCriticalSection(&m_CriticalSection);
	}
	else
	{
		for(INT i=start; i<=end; i++)
			pData[i] = 0;
	}
	if(m_DLLStatus != DLLSTAT_RX)	//keep tx level from triggering
		return FALSE;				// an overflo
	else
		return overflo;
}

///////////////////////////////////////////////////////////////////////////
// Gets latest raw data and put's it in users array
///////////////////////////////////////////////////////////////////////////
BOOL CIOCntrl::GetRawData(LONG* pData, LONG start, LONG end)
{
BOOL Ovrld = FALSE;
	if( !m_fProcThreadQuit )
	{
		EnterCriticalSection(&m_CriticalSection);
		for(INT i=start; i<=end;i++)
		{
			pData[i] = (INT)m_pBuffer[i];
			if( pData[i] > 29491 )	//flag overload if within 10% of max
				Ovrld = TRUE;
		}
		LeaveCriticalSection(&m_CriticalSection);
	}
	else
	{
		for(INT i=start; i<=end; i++)
			pData[i] = 0;
	}
	return Ovrld;
}

void CIOCntrl::SetRXFrequency(long freq, long CaptureRange, long chan)
{
INT pkfreq = freq;
	if(CaptureRange>100)
		CaptureRange = 100;
	if(CaptureRange<0)
		CaptureRange = 0;
	if( (CaptureRange>10) && (m_DLLStatus != DLLSTAT_OFF) )
	{
		EnterCriticalSection(&m_CriticalSection);
		pkfreq = m_Cfft.FindPKfreq( freq, CaptureRange );
		LeaveCriticalSection(&m_CriticalSection);
	}
	if( m_pPSKDet[chan] != NULL )
		m_pPSKDet[chan]->SetRXFrequency(pkfreq);
}

void CIOCntrl::SetClockErrorAdjustment(INT ppm)
{
	for( INT i=0; i<m_MaxNumRXChannels; i++)
	{
		if( m_pPSKDet[i] != NULL )
			m_pPSKDet[i]->SetSampleClkAdj(ppm);
	}
	m_PSKMod.SetSampleClkAdj(ppm);
	m_ClockError = 1.0 - (double)ppm/1000000.0;
}


void CIOCntrl::GetErrorMsg(CString& err)
{
	err = m_ErrorMessage;
}


void CIOCntrl::RewindInput(INT Blocks)
{
	if(m_RXState||m_SatMode)
	{
		if( m_SoundDev.m_InputOpen)
		{
			m_SoundDev.RewindInputData(Blocks);
		}
		else
		{
			if(m_InTotalSamples >= BUF_SIZE*Blocks)
			{
				if( m_WaveDev.RewindInputData(Blocks) )
					m_InTotalSamples = m_InTotalSamples - BUF_SIZE*Blocks;
			}
		}
	}
}

INT CIOCntrl::SetInputWavePath(CString sPath, PINT pLengthTime, INT Offset)
{
INT error;
DWORD filesize;
WAVEFORMATEX WaveFormatEx;
	m_InWavePath = _T("");
	m_WaveInOffset = Offset*SAMP_RATE;
	error = m_WaveDev.InOpen(&sPath, &WaveFormatEx, 0, &filesize, 0);
	if( (error == 0) && (WaveFormatEx.nChannels == 1) &&
			(WaveFormatEx.nSamplesPerSec == SAMP_RATE) &&
			(WaveFormatEx.wBitsPerSample == 16) )
	{
		m_InWavePath = sPath;
		*pLengthTime = filesize/SAMP_RATE;
		if(m_WaveInOffset > (INT)filesize)
			m_WaveInOffset = filesize-1;
		return 0;
	}
	else
	{
		*pLengthTime = 0;
		ProcessError( error );
		return m_DLLStatus;
	}
}

INT CIOCntrl::SetOutputWavePath(CString sPath, INT TimeLimit, INT Append)
{
INT ret;
	if(sPath != "")
	{
		m_OutWavePath = sPath;
		ret = 0;
	}
	else
	{
		ProcessError( WAVOUT_ERR_OPEN );
		ret = DLLERR_WAVOUT_OPEN;
	}
	m_WaveOutLimit = TimeLimit;
	m_WaveOutAppend = Append;
	return ret;
}

//*************************************************************************
//**********************  local routines  *********************************
//*************************************************************************

///////////////////////////////////////////////////////////////////////////
//  Main I/O thread processing loop
///////////////////////////////////////////////////////////////////////////
void CIOCntrl::ProcessLoop()
{
INT i;
INT ErrorCode;
DWORD insize=0;
WAVEFORMATEX WaveFormatEx;

#ifdef USE_PERFORMANCE
	InitPerformance();
#endif

	m_InTotalSamples = 0;
	m_OutTotalSamples = 0;
	m_InFilePercentage = 0;
	m_OutFilePercentage = 0;
	if( m_IOMode & IOM_WAVEIN)
	{
		//Open wavefile for input if it is the source
		ErrorCode = m_WaveDev.InOpen( &m_InWavePath, &WaveFormatEx,
												BUF_SIZE, &insize, m_WaveInOffset);
		if( ErrorCode != NO_ERROR )
		{
			ProcessError( ErrorCode );
			StopThread();
		}
		if( (WaveFormatEx.nChannels != 1) ||
			(WaveFormatEx.nSamplesPerSec != SAMP_RATE) ||
			(WaveFormatEx.wBitsPerSample != 16) )
		{
			ProcessError( WAVIN_ERR_OPEN );
			StopThread();
		}
		m_InSampleLimit = insize;
		::PostMessage( m_hWnd, MSG_STATUSCHANGE, DLLSTAT_INWAVEFILESTATUS, 0);
	}
	if( m_IOMode & (IOM_WAVELOG|IOM_WAVETX) )
	{
		//Open wavefile for Output if it is specified
		ErrorCode = m_WaveDev.OutOpen( &m_OutWavePath, &m_WaveFormatEx, BUF_SIZE,
										m_WaveOutLimit*8000, !m_WaveOutAppend);
		if( ErrorCode != NO_ERROR )
		{
			ProcessError( ErrorCode );
			StopThread();
		}
		m_OutSampleLimit = m_WaveOutLimit*8000;
		::PostMessage( m_hWnd, MSG_STATUSCHANGE, DLLSTAT_OUTWAVEFILESTATUS, 0);
	}
	for( i=0; i<m_MaxNumRXChannels; i++)
	{
		EnterCriticalSection(&m_CriticalSection);
		if( m_pPSKDet[i] != NULL )
		{
			m_pPSKDet[i]->ResetDetector();
			m_pPSKDet[i]->m_hWnd = m_hWnd;
		}
		LeaveCriticalSection(&m_CriticalSection);
	}
	m_Cfft.ResetFFT();
	m_PSKMod.InitPSKMod(m_WaveFormatEx.nSamplesPerSec, m_hWnd, m_SatMode);
	m_ThreadActive = TRUE;
	StatusChange(DLLSTAT_RX);
	AbortTX();
	Sleep(50);
	PTTCntrl(FALSE);
	m_TXRunning = TRUE;
	m_RXState = TRUE;
//m_pPSKMod->SetTXMode(TUNE_MODE);
//m_pPSKMod->SetTXMode(BPSK_MODE);
//m_pPSKMod->SetTXMode(QPSKU_MODE);
//m_pPSKMod->InitPSKMod( SAMP_RATE, m_hWnd);

	while( !m_fProcThreadQuit )
	{
		if(m_SatMode)	// if full duplex operation
		{
			if( m_RXState )
			{
				if(m_TXRunning)	//if just finished TX mode or first time in
				{
					// Open soundcard if it is the source and not already open
					if( !(m_IOMode & IOM_WAVEIN) && !m_SoundDev.m_InputOpen)
					{
						// Open Soundcard for input
						ErrorCode = m_SoundDev.InOpen( &m_WaveFormatEx, BUF_SIZE,
																0, m_InputCardNum);
						if( ErrorCode != NO_ERROR )
						{
							ProcessError( ErrorCode );
							StopThread();
						}
					}
					if( m_SoundDev.m_OutputOpen && !(m_IOMode & IOM_ECHOINPUT))
					{
						m_SoundDev.OutClose();
					}
					StatusChange(DLLSTAT_RX);
					Sleep(50);	//delay to give xmitters time to switch
					PTTCntrl(FALSE);
					m_TXRunning = FALSE;
				}
			}
			else
			{
				if(!m_TXRunning)	//if just gone to TX mode
				{
					if( ((m_IOMode & IOM_NOTXSOUND)==0) && !m_SoundDev.m_OutputOpen)
					{
						// Open Soundcard for Output
						ErrorCode = m_SoundDev.OutOpen( &m_WaveFormatEx, BUF_SIZE,
																0, m_OutputCardNum); //AA6YQ 1.20
						if( ErrorCode != NO_ERROR )
						{
							ProcessError( ErrorCode );
							StopThread();
						}
					}
					m_PSKMod.InitPSKMod(m_WaveFormatEx.nSamplesPerSec, m_hWnd, m_SatMode);
					StatusChange(DLLSTAT_TX);
					PTTCntrl(TRUE);
					Sleep(50);	//delay to give xmitters time to switch
					m_TXRunning = TRUE;
				}
				ProcessTX();
			}
			ProcessRX();
		}
		else	// half duplex non-satellite operation
		{		
			if( m_RXState )
			{
				if(m_TXRunning)	//if just finished TX mode
				{
					for( i=0; i<m_MaxNumRXChannels; i++)
					{
						EnterCriticalSection(&m_CriticalSection);
						if( m_pPSKDet[i] != NULL )
							m_pPSKDet[i]->ResetDetector();
						LeaveCriticalSection(&m_CriticalSection);
					}
					m_Cfft.ResetFFT();
					StatusChange(DLLSTAT_RX);
					if( m_SoundDev.m_OutputOpen && !(m_IOMode & IOM_ECHOINPUT))
					{
						m_SoundDev.OutClose();
						Sleep(100);	//delay to give xmitters time to switch
						PTTCntrl(FALSE);
					}
					// Open soundcard if it is the source and not already open
					if( !(m_IOMode & IOM_WAVEIN) && !m_SoundDev.m_InputOpen)
					{
						// Open Soundcard for input
						ErrorCode = m_SoundDev.InOpen( &m_WaveFormatEx, BUF_SIZE,
																0, m_InputCardNum);
						if( ErrorCode != NO_ERROR )
						{
							ProcessError( ErrorCode );
							StopThread();
						}
					}
					if( m_IOMode & IOM_ECHOINPUT )	//if need to echo input to soundcard out
					{
						if( !m_SoundDev.m_OutputOpen)
						{	// Open Soundcard for Echo Output
							ErrorCode = m_SoundDev.OutOpen( &m_WaveFormatEx, BUF_SIZE,
															0, m_OutputCardNum); //AA6YQ 1.20
							if( ErrorCode != NO_ERROR )
							{
								ProcessError( ErrorCode );
								StopThread();
							}
						}
					}
					m_TXRunning = FALSE;
				}
				ProcessRX();
			}
			else
			{
				if(!m_TXRunning)	//if just finished Rx mode
				{
					if( m_SoundDev.m_InputOpen)
					{
						m_SoundDev.InClose();
					}
					if( ((m_IOMode & IOM_NOTXSOUND) ==0) && !m_SoundDev.m_OutputOpen)
					{
						// Open Soundcard for Output
						ErrorCode = m_SoundDev.OutOpen( &m_WaveFormatEx, BUF_SIZE,
																0, m_OutputCardNum); //AA6YQ 1.20
						if( ErrorCode != NO_ERROR )
						{
							ProcessError( ErrorCode );
							StopThread();
						}
						PTTCntrl(TRUE);
						Sleep(100);	//delay to give xmitters time to switch
					}
					m_PSKMod.InitPSKMod(m_WaveFormatEx.nSamplesPerSec, m_hWnd, m_SatMode);
					StatusChange(DLLSTAT_TX);
					m_TXRunning = TRUE;
				}
				ProcessTX();
			}
		}
	}
	m_RXState = TRUE;
	m_ThreadActive = FALSE;
	DeleteResources();		//kill all resources
	Sleep(50);
	PTTCntrl(FALSE);

}

////////////////////////////////////////////////////////////////////
// Process for Receiving audio data from soundcard
////////////////////////////////////////////////////////////////////
void CIOCntrl::ProcessRX()
{
INT i;
INT result;
INT ErrorCode;
	if( m_IOMode & IOM_WAVEIN)	// Get data from wavefile
	{
		if( (ErrorCode = m_WaveDev.InRead( m_pBuffer, BUF_SIZE )) != BUF_SIZE)
		{
			if( ErrorCode < 0)
			{
				ErrorCode = m_WaveDev.GetError();
				ProcessError( ErrorCode );
			}
			StatusChange(DLLSTAT_INWAVEDONE);
			m_WaveDev.InClose();
			::PostMessage( m_hWnd, MSG_STATUSCHANGE, DLLSTAT_INWAVEFILESTATUS, 100);
			StopThread();
		}
		m_InTotalSamples += BUF_SIZE;
		if( (m_InSampleLimit > 0) )
		{
			INT Percent = (100*m_InTotalSamples)/m_InSampleLimit;
			if(Percent != m_InFilePercentage)
			{
				::PostMessage( m_hWnd, MSG_STATUSCHANGE, DLLSTAT_INWAVEFILESTATUS, Percent);
				m_InFilePercentage = Percent;
			}
		}
		Sleep(25);		// delay a bit so Windows msg system doesn't clog up
	}
	else	//Get data from soundcard
	{
		if( (result = m_SoundDev.InRead( m_pBuffer, BUF_SIZE )) != BUF_SIZE)
		{
			ErrorCode = m_SoundDev.GetError();
			if( (ErrorCode == SOUNDIN_ERR_OVERFLOW) ||
				(ErrorCode == SOUNDIN_ERR_TIMEOUT) )
			{								//cpu couldn't keep up
											// so try again
				m_SoundDev.InClose();
				StatusChange(DLLSTAT_CPUSLOW);
				ErrorCode = m_SoundDev.InOpen( &m_WaveFormatEx, BUF_SIZE, 0, m_InputCardNum);
			}
			if( ErrorCode != NO_ERRORS)	//must be some other soundcard error
			{
				m_SoundDev.InClose();
				ProcessError( ErrorCode );
				StopThread();
			}
		}
		Sleep(0);		// delay a bit so Windows msg system doesn't clog up
	}
//		m_pPSKMod->CalcPSK( m_pBuffer, BUF_SIZE );
//		AddGaussian(m_pBuffer, BUF_SIZE, 16384.0/5.0);

//////////  Loop through all the receiver channels and decode each one
#ifdef USE_PERFORMANCE
StartPerformance();
#endif
	for( i=0; i<m_MaxNumRXChannels; i++)
	{
		EnterCriticalSection(&m_CriticalSection);
		if( m_pPSKDet[i] != NULL )
			m_pPSKDet[i]->ProcPSKDet(m_pBuffer);
		LeaveCriticalSection(&m_CriticalSection);
	}
#ifdef USE_PERFORMANCE
StopPerformance();
#endif
	EnterCriticalSection(&m_CriticalSection);
	m_Cfft.CalcFFT( m_pBuffer);
	if( m_pPSKDet[0] != NULL )
	{
		LeaveCriticalSection(&m_CriticalSection);
		::PostMessage( m_hWnd, MSG_DATARDY, m_pPSKDet[0]->GetRXFrequency(),
								m_pPSKDet[0]->GetSignalLevel());
	}
	else
	{
		LeaveCriticalSection(&m_CriticalSection);
		::PostMessage( m_hWnd, MSG_DATARDY, 1500, 0 );
	}
	if( (m_IOMode & IOM_WAVELOG) )	//if need to log rx audio to wave file
	{
		if( (ErrorCode = m_WaveDev.OutWrite( m_pBuffer, BUF_SIZE )) != BUF_SIZE)
		{
			if( ErrorCode < 0)
			{
				ErrorCode = m_WaveDev.GetError();
				ProcessError( ErrorCode );
			}
			StatusChange(DLLSTAT_OUTWAVEDONE);
			m_WaveDev.OutClose();	//time limit expired
			::PostMessage( m_hWnd, MSG_STATUSCHANGE, DLLSTAT_OUTWAVEFILESTATUS, 100);
			StopThread();
		}
		m_OutTotalSamples += BUF_SIZE;
		if( (m_OutSampleLimit > 0) )
		{
			INT Percent = (100*m_OutTotalSamples)/m_OutSampleLimit;
			if(Percent != m_OutFilePercentage)
			{
				::PostMessage( m_hWnd, MSG_STATUSCHANGE, DLLSTAT_OUTWAVEFILESTATUS, Percent);
				m_OutFilePercentage = Percent;
			}
		}
	}
	if( (m_IOMode & IOM_ECHOINPUT) && !m_TXRunning )	//if need to echo input to soundcard out
	{
		if( (result = m_SoundDev.OutWrite( m_pBuffer, BUF_SIZE )) != BUF_SIZE)
		{
			ErrorCode = m_SoundDev.GetError();
			if( (ErrorCode == SOUNDOUT_ERR_UNDERFLOW) ||
				(ErrorCode == SOUNDOUT_ERR_TIMEOUT) )
			{								//cpu couldn't keep up
											// so try again
				m_SoundDev.OutClose();
				StatusChange(DLLSTAT_CPUSLOW);
				ErrorCode = m_SoundDev.OutOpen( &m_WaveFormatEx, BUF_SIZE, 0, m_OutputCardNum);  //AA6YQ 1.20
			}
			if( ErrorCode != NO_ERRORS)	//must be some other soundcard error
			{
				m_SoundDev.OutClose();
				ProcessError( ErrorCode );
				StopThread();
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////
// Processes transmit data while in TX mode
///////////////////////////////////////////////////////////////////////
void CIOCntrl::ProcessTX()
{
INT result;
INT ErrorCode;
	m_PSKMod.CalcPSK( m_pBuffer, BUF_SIZE );	//generate the tx audio samples
	if( ((m_IOMode & IOM_NOTXSOUND) == 0) )		//send to sound card output?
	{
		if( (result = m_SoundDev.OutWrite( m_pBuffer, BUF_SIZE )) != BUF_SIZE)
		{
			ErrorCode = m_SoundDev.GetError();
			if( (ErrorCode == SOUNDOUT_ERR_UNDERFLOW) ||
				(ErrorCode == SOUNDOUT_ERR_TIMEOUT) )
			{								//cpu couldn't keep up
											// so try again
				m_SoundDev.OutClose();
				StatusChange(DLLSTAT_CPUSLOW);
				ErrorCode = m_SoundDev.OutOpen( &m_WaveFormatEx, BUF_SIZE, 0, m_OutputCardNum); //AA6YQ 1.20
			}
			if( ErrorCode != NO_ERRORS)	//must be some other soundcard error
			{
				m_SoundDev.OutClose();
				ProcessError( ErrorCode );
				StopThread();
			}
		}
	}
	else
		Sleep(25);		// delay a bit so Windows msg system doesn't clog up
	if( (m_IOMode & IOM_WAVETX) )	//if need to write Tx data to wave file
	{
		if( (ErrorCode = m_WaveDev.OutWrite( m_pBuffer, BUF_SIZE )) != BUF_SIZE)
		{
			if( ErrorCode < 0)
			{
				ErrorCode = m_WaveDev.GetError();
				ProcessError( ErrorCode );
			}
			StatusChange(DLLSTAT_OUTWAVEDONE);
			m_WaveDev.OutClose();	//time limit expired
			::PostMessage( m_hWnd, MSG_STATUSCHANGE, DLLSTAT_OUTWAVEFILESTATUS, 100);
			StopThread();
		}
		m_OutTotalSamples += BUF_SIZE;
		if( (m_OutSampleLimit > 0) )
		{
			INT Percent = (100*m_OutTotalSamples)/m_OutSampleLimit;
			if(Percent != m_OutFilePercentage)
			{
				::PostMessage( m_hWnd, MSG_STATUSCHANGE, DLLSTAT_OUTWAVEFILESTATUS, Percent);
				m_OutFilePercentage = Percent;
				Sleep(25);
			}
		}
	}
	else	// here if neither sound or wave file output just delay
		if( (m_IOMode & IOM_NOTXSOUND) && !(m_IOMode & IOM_WAVETX) )
			Sleep(225);

	if(!m_SatMode)	// if half duplex operation then create display fft of tx data
	{
		EnterCriticalSection(&m_CriticalSection);
		m_Cfft.CalcFFT( m_pBuffer );
		if( m_pPSKDet[0] != NULL )
		{
			LeaveCriticalSection(&m_CriticalSection);
			::PostMessage( m_hWnd, MSG_DATARDY, m_pPSKDet[0]->GetRXFrequency(),
								m_pPSKDet[0]->GetSignalLevel());
		}
		else
		{
			LeaveCriticalSection(&m_CriticalSection);
			::PostMessage( m_hWnd, MSG_DATARDY, 1500, 0 );
		}
	}
	if( !m_PSKMod.GetTXState() )
	{
		m_RXState = TRUE;
	}
}

///////////////////////////////////////////////////////////////////////////
//  Free resources and exit process thread
//   Called by process thread itself to shut itself off if not signaled by
//  main program thread.
///////////////////////////////////////////////////////////////////////////
void CIOCntrl::StopThread()
{
//  if any resources are open, kill em.
	m_pProcThread->m_bAutoDelete = TRUE;	//make thread clean up after
	m_pProcThread = NULL;  				// itself before it dies
	DeleteResources();
	AfxEndThread(3); 
}

///////////////////////////////////////////////////////////////////////////
//   Called to delete any allocated resources in this class
///////////////////////////////////////////////////////////////////////////
void CIOCntrl::DeleteResources()
{
	m_ThreadActive = FALSE;
	m_IOMode = IOM_NORMAL;
	m_WaveDev.InClose();
	m_WaveDev.OutClose();
	m_SoundDev.InClose();
	m_SoundDev.OutClose();
}

//////////////////////////////////////////////////////////////////////////
// Simple minded error message display for various soundcard errors
// Should be put in string table.
//////////////////////////////////////////////////////////////////////////
void CIOCntrl::ProcessError( INT ErrorCode )
{
	switch( ErrorCode )
	{
		case NO_ERRORS:
			m_ErrorMessage = _T("");
			break;
		case MEMORY_ERROR:
		case MMSYSERR_NOMEM:
			m_ErrorMessage = _T("Memory Allocation error");
			ErrorCode = DLLERR_MEMORY;
			break;
		case SOUNDIN_ERR_NOTOPEN:
			m_ErrorMessage = _T("Tried to read and soundcard is not open");
			ErrorCode = DLLERR_IN_NOTOPEN;
			break;
		case SOUNDIN_ERR_OVERFLOW:
			m_ErrorMessage = _T("Input buffers overflowed");
			ErrorCode = DLLERR_IN_OVERFLOW;
			break;
		case SOUNDIN_ERR_TIMEOUT:
			m_ErrorMessage = _T("Timed out waiting for input buffers");
			ErrorCode = DLLERR_IN_TIMEOUT;
			break;
		case SOUNDOUT_ERR_NOTOPEN:
			m_ErrorMessage = _T("Tried to write and soundcard is not open");
			ErrorCode = DLLERR_OUT_NOTOPEN;
			break;
		case SOUNDOUT_ERR_UNDERFLOW:
			m_ErrorMessage = _T("Output buffers Underflowed");
			ErrorCode = DLLERR_OUT_UNDERFLOW;
			break;
		case SOUNDOUT_ERR_TIMEOUT:
			m_ErrorMessage = _T("Timed out waiting for output buffers");
			ErrorCode = DLLERR_OUT_TIMEOUT;
			break;

		case WAVERR_BADFORMAT:
			m_ErrorMessage = _T("Card doesn't support 16bit, 8000Hz, Mono format");
			ErrorCode = DLLERR_WAV_BADFORMAT;
			break;
		case WAVERR_STILLPLAYING:
			m_ErrorMessage = _T("still something playing");
			ErrorCode = DLLERR_WAV_STILLPLAYING;
			break;
		case WAVERR_UNPREPARED:
			m_ErrorMessage = _T("header not prepared ");
			ErrorCode = DLLERR_WAV_UNPREPARED;
			break;
		case WAVERR_SYNC:
			m_ErrorMessage = _T("device is synchronous");
			ErrorCode = DLLERR_WAV_SYNC;
			break;

		case WAVIN_ERR_OPEN:
			m_ErrorMessage = _T("Can't open wave file for input");
			ErrorCode = DLLERR_WAVIN_OPEN;
			break;
		case WAVIN_ERR_NOTWAVE:
			m_ErrorMessage = _T("File is not a RIFF wave type");
			ErrorCode = DLLERR_WAVIN_NOTWAVE;
			break;
		case WAVIN_ERR_INVALID:
			m_ErrorMessage = _T("Invalid wave file");
			ErrorCode = DLLERR_WAVIN_INVALID;
			break;
		case WAVIN_ERR_NODATA:
			m_ErrorMessage = _T("No data in file");
			ErrorCode = DLLERR_WAVIN_NODATA;
			break;
		case WAVIN_ERR_NOTSUPPORTED:
			m_ErrorMessage = _T("Not a supported data type");
			ErrorCode = DLLERR_WAVIN_NOTSUPPORTED;
			break;
		case WAVIN_ERR_READING:
			m_ErrorMessage = _T("Error reading data from file");
			ErrorCode = DLLERR_WAVIN_READING;
			break;
		case WAVIN_ERR_NOTOPEN:
			m_ErrorMessage = _T("Tried to read and file is not open");
			ErrorCode = DLLERR_WAVIN_NOTOPEN;
			break;

		case WAVOUT_ERR_OPEN:
			m_ErrorMessage = _T("Can't open wave file for output");
			ErrorCode = DLLERR_WAVOUT_OPEN;
			break;
		case WAVOUT_ERR_WRITING:
			m_ErrorMessage = _T("Error writing to wave file");
			ErrorCode = DLLERR_WAVOUT_WRITING;
			break;
		case WAVOUT_ERR_NOTOPEN:
			m_ErrorMessage = _T("Tried to write and file is not open");
			ErrorCode = DLLERR_WAVOUT_NOTOPEN;
			break;


		case MMSYSERR_BADDEVICEID:
			m_ErrorMessage = _T("Bad Device ID\nIs Soundcard Present?");
			ErrorCode = DLLERR_SYS_BADDEVICEID;
			break;
		case MMSYSERR_NOTENABLED:
			m_ErrorMessage = _T("Driver failed enable");
			ErrorCode = DLLERR_SYS_NOTENABLED;
			break;
		case MMSYSERR_ALLOCATED:
			m_ErrorMessage = _T("Device already allocated");
			ErrorCode = DLLERR_SYS_ALLOCATED;
			break;
		case MMSYSERR_INVALHANDLE:
			m_ErrorMessage = _T("Device handle is invalid");
			ErrorCode = DLLERR_SYS_INVALHANDLE;
			break;
		case MMSYSERR_NODRIVER:
			m_ErrorMessage = _T("No device driver present");
			ErrorCode = DLLERR_SYS_NODRIVER;
			break;
		case MMSYSERR_NOTSUPPORTED:
			m_ErrorMessage = _T("Function isn't supported");
			ErrorCode = DLLERR_SYS_NOTSUPPORTED;
			break;
		case MMSYSERR_BADERRNUM:
			m_ErrorMessage = _T("Error value out of range");
			ErrorCode = DLLERR_SYS_BADERRNUM;
			break;
		case MMSYSERR_INVALFLAG: 
			m_ErrorMessage = _T("Invalid flag passed");
			ErrorCode = DLLERR_SYS_INVALFLAG;
			break;
		case MMSYSERR_INVALPARAM: 
			m_ErrorMessage = _T("Invalid parameter passed");
			ErrorCode = DLLERR_SYS_INVALPARAM;
			break;
		case MMSYSERR_HANDLEBUSY:
			m_ErrorMessage = _T("Handle being used");
			ErrorCode = DLLERR_SYS_HANDLEBUSY;
			break;
		case MMSYSERR_NODRIVERCB:
			m_ErrorMessage = _T("driver does not call DriverCallback");
			ErrorCode = DLLERR_SYS_NODRIVERCB;
			break;
		case MMSYSERR_INVALIDALIAS:
		case MMSYSERR_BADDB:
		case MMSYSERR_KEYNOTFOUND:
		case MMSYSERR_READERROR:
		case MMSYSERR_WRITEERROR:
		case MMSYSERR_DELETEERROR:
		case MMSYSERR_VALNOTFOUND:
			m_ErrorMessage = _T("Registry error");
			ErrorCode = DLLERR_SYS_REGISTRY;
			break;
		default:
			m_ErrorMessage = _T("Unknown Error");
			ErrorCode = DLLERR_UNKNOWN;
			break;
	}
	m_DLLStatus = ErrorCode;
	if(	m_ThreadActive )
		StatusChange(ErrorCode);
}


void CIOCntrl::StatusChange(INT code)
{
	if( code != m_DLLStatus )
	{
		m_DLLStatus = code;
		::PostMessage( m_hWnd, MSG_STATUSCHANGE, code, 0);
	}
}

//////////////////////////////////////////////////////////////////
//  Controls COM port signals DTR and RTS for Xmitter on/off control
//////////////////////////////////////////////////////////////////
void CIOCntrl::PTTCntrl(BOOL Ptton)
{
char szPort[10];
	if( ( m_PTTPort == 0) || (m_ComPortMode == 0) )
		return;
	if( m_hComPort == INVALID_HANDLE_VALUE ) //need to open?
	{
		wsprintf( szPort, "COM%d", m_PTTPort );
		EnterCriticalSection(&m_CriticalSection);
		m_hComPort = CreateFile( szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL,
							OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
		LeaveCriticalSection(&m_CriticalSection);
		if( m_hComPort == INVALID_HANDLE_VALUE )
			return;
	}
	if( Ptton )
	{
		EnterCriticalSection(&m_CriticalSection);
		if(m_ComPortMode == 1)
		{
			EscapeCommFunction(m_hComPort, SETRTS);
			EscapeCommFunction(m_hComPort, CLRDTR);
		}
		else if(m_ComPortMode == 2)
		{
			EscapeCommFunction(m_hComPort, CLRRTS);
			EscapeCommFunction(m_hComPort, SETDTR);
		}
		else
		{
			EscapeCommFunction(m_hComPort, SETRTS);
			EscapeCommFunction(m_hComPort, SETDTR);
		}
		LeaveCriticalSection(&m_CriticalSection);
	}
	else
	{
		EnterCriticalSection(&m_CriticalSection);
		EscapeCommFunction(m_hComPort, CLRRTS);
		EscapeCommFunction(m_hComPort, CLRDTR);
		LeaveCriticalSection(&m_CriticalSection);
	}
}

//////////////////////////////////////////////////////////////////
// Adds n gaussian random doubles with 0 mean and std_dev = RMS = std
//    to the specified buffer
//////////////////////////////////////////////////////////////////
void CIOCntrl::AddGaussian(double * pData, INT n, double std)
{
#ifdef USE_NOISEGEN
INT i = 0;
double rad;
double r;
double u1;
double u2;
	while( i<n )
	{
// Generate two uniform random numbers between -1 and +1
// that are inside the unit circle
		do {
			u1 = 1.0 - 2.0 * (double)rand()/(double)RAND_MAX ;
			u2 = 1.0 - 2.0 * (double)rand()/(double)RAND_MAX ;
			r = u1*u1 + u2*u2;
		} while(r >= 1.0 || r == 0.0);
		rad = sqrt(-2.0*log(r)/r);
		pData[i++] += (std*u1*rad);
		pData[i++] += (std*u2*rad);
	}
#endif
}

