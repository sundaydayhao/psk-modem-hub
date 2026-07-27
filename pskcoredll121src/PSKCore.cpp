//////////////////////////////////////////////////////////////////////
// PSKCore.cpp : Defines the initialization routines for the DLL.
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


#include "stdafx.h"
#include "PSKCore.h"
#include "iocntrl.h"
#include "perform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern double DebugFP1;
extern double DebugFP2;
extern INT DebugINT;


#ifdef DLL_VERSION	//(dec) 11-Mar-2001
/////////////////////////////////////////////////////////////////////////////
// CPSKCoreApp

BEGIN_MESSAGE_MAP(CPSKCoreApp, CWinApp)
	//{{AFX_MSG_MAP(CPSKCoreApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPSKCoreApp construction

CPSKCoreApp::CPSKCoreApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}
#endif

CIOCntrl* pCIOCntrl = NULL;
/////////////////////////////////////////////////////////////////
// Control functions
/////////////////////////////////////////////////////////////////
long COREDLL_API __stdcall fnStartSoundCard(HWND h_Wnd, long cardnum,
													long numRXchannels)
{
long error = 0;
	if(pCIOCntrl)
	{
		pCIOCntrl->m_InputCardNum = cardnum;
		pCIOCntrl->m_OutputCardNum = cardnum;  //AA6YQ 1.20
		error =  pCIOCntrl->StartIO(h_Wnd, (INT)numRXchannels, 0);
	}
	return error;
}

long COREDLL_API __stdcall fnStartSoundCardEx(HWND h_Wnd, long cardnum,
													long numRXchannels, long IOMode)
{
long error = 0;
	if(pCIOCntrl)
	{
		pCIOCntrl->m_InputCardNum = cardnum;
		pCIOCntrl->m_OutputCardNum = cardnum;  //AA6YQ 1.20
		error =  pCIOCntrl->StartIO(h_Wnd, (INT)numRXchannels,IOMode);
	}
	return error;
}


//added by AA6YQ 1.20
long COREDLL_API __stdcall fnStartRXTXSoundCard(HWND h_Wnd, long RXcardnum,long TXcardnum,
													long numRXchannels)
{
long error = 0;
	if(pCIOCntrl)
	{
		pCIOCntrl->m_InputCardNum = RXcardnum;
		pCIOCntrl->m_OutputCardNum = TXcardnum;  
		error =  pCIOCntrl->StartIO(h_Wnd, (INT)numRXchannels, 0);
	}
	return error;
}

void COREDLL_API __stdcall fnStopSoundCard(void)
{
	if(pCIOCntrl)
		pCIOCntrl->StopIO();
}

/////////////////////////////////////////////////////////////////
// RX Functions
/////////////////////////////////////////////////////////////////
void COREDLL_API __stdcall fnSetRXFrequency(long freq,
										long CaptureRange, long chan)
{
	if(freq>3500)
		freq = 3500;
	if(freq<100)
		freq = 100;
	if(pCIOCntrl)
		if( pCIOCntrl->ChkDetExists(chan) )
			pCIOCntrl->SetRXFrequency( freq, CaptureRange, chan );
}

long COREDLL_API __stdcall fnEnableRXChannel(long chan, long enable)
{
long error = DLLERR_MEMORY;
	if(pCIOCntrl)
	{
		error = pCIOCntrl->EnableRXChannel( chan, enable );
	}
	return error;
}

long COREDLL_API __stdcall fnIsRXChannelActive(long chan)
{
BOOL ret = FALSE;
	if(pCIOCntrl)
	{
		ret = pCIOCntrl->IsRxChannelActive(chan);
	}
	return ret;
}

long COREDLL_API __stdcall fnGetNumActiveRXChannels()
{
int ret = 0;
	if(pCIOCntrl)
	{
		ret = pCIOCntrl->GetNumActiveRXChannels();
	}
	return ret;
}

void COREDLL_API __stdcall fnSetRXPSKMode (long mode, long chan)
{
	if(pCIOCntrl)
		if( pCIOCntrl->ChkDetExists(chan) )
			pCIOCntrl->m_pPSKDet[chan]->SetRXPSKMode(mode&0x001F);  //AA6YQ 1.21 need 5 bits of mode
}

long COREDLL_API __stdcall fnGetRXFrequency(long chan)
{
INT f = 1000;
	if(pCIOCntrl)
		if( pCIOCntrl->IsRxChannelActive(chan) )
			f = pCIOCntrl->m_pPSKDet[chan]->GetRXFrequency();
	if(f>3500)
		f = 3500;
	if(f<100)
		f = 100;
	return f;
}

void COREDLL_API __stdcall fnSetFFTMode(long ave, long maxscale, long type)
{
	if(ave>10)
		ave = 10;
	if(ave<1)
		ave = 1;
	if(type>99)
		type = 99;
	if(type<0)
		type = 0;
	if(pCIOCntrl)
		pCIOCntrl->SetFFTParams(  ave,( double)maxscale*0.10, type );
}

