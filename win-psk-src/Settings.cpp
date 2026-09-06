// Settings.cpp: implementation of the CSettings class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "WinPSK.h"
#include "Settings.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSettings::CSettings()
{

}

CSettings::~CSettings()
{

}


BOOL CSettings::ReadFromRegistry()
{
CWinApp* pApp = AfxGetApp();
CString str;
INT cRead;
INT x1,x2,x3,x4,x5,x6,x7,x8;
BOOL bSpare;
INT iSpare;
	str = pApp->GetProfileString ("Settings\\BOOLs", "BOOLs");
	if(str.IsEmpty ())
		return FALSE;
// Get BOOLs
	cRead = _stscanf (str, "%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i",
		&m_UseAFC,
		&m_UseSlash,
		&m_fSlowAve,
		&m_TXNet,
		&m_UseLSB,
		&m_EchoTXText,
		&m_AutoTxtSel,
		&m_UseLocalTime,
		&m_AppendMode,
		&m_TxOffsetMode,
		&m_PSK63Mode,
		&bSpare,
		&bSpare,
		&bSpare,
		&bSpare
		);
	if(cRead != 15)
		return FALSE;
//Get INT's
	str = pApp->GetProfileString ("Settings\\INTs", "INTs");
	if(str.IsEmpty ())
		return FALSE;
	cRead = _stscanf(str,"%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i",
		&m_SQThreshold,
		&m_TabSel,
		&m_RxFreq,
		&m_TxFreq,
		&m_ModType,
		&m_PTTPort,
		&m_PlotViewState[FFT_DISPVIEW].xcur,
		&m_PlotViewState[FFT_DISPVIEW].zoom,
		&m_PlotViewState[WATERFALL_DISPVIEW].xcur,
		&m_PlotViewState[WATERFALL_DISPVIEW].zoom,
		&m_ClockAdj,
		&m_MacroSet,
		&m_FreqMin,
		&m_FreqMax,
		&m_AFCLimit,
		&m_SquelchSpeed,
		&m_SoundcardNum,
		&m_InWaveOffset,
		&m_IOMode,
		&m_OutWaveTimeLimit,
		&m_TxFreqOffset,
		&iSpare,
		&iSpare,
		&iSpare,
		&iSpare,
		&iSpare
		);
	if(cRead != 26)
		return FALSE;
//Get colors
	str = pApp->GetProfileString ("Settings\\Colors", "Colors");
	if(str.IsEmpty ())
		return FALSE;
	cRead = _stscanf( str, "%i:%i:%i:%i:%i",
		&m_crRcvBackColor,
		&m_crXmitBackColor,
		&m_crRcvTextColor,
		&m_crXmitTextColor,
		&m_crXmitSentTextColor);
	if(cRead != 5)
		return FALSE;

//get fonts
	str = pApp->GetProfileString("Settings\\Fonts", "RcvName");
	if(str.IsEmpty ())
		return FALSE;
	_tcscpy(m_lfRcvFont.lfFaceName,str);
	str = pApp->GetProfileString("Settings\\Fonts", "Rcv");
	if(str.IsEmpty ())
		return FALSE;
	cRead = _stscanf( str, "%i:%i:%d:%i:%i:%i:%i:%i:%i:%i",
			&m_lfRcvFont.lfWeight, 
			&m_lfRcvFont.lfHeight, 
			&x1,&x2,&x3,&x4,&x5,&x6,&x7,&x8);
	if(cRead != 10)
		return FALSE;
	m_lfRcvFont.lfOutPrecision = x1;
	m_lfRcvFont.lfClipPrecision = x2;
	m_lfRcvFont.lfQuality = x3;
	m_lfRcvFont.lfItalic = x4;
	m_lfRcvFont.lfUnderline = x5;
	m_lfRcvFont.lfStrikeOut = x6;
	m_lfRcvFont.lfCharSet = x7;
	m_lfRcvFont.lfPitchAndFamily = x8;

	str = pApp->GetProfileString("Settings\\Fonts", "XmitName");
	if(str.IsEmpty ())
		return FALSE;
	_tcscpy(m_lfXmitFont.lfFaceName,str);
	str = pApp->GetProfileString("Settings\\Fonts", "Xmit");
	if(str.IsEmpty ())
		return FALSE;
	cRead = _stscanf( str, "%i:%i:%d:%i:%i:%i:%i:%i:%i:%i",
			&m_lfXmitFont.lfWeight, 
			&m_lfXmitFont.lfHeight, 
			&x1,&x2,&x3,&x4,&x5,&x6,&x7,&x8);
	if(cRead != 10)
		return FALSE;
	m_lfXmitFont.lfOutPrecision = x1;
	m_lfXmitFont.lfClipPrecision = x2;
	m_lfXmitFont.lfQuality = x3;
	m_lfXmitFont.lfItalic = x4;
	m_lfXmitFont.lfUnderline = x5;
	m_lfXmitFont.lfStrikeOut = x6;
	m_lfXmitFont.lfCharSet = x7;
	m_lfXmitFont.lfPitchAndFamily = x8;

	str = pApp->GetProfileString("Settings\\Fonts", "SentName");
	if(str.IsEmpty ())
		return FALSE;
	_tcscpy(m_lfXmitSentFont.lfFaceName,str);
	str = pApp->GetProfileString("Settings\\Fonts", "Sent");
	if(str.IsEmpty ())
		return FALSE;
	cRead = _stscanf( str, "%i:%i:%d:%i:%i:%i:%i:%i:%i:%i",
			&m_lfXmitSentFont.lfWeight, 
			&m_lfXmitSentFont.lfHeight, 
			&x1,&x2,&x3,&x4,&x5,&x6,&x7,&x8);
	if(cRead != 10)
		return FALSE;
	m_lfXmitSentFont.lfOutPrecision = x1;
	m_lfXmitSentFont.lfClipPrecision = x2;
	m_lfXmitSentFont.lfQuality = x3;
	m_lfXmitSentFont.lfItalic = x4;
	m_lfXmitSentFont.lfUnderline = x5;
	m_lfXmitSentFont.lfStrikeOut = x6;
	m_lfXmitSentFont.lfCharSet = x7;
	m_lfXmitSentFont.lfPitchAndFamily = x8;
//
CString str2;
	for( INT i=0; i<NUM_MACROSETS; i++)
	{
		for( INT j=0; j<NUM_MACROKEYS; j++)
		{
			str.Format("Title[%d][%d]", i,j);
			str2 = pApp->GetProfileString("Settings\\Macros", str,_T("Undefined"));
			m_saMacroTitle.InsertAt(i*NUM_MACROKEYS + j, str2);
			str.Format("Macro[%d][%d]", i,j);
			str2 = pApp->GetProfileString("Settings\\Macros", str, _T(" "));
			m_saMacroData.InsertAt(i*NUM_MACROKEYS + j, str2);
		}
	}
//get other strings
	m_sMyCall = pApp->GetProfileString("Settings\\Strings", "MyCall",_T("No Call Set"));
	if(m_sMyCall.IsEmpty ())
		return FALSE;
	m_PathName = pApp->GetProfileString("Settings\\Strings", "FilePath",_T("c:\\Textfile.txt"));
	if(m_PathName.IsEmpty ())
		return FALSE;
	m_InPathName = pApp->GetProfileString("Settings\\Strings", "InFilePath",_T(""));
	m_OutPathName = pApp->GetProfileString("Settings\\Strings", "OutFilePath",_T(""));
	m_PALPathName = pApp->GetProfileString("Settings\\Strings", "PalettePath",_T(""));
	return TRUE;
}

