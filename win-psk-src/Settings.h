// Settings.h: interface for the CSettings class.
//
//////////////////////////////////////////////////////////////////////
//

#if !defined(AFX_SETTINGS_H__0A86FC41_C99E_11D2_88B9_444553540000__INCLUDED_)
#define AFX_SETTINGS_H__0A86FC41_C99E_11D2_88B9_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#define FFT_DISPVIEW 0			//TAB controls for signal view display selection
#define WATERFALL_DISPVIEW 1
#define INPUT_DISPVIEW 2
#define SYNC_DISPVIEW 3

#define BPSK_MODE 0				// used to specify which mode to use
#define QPSK_MODE 1
#define IQPSK_MODE 2
#define TUNE_MODE 3
#define TUNEID_MODE 4

#define PSK63MODE (1<<3)

#define PTT_NONE 0				//COM port selections

#define NUM_MACROSETS 4		//Number of MACRO Sets allowed
#define NUM_MACROKEYS 10	//Number of MACRO Keys

#define RX_CHAN 0


typedef struct
{
	INT xcur;
	INT zoom;
}StructDispSave;


class CSettings  
{
public:
	CSettings();
	virtual ~CSettings();
	BOOL WriteToRegistry();
	BOOL ReadFromRegistry();
//	User settings Variables that are saved
	BOOL m_fSlowAve;
	BOOL m_UseSlash;
	BOOL m_UseAFC;
	BOOL m_TXNet;
	BOOL m_UseLSB;
	BOOL m_EchoTXText;
	BOOL m_UseLocalTime;
	BOOL m_AutoTxtSel;
	BOOL m_AppendMode;
	BOOL m_TxOffsetMode;
	BOOL m_PSK63Mode;
	INT m_SQThreshold;
	INT m_ModType;
	INT m_ClockAdj;
	INT m_FreqMin;
	INT m_FreqMax;
	INT m_AFCLimit;
	INT m_SquelchSpeed;
	INT m_SoundcardNum;
	INT m_TabSel;
	INT m_RxFreq;
	INT m_TxFreq;
	INT m_MacroSet;
	INT m_InWaveOffset;
	INT m_IOMode;
	INT m_OutWaveTimeLimit;
	INT m_TxFreqOffset;
	UINT m_PTTPort;
	CString m_sMyCall;
	CString m_PathName;
	CString m_InPathName;
	CString m_OutPathName;
	CString m_PALPathName;
	CStringArray m_saMacroData;
	CStringArray m_saMacroTitle;
	LOGFONT m_lfRcvFont;
	LOGFONT m_lfXmitFont;
	LOGFONT m_lfXmitSentFont;
	COLORREF m_crRcvBackColor;
	COLORREF m_crXmitBackColor;
	COLORREF m_crRcvTextColor;
	COLORREF m_crXmitTextColor;
	COLORREF m_crXmitSentTextColor;
	StructDispSave m_PlotViewState[SYNC_DISPVIEW+1];
private:

};

#endif // !defined(AFX_SETTINGS_H__0A86FC41_C99E_11D2_88B9_444553540000__INCLUDED_)