long COREDLL_API __stdcall fnGetFFTData(long* array, long start, long end)
{
long result = 0;
	if(start>1022)
		start = 1022;
	if(start<0)
		start = 0;
	if(end>1023)
		end = 1023;
	if(end<1)
		end = 1;
	if(pCIOCntrl)
		result = pCIOCntrl->GetFFTData(array, start, end);
	return result;
}

long COREDLL_API __stdcall fnGetClosestPeak(long Startf, long Stopf)
{
LONG freq = 1500;
	if(pCIOCntrl)
		freq = pCIOCntrl->m_Cfft.GetClosestPeak( Startf, Stopf);
	return freq;
}

void COREDLL_API __stdcall fnGetVectorData(long* vectorarray, long chan)
{
	if(pCIOCntrl)
	{
		if( pCIOCntrl->IsRxChannelActive(chan) )
		{
			if(pCIOCntrl->m_RXState || pCIOCntrl->m_SatMode)
			{
				if(pCIOCntrl->m_pPSKDet[chan])
					pCIOCntrl->m_pPSKDet[chan]->GetVectorData(vectorarray);
			}
			else
			{
				if(pCIOCntrl )
					pCIOCntrl->m_PSKMod.GetVectorData(vectorarray);
			}
		}
		else
		{
			for(INT i=0; i<16; i++)
				vectorarray[i] = 0;
		}
	}
}

void COREDLL_API __stdcall fnGetSyncData(long* syncarray, long chan)
{
	if(pCIOCntrl)
		if( pCIOCntrl->IsRxChannelActive(chan) )
		{
			pCIOCntrl->m_pPSKDet[chan]->GetSyncData(syncarray);
		}
		else
		{
			for(INT i=0; i<16; i++)
				syncarray[i] = 0;
		}
}

long COREDLL_API __stdcall fnGetRawData(long* array, long start, long end)
{
LONG result = 0;
	if(start < end)
	{
		if(start>2046)
			start = 2046;
		if(start<0)
			start = 0;
		if(end>2047)
			end = 2047;
		if(end<1)
			end = 1;
		if(pCIOCntrl)
			result = pCIOCntrl->GetRawData(array, start, end);
	}
	return result;
}

void COREDLL_API __stdcall fnSetAFCLimit(long limit, long chan)
{
	if(pCIOCntrl)
		if( pCIOCntrl->ChkDetExists(chan) && (limit>=0) && (limit<=3500) )
			pCIOCntrl->m_pPSKDet[chan]->SetAFCLimit(limit);
}

void COREDLL_API __stdcall fnSetSquelchThreshold(long thresh, long mode, long chan)
{
	if(thresh>99)
		thresh = 99;
	if(thresh<0)
		thresh = 0;
	if(mode>200)
		mode = 200;
	if(mode<0)
		mode = 0;
	if( (mode>1) && (mode<10) )
		mode = 10;
	if(pCIOCntrl)
		if( pCIOCntrl->ChkDetExists(chan) )
			pCIOCntrl->m_pPSKDet[chan]->SetSquelchThresh(thresh,mode);
}

long COREDLL_API __stdcall fnGetSignalLevel(long chan)
{
LONG level = 0;
	if(pCIOCntrl)
		if( pCIOCntrl->IsRxChannelActive(chan) )
			level = pCIOCntrl->m_pPSKDet[chan]->GetSignalLevel();
	if(level>99)
		level = 99;
	if(level<0)
		level = 0;
	return level;
}


/////////////////////////////////////////////////////////////////
// TX Functions
/////////////////////////////////////////////////////////////////
void COREDLL_API __stdcall fnStartTX(long mode)
{
	if(pCIOCntrl)
	{
		if(mode & (1<<7))
		{
			mode &= ~(1<<7);
			pCIOCntrl->m_SatMode = TRUE;
		}
		else
		{
			pCIOCntrl->m_SatMode = FALSE;
		}
		if(mode>18)	//AA6YQ 1.21
			mode = 18;
		if(mode<0)
			mode = 0;
		if(pCIOCntrl)
		{
			pCIOCntrl->m_PSKMod.SetTXMode(mode);
			pCIOCntrl->StartTX();
		}
	}
}

void COREDLL_API __stdcall fnStopTX()
{
	if(pCIOCntrl)
		pCIOCntrl->StopTX();
}

void COREDLL_API __stdcall fnAbortTX()
{
	if(pCIOCntrl)
		pCIOCntrl->AbortTX();
}

void COREDLL_API __stdcall fnSetTXFrequency(long freq)
{
	if(freq>3500)
		freq = 3500;
	if(freq<100)
		freq = 100;
	if(pCIOCntrl)
		pCIOCntrl->m_PSKMod.SetTXFreq(freq);
}

void COREDLL_API __stdcall fnSetCWIDString(char* lpszIDstrg)
{
	if(pCIOCntrl)
		pCIOCntrl->m_PSKMod.SetTXCWID(lpszIDstrg);
}

long COREDLL_API __stdcall fnSendTXCharacter( long txchar, bool cntrl)
{
LONG count = 0;
	if(pCIOCntrl)
	{
		pCIOCntrl->m_PSKMod.PutTxQue(txchar, cntrl);
		count = pCIOCntrl->m_PSKMod.GetTXCharsRemaining();
	}
	return count;
}

