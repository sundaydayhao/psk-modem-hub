#if !defined(PSKCOREXT_H__F8820958_EC65_11D3_B4E7_903B50C10000__INCLUDED_)
#define PSKCOREXT_H__F8820958_EC65_11D3_B4E7_903B50C10000__INCLUDED_
//////////////////////////////////////////////////////////////////
//              P S K C O R E I M P L I C I T . H
//////////////////////////////////////////////////////////////////
//In order to link the pskcore dll implicitly, make sure the file
// pskcore.lib is also included in the linker list and the file 
//  itself is in the project space.
//  The functions can then be called directly as defined above.
//////////////////////////////////////////////////////////////////


#ifdef DLL_VERSION	//(dec) 11-Mar-2001
#ifdef COREDLL_EXPORTS
#define COREDLL_API __declspec(dllexport)
#else
#define COREDLL_API __declspec(dllimport)
#endif
#else
#define COREDLL_API	//(dec) 11-Mar-2001 not needed for static link version
#endif

//===========================================================================
/////////////////   DLL Exposed Functions   /////////////////////////////////
//===========================================================================

#ifndef DLL_VERSION	//(dec) 11-Mar-2001
//  Initialization/Shutdown Functions
extern "C" BOOL COREDLL_API __stdcall fnInitPSKLib(void);
extern "C" void COREDLL_API __stdcall fnTermPSKLib(void);
#endif

//  Initialization/Shutdown Functions
extern "C" long COREDLL_API __stdcall fnStartSoundCard(HWND h_Wnd, long cardnum, long numRXchannels);
extern "C" void COREDLL_API __stdcall fnStopSoundCard(void);

//  RX Functions
extern "C" void COREDLL_API __stdcall fnSetRXFrequency(long freq, long CaptureRange, long chan);
extern "C" void COREDLL_API __stdcall fnSetRXPSKMode (long mode, long chan);
extern "C" long COREDLL_API __stdcall fnGetRXFrequency(long chan);
extern "C" void COREDLL_API __stdcall fnSetFFTMode(long ave, long maxscale, long log);
extern "C" long COREDLL_API __stdcall fnGetFFTData(long* datarray, long start, long end);
extern "C" long COREDLL_API __stdcall fnGetClosestPeak( long Startf, long Stopf);
extern "C" void COREDLL_API __stdcall fnGetVectorData(long* vectorarray, long chan);
extern "C" void COREDLL_API __stdcall fnGetSyncData(long* syncarray, long chan);
extern "C" long COREDLL_API __stdcall fnGetRawData(long* array, long start, long end);
extern "C" void COREDLL_API __stdcall fnSetAFCLimit(long limit, long chan);
extern "C" void COREDLL_API __stdcall fnSetSquelchThreshold(long thresh, long mode, long chan);
extern "C" long COREDLL_API __stdcall fnGetSignalLevel(long chan);
//  TX Functions
extern "C" void COREDLL_API __stdcall fnStartTX(long mode);
extern "C" void COREDLL_API __stdcall fnStopTX();
extern "C" void COREDLL_API __stdcall fnAbortTX();
extern "C" void COREDLL_API __stdcall fnSetTXFrequency(long freq);
extern "C" void COREDLL_API __stdcall fnSetCWIDString(char* lpszIDstrg);
extern "C" long COREDLL_API __stdcall fnSendTXCharacter( long txchar, bool cntrl);
extern "C" long COREDLL_API __stdcall fnSendTXString(char* lpszTXStrg);
extern "C" long COREDLL_API __stdcall fnGetTXCharsRemaining();
extern "C" void COREDLL_API __stdcall fnClearTXBuffer();
extern "C" void COREDLL_API __stdcall fnSetCWIDSpeed (long speed );
extern "C" long COREDLL_API __stdcall fnSetComPort (long portnum, long mode );

//  MISC Functions
extern "C" void COREDLL_API __stdcall fnSetClockErrorAdjustment (long ppm);
extern "C" long COREDLL_API __stdcall fnGetDLLVersion ();
extern "C" void COREDLL_API __stdcall fnGetErrorString(char* lpszError);
extern "C" long COREDLL_API __stdcall fnGetDebugData(double* val1, double* val2);

//  New Functions added after the initial release
extern "C" void COREDLL_API __stdcall fnRewindInput (long Blocks );
extern "C" long COREDLL_API __stdcall fnSetInputWavePath(char* path, long* pLengthTime, long Offset);
extern "C" long COREDLL_API __stdcall fnSetOutputWavePath(char* path, long TimeLimit, long Append);
extern "C" long COREDLL_API __stdcall fnStartSoundCardEx(HWND h_Wnd, long cardnum, long numRXchannels, long IOMode);
extern "C" long COREDLL_API __stdcall fnEnableRXChannel(long chan, long enable);
extern "C" long COREDLL_API __stdcall fnIsRXChannelActive(long chan);


/////////////////////////////////////////////////////////////////
//   Windows USER Message defines   
/////////////////////////////////////////////////////////////////
//#define WM_USER 0x400
#define MSG_DATARDY (WM_USER+1000)
#define MSG_PSKCHARRDY (WM_USER+1001)
#define MSG_STATUSCHANGE (WM_USER+1002)
#define MSG_IMDRDY (WM_USER+1003)
#define MSG_CLKERROR (WM_USER+1004)


#endif // !defined(PSKCOREXT_H__F8820958_EC65_11D3_B4E7_903B50C10000__INCLUDED_)