BOOL CSettings::WriteToRegistry()
{
CWinApp* pApp = AfxGetApp();
CString str;
BOOL bSpare = 0;
INT iSpare = 0;
//save BOOL's
	str.Format("%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i",
		m_UseAFC,
		m_UseSlash,
		m_fSlowAve,
		m_TXNet,
		m_UseLSB,
		m_EchoTXText,
		m_AutoTxtSel,
		m_UseLocalTime,
		m_AppendMode,
		m_TxOffsetMode,
		m_PSK63Mode,
		bSpare,
		bSpare,
		bSpare,
		bSpare
		);
	if(!pApp->WriteProfileString("Settings\\BOOLs", "BOOLs", str))
		return FALSE;
//save INT's
	str.Format("%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i",
		m_SQThreshold,
		m_TabSel,
		m_RxFreq,
		m_TxFreq,
		m_ModType,
		m_PTTPort,
		m_PlotViewState[FFT_DISPVIEW].xcur,
		m_PlotViewState[FFT_DISPVIEW].zoom,
		m_PlotViewState[WATERFALL_DISPVIEW].xcur,
		m_PlotViewState[WATERFALL_DISPVIEW].zoom,
		m_ClockAdj,
		m_MacroSet,
		m_FreqMin,
		m_FreqMax,
		m_AFCLimit,
		m_SquelchSpeed,
		m_SoundcardNum,
		m_InWaveOffset,
		m_IOMode,
		m_OutWaveTimeLimit,
		m_TxFreqOffset,
		iSpare,
		iSpare,
		iSpare,
		iSpare,
		iSpare
		);
	if(!pApp->WriteProfileString("Settings\\INTs", "INTs", str))
		return FALSE;
// save colors	
	str.Format("%i:%i:%i:%i:%i",
		m_crRcvBackColor,
		m_crXmitBackColor,
		m_crRcvTextColor,
		m_crXmitTextColor,
		m_crXmitSentTextColor);
	if(!pApp->WriteProfileString("Settings\\Colors", "Colors", str))
		return FALSE;
//save fonts
	str.Format("%li:%li:%i:%i:%i:%i:%i:%i:%i:%i",
		m_lfRcvFont.lfWeight,
		m_lfRcvFont.lfHeight,
		m_lfRcvFont.lfOutPrecision,
		m_lfRcvFont.lfClipPrecision,
		m_lfRcvFont.lfQuality,
		m_lfRcvFont.lfItalic,
		m_lfRcvFont.lfUnderline,
		m_lfRcvFont.lfStrikeOut,
		m_lfRcvFont.lfCharSet,
		m_lfRcvFont.lfPitchAndFamily);
	if(!pApp->WriteProfileString("Settings\\Fonts", "RcvName", m_lfRcvFont.lfFaceName))
		return FALSE;
	if(!pApp->WriteProfileString("Settings\\Fonts", "Rcv", str))
		return FALSE;

	str.Format("%li:%li:%i:%i:%i:%i:%i:%i:%i:%i",
		m_lfXmitFont.lfWeight,
		m_lfXmitFont.lfHeight,
		m_lfXmitFont.lfOutPrecision,
		m_lfXmitFont.lfClipPrecision,
		m_lfXmitFont.lfQuality,
		m_lfXmitFont.lfItalic,
		m_lfXmitFont.lfUnderline,
		m_lfXmitFont.lfStrikeOut,
		m_lfXmitFont.lfCharSet,
		m_lfXmitFont.lfPitchAndFamily);
	if(!pApp->WriteProfileString("Settings\\Fonts", "XmitName", m_lfXmitFont.lfFaceName))
		return FALSE;
	if(!pApp->WriteProfileString("Settings\\Fonts", "Xmit", str))
		return FALSE;

	str.Format("%li:%li:%i:%i:%i:%i:%i:%i:%i:%i",
		m_lfXmitSentFont.lfWeight,
		m_lfXmitSentFont.lfHeight,
		m_lfXmitSentFont.lfOutPrecision,
		m_lfXmitSentFont.lfClipPrecision,
		m_lfXmitSentFont.lfQuality,
		m_lfXmitSentFont.lfItalic,
		m_lfXmitSentFont.lfUnderline,
		m_lfXmitSentFont.lfStrikeOut,
		m_lfXmitSentFont.lfCharSet,
		m_lfXmitSentFont.lfPitchAndFamily);
	if(!pApp->WriteProfileString("Settings\\Fonts", "SentName", m_lfXmitSentFont.lfFaceName))
		return FALSE;
	if(!pApp->WriteProfileString("Settings\\Fonts", "Sent", str))
		return FALSE;

//save macro strings
	for( INT i=0; i<NUM_MACROSETS; i++)
	{
		for( INT j=0; j<NUM_MACROKEYS; j++)
		{
			str.Format("Title[%d][%d]", i,j);
			if(!pApp->WriteProfileString("Settings\\Macros", str, m_saMacroTitle[i*NUM_MACROKEYS + j]))
				return FALSE;
			str.Format("Macro[%d][%d]", i,j);
			if(!pApp->WriteProfileString("Settings\\Macros", str, m_saMacroData[i*NUM_MACROKEYS + j]))
				return FALSE;
		}
	}
//save other strings
	if(!pApp->WriteProfileString("Settings\\Strings", "MyCall", m_sMyCall))
		return FALSE;
	if(!pApp->WriteProfileString("Settings\\Strings", "FilePath", m_PathName))
		return FALSE;
	if(!pApp->WriteProfileString("Settings\\Strings", "InFilePath", m_InPathName))
		return FALSE;
	if(!pApp->WriteProfileString("Settings\\Strings", "OutFilePath", m_OutPathName))
		return FALSE;
	if(!pApp->WriteProfileString("Settings\\Strings", "PalettePath", m_PALPathName))
		return FALSE;
	
	return TRUE;
}