long COREDLL_API __stdcall fnSendTXString(char* lpszTXStrg)
{
LONG ret = 0;
INT count = 0;
char ch;
	if(pCIOCntrl)
	{
		while( ((ch = lpszTXStrg[count++]) != 0 ) && (count < TX_BUF_SIZE) )
		{
			pCIOCntrl->m_PSKMod.PutTxQue(ch, FALSE);
		}
		ret = pCIOCntrl->m_PSKMod.GetTXCharsRemaining();
	}
	return ret;
}

long COREDLL_API __stdcall fnGetTXCharsRemaining()
{
LONG count = 0;
	if(pCIOCntrl)
		count = pCIOCntrl->m_PSKMod.GetTXCharsRemaining();
	return count;
}

void COREDLL_API __stdcall fnClearTXBuffer()
{
	if(pCIOCntrl)
		pCIOCntrl->m_PSKMod.ClrQue();
}


void COREDLL_API __stdcall fnSetCWIDSpeed (long speed )
{
	if(speed>4)
		speed = 4;
	if(speed<1)
		speed = 1;
	if(pCIOCntrl)
		pCIOCntrl->m_PSKMod.SetCWIDSpeed(speed);
}

long COREDLL_API __stdcall fnSetComPort (long portnum, long mode )
{
LONG ret = 0;
	if( portnum > 8 )
		portnum = 8;	//clamp number of ports 0 to 8
	if( portnum < 0 )
		portnum = 0;
	if(pCIOCntrl)
		ret = pCIOCntrl->SetComPort(portnum, mode);
	return ret;
}

/////////////////////////////////////////////////////////////////
//  MISC Functions
/////////////////////////////////////////////////////////////////
void COREDLL_API __stdcall fnSetClockErrorAdjustment (long ppm)
{
	if(ppm>20000)
		ppm = 20000;
	if(ppm<-20000)
		ppm = -20000;
	if(pCIOCntrl)
		pCIOCntrl->SetClockErrorAdjustment(-ppm);
}

long COREDLL_API __stdcall fnGetDLLVersion ()
{	
long version = 0;
	if(pCIOCntrl)
		version = pCIOCntrl->m_DLLRevision;
	return version;
}

void COREDLL_API __stdcall fnGetErrorString(char* lpszError)
{
CString err = "";
	if(pCIOCntrl)
		pCIOCntrl->GetErrorMsg(err);
	strcpy( lpszError, (LPCTSTR)err);
}

long COREDLL_API __stdcall fnSetInputWavePath( char* sPath, 
								  long* pLengthTime, long Offset)
{
long error = DLLERR_UNKNOWN;
	if(pCIOCntrl)
		error = pCIOCntrl->SetInputWavePath( sPath, (PINT)pLengthTime, (INT)Offset);
	return error;
}

long COREDLL_API __stdcall fnSetOutputWavePath( char* sPath, long TimeLimit, long Append)
{
long error = DLLERR_UNKNOWN;
	if(pCIOCntrl)
		error = pCIOCntrl->SetOutputWavePath( sPath, (INT)TimeLimit, (BOOL)Append);
	return error;
}

long COREDLL_API __stdcall fnGetDebugData(double* val1, double* val2)
{
	*val1 = DebugFP1;
	*val2 = DebugFP2;
	return DebugINT;
}

/////////////////////////////////////////////////////////////////
//  Functions added after the initial release of the dll
/////////////////////////////////////////////////////////////////
void COREDLL_API __stdcall fnRewindInput (long Blocks )
{
	if(Blocks>99)
		Blocks = 99;
	if(Blocks<1)
		Blocks = 1;
	if(pCIOCntrl)
		pCIOCntrl->RewindInput( (INT)Blocks );
}



/////////////////////////////////////////////////////////////////////////////
// The one and only CPSKCoreApp object

#ifdef DLL_VERSION	//(dec) 11-Mar-2001
CPSKCoreApp theApp;

BOOL CPSKCoreApp::InitInstance() 
{
	pCIOCntrl = new CIOCntrl();
	return ( CWinApp::InitInstance() );
}

int CPSKCoreApp::ExitInstance() 
{
	if(pCIOCntrl)
	{
		pCIOCntrl->StopIO();
		delete pCIOCntrl;
		pCIOCntrl = NULL;
	}

	return CWinApp::ExitInstance();
}
#else
//(dec) 11-Mar-01 Created Init and Term functions to replace DLL Init and Exit instance f/n's.
BOOL COREDLL_API __stdcall fnInitPSKLib()
{
	BOOL bRet = TRUE;
	if ( pCIOCntrl == NULL )
	{
		pCIOCntrl = new CIOCntrl();
		if ( pCIOCntrl == NULL )
			return FALSE;
	}
	return bRet;
}

void COREDLL_API __stdcall fnTermPSKLib()
{
	if ( pCIOCntrl )
	{
		pCIOCntrl->StopIO();
		delete pCIOCntrl;
		pCIOCntrl = NULL;
	}
}
#endif
