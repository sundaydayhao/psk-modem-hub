#if !defined(PSKCOREDEFS_H__F8820958_EC65_11D3_B4E7_903B50C10000__INCLUDED_)
#define PSKCOREDEFS_H__F8820958_EC65_11D3_B4E7_903B50C10000__INCLUDED_


/////////////////////////////////////////////////////////////////
// Control functions
/////////////////////////////////////////////////////////////////
typedef long (__stdcall FNSTARTSOUNDCARD)(HWND h_Wnd, long cardnum, long numRXchannels);
typedef void (__stdcall FNSTOPSOUNDCARD)();

/////////////////////////////////////////////////////////////////
// RX Functions
/////////////////////////////////////////////////////////////////
typedef void (__stdcall FNSETRXFREQUENCY)(long freq, long capturerange, long chan);
typedef void (__stdcall FNSETRXPSKMODE)(long mode, long chan);
typedef long (__stdcall FNGETRXFREQUENCY)(long chan);
typedef void (__stdcall FNSETFFTMODE)(long ave, long maxscale, long log);
typedef long (__stdcall FNGETFFTDATA)(long* data, long start, long end);
typedef long (__stdcall FNGETCLOSESTPEAK)(long Startf, long Stopf);
typedef long (__stdcall FNGETRAWDATA)(long* data, long start, long end);
typedef long (__stdcall FNGETVECTORDATA)(long* data, long chan);
typedef void (__stdcall FNGETSYNCDATA)(long* syncarray, long chan);
typedef void (__stdcall FNSETAFCLIMIT)(long limit, long chan);
typedef void (__stdcall FNSETSQUELCHTHRESHOLD)(long thresh, long mode, long chan);
typedef long (__stdcall FNGETSIGNALLEVEL)(long chan);

/////////////////////////////////////////////////////////////////
// TX Functions
/////////////////////////////////////////////////////////////////
typedef void (__stdcall FNSTARTTX)(long mode);
typedef void (__stdcall FNSTOPTX)();
typedef void (__stdcall FNABORTTX)();
typedef void (__stdcall FNSETTXFREQUENCY)(long freq);
typedef void (__stdcall FNSETCWIDSTRING)( char* lpszIDStrng);
typedef long (__stdcall FNSENDTXCHARACTER)( long txchar, bool cntrl);
typedef long (__stdcall FNSENDTXSTRING)( char* lpszTXStrng);
typedef long (__stdcall FNGETTXCHARSREMAINING)( );
typedef void (__stdcall FNCLEARTXBUFFER)( );
typedef void (__stdcall FNSETCWIDSPEED)(long speed);
typedef long (__stdcall FNSETCOMPORT)(long portnum,long mode);

/////////////////////////////////////////////////////////////////
//  MISC Functions
/////////////////////////////////////////////////////////////////
typedef long (__stdcall FNGETDLLVERSION)();
typedef void (__stdcall FNGETERRORSTRING)(char* lpszError);
typedef long (__stdcall FNSETCLOCKERRORADJUSTMENT)(long ppm);
typedef long (__stdcall FNGETDEBUGDATA)(double* val1, double* val2);

/////////////////////////////////////////////////////////////////
//  Functions added after the initial release
/////////////////////////////////////////////////////////////////
typedef void (__stdcall FNREWINDINPUT)(long Blocks);


/////////////////////////////////////////////////////////////////
//   Windows USER Message defines   
/////////////////////////////////////////////////////////////////
//#define WM_USER 0x400
#define MSG_DATARDY (WM_USER+1000)
#define MSG_PSKCHARRDY (WM_USER+1001)
#define MSG_STATUSCHANGE (WM_USER+1002)
#define MSG_IMDRDY (WM_USER+1003)
#define MSG_CLKERROR (WM_USER+1004)

/////////////////////////////////////////////////////////////////
//  example VC++ use of dll function
/////////////////////////////////////////////////////////////////
//HINSTANCE m_hModule;
//FNSTARTSOUNDCARD* m_pfnStartSoundCard;
//FNGETRXFREQUENCY* m_pfnGetRXFrequency;
//FNSTOPSOUNDCARD* m_pfnStopSoundCard;
//
//VERIFY(m_hModule = ::LoadLibrary("PskCore.dll"));//look in same folder or windows system
//
//VERIFY( m_pfnStartSoundCard = (FNSTARTSOUNDCARD*)GetProcAddress(m_hModule,"fnStartSoundCard"));
//
//long error = m_pfnStartSoundCard( m_hWnd, -1, NUM_RXCHANNELS );
//
//VERIFY( m_pfnGetRXFrequency = (FNGETRXFREQUENCY*)GetProcAddress(m_hModule,"fnGetRXFrequency"));
//
//long RXfreq = m_pfnGetRXFrequency( chan );
//
//VERIFY( m_pfnStopSoundCard = (FNSTOPSOUNDCARD*)GetProcAddress(m_hModule,"fnStopSoundCard"));
//
//m_pfnStopSoundCard();		//Stop the SoundCard
//
//FreeLibrary(m_hModule);	//unload dll
//
//////////////////////////////////////////////////////////////////

#endif // !defined(PSKCOREDEFS_H__F8820958_EC65_11D3_B4E7_903B50C10000__INCLUDED_)
