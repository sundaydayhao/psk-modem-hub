// WinPSKDoc.cpp : implementation of the CWinPSKDoc class
//
//////////////////////////////////////////////////////////////////////
//

#include "stdafx.h"
#include "WinPSK.h"
#include "WinPSKDoc.h"
#include "WinPSKView.h"
#include "PlotTables.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CWinPSKDoc

IMPLEMENT_DYNCREATE(CWinPSKDoc, CDocument)

BEGIN_MESSAGE_MAP(CWinPSKDoc, CDocument)
	//{{AFX_MSG_MAP(CWinPSKDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinPSKDoc construction/destruction

CWinPSKDoc::CWinPSKDoc()
{
	m_pRcvEdit = NULL;
	m_pXmitEdit = NULL;
	m_pTheirCallEdit = NULL;
	m_pTheirNameEdit = NULL;
	m_pTextGrabEdit = NULL;
	m_pWinPSKView = NULL;
	m_hRcvEditCtrl = NULL;
	m_hXmitEditCtrl = NULL;
	m_MacroStartTX = FALSE;
	m_IMDRdy = FALSE;
	m_IMD = 0;
	m_SoundCardReset = FALSE;
}

CWinPSKDoc::~CWinPSKDoc()
{
	if( m_pSettings != NULL)
		delete m_pSettings;
	if(m_pXmitEdit)
		delete m_pXmitEdit;
	if(m_pRcvEdit)
		delete m_pRcvEdit;
	if(m_pTheirCallEdit)
		delete m_pTheirCallEdit;
	if(m_pTheirNameEdit)
		delete m_pTheirNameEdit;
	if(m_pTextGrabEdit)
		delete m_pTextGrabEdit;
}

////////////////////////////////////////////////////////////////////////
//   Get user settings or load defaults
////////////////////////////////////////////////////////////////////////
BOOL CWinPSKDoc::OnNewDocument()
{
	m_sTheirCall = _T("");
	m_sTheirName = _T("");
	m_sTextGrab = _T("");
	m_pSettings = NULL;
	m_pSettings = new CSettings;
	ASSERT(m_pSettings);
// load settings values that are not saved in the settings file.

	
//  clear out all the logfont memory before setting it's members
	memset(&m_pSettings->m_lfRcvFont, 0, sizeof m_pSettings->m_lfRcvFont);
	memset(&m_pSettings->m_lfXmitFont, 0, sizeof m_pSettings->m_lfXmitFont);
	memset(&m_pSettings->m_lfXmitSentFont, 0, sizeof m_pSettings->m_lfXmitSentFont);
	m_pSettings->m_saMacroData.SetSize(NUM_MACROSETS*NUM_MACROKEYS);
	m_pSettings->m_saMacroTitle.SetSize(NUM_MACROSETS*NUM_MACROKEYS);
///////////////////////////////////////////////////////////////////////
	if(	m_pSettings->ReadFromRegistry() == FALSE )
	{
/////////////////  Load default settings if settings file not found
	//BOOL's
		m_pSettings->m_UseAFC = TRUE;
		m_pSettings->m_UseSlash = TRUE;
		m_pSettings->m_fSlowAve = FALSE;
		m_pSettings->m_AutoTxtSel = TRUE;
		m_pSettings->m_TXNet = TRUE;
		m_pSettings->m_UseLSB = FALSE;
		m_pSettings->m_EchoTXText = TRUE;
		m_pSettings->m_UseLocalTime = FALSE;
		m_pSettings->m_AppendMode = FALSE;
		m_pSettings->m_TxOffsetMode = FALSE;
	//INT's
		m_pSettings->m_TabSel = FFT_DISPVIEW;
		m_pSettings->m_RxFreq = 1000;
		m_pSettings->m_TxFreq = 1000;
		m_pSettings->m_ModType = BPSK_MODE;
		m_pSettings->m_PTTPort = PTT_NONE;
		m_pSettings->m_SQThreshold = 25;
		m_pSettings->m_FreqMin = 200;
		m_pSettings->m_FreqMax = 3500;
		m_pSettings->m_ClockAdj = 0;
		m_pSettings->m_MacroSet = 0;
		m_pSettings->m_AFCLimit = 100;
		m_pSettings->m_SoundcardNum = -1;
		m_pSettings->m_SquelchSpeed = 75;
		m_pSettings->m_InWaveOffset = 0;
		m_pSettings->m_IOMode = 0;
		m_pSettings->m_OutWaveTimeLimit = 1;
		m_pSettings->m_TxFreqOffset = 0;
		m_pSettings->m_PlotViewState[FFT_DISPVIEW].xcur = 1000;
		m_pSettings->m_PlotViewState[FFT_DISPVIEW].zoom = 1;

		m_pSettings->m_PlotViewState[WATERFALL_DISPVIEW].xcur = 1000;
		m_pSettings->m_PlotViewState[WATERFALL_DISPVIEW].zoom = 1;

//colors	
		m_pSettings->m_crRcvBackColor = RGB(0,0,0);
		m_pSettings->m_crXmitBackColor = RGB(0,0,0);
		m_pSettings->m_crRcvTextColor = RGB(0,255,0);
		m_pSettings->m_crXmitTextColor = RGB(255,0,0);
		m_pSettings->m_crXmitSentTextColor = RGB(255,255,0);
// set a default font for receive and xmit edit boxes
		m_pSettings->m_lfRcvFont.lfHeight = -10;
		lstrcpy(m_pSettings->m_lfRcvFont.lfFaceName, _T("Arial"));
		m_pSettings->m_lfRcvFont.lfOutPrecision = OUT_TT_PRECIS;
		m_pSettings->m_lfRcvFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		m_pSettings->m_lfRcvFont.lfQuality = PROOF_QUALITY;
		m_pSettings->m_lfRcvFont.lfPitchAndFamily = FF_SWISS | VARIABLE_PITCH;
//
		m_pSettings->m_lfXmitFont.lfHeight = -10;
		lstrcpy(m_pSettings->m_lfXmitFont.lfFaceName, _T("Arial"));
		m_pSettings->m_lfXmitFont.lfOutPrecision = OUT_TT_PRECIS;
		m_pSettings->m_lfXmitFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		m_pSettings->m_lfXmitFont.lfQuality = PROOF_QUALITY;
		m_pSettings->m_lfXmitFont.lfPitchAndFamily = FF_SWISS | VARIABLE_PITCH;
//
		m_pSettings->m_lfXmitSentFont.lfHeight = -10;
		lstrcpy(m_pSettings->m_lfXmitSentFont.lfFaceName, _T("Arial"));
		m_pSettings->m_lfXmitSentFont.lfOutPrecision = OUT_TT_PRECIS;
		m_pSettings->m_lfXmitSentFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		m_pSettings->m_lfXmitSentFont.lfQuality = PROOF_QUALITY;
		m_pSettings->m_lfXmitSentFont.lfPitchAndFamily = FF_SWISS | VARIABLE_PITCH;
//Initialize macros
		m_pSettings->m_saMacroTitle.InsertAt(0, _T("QSO Start"));
		m_pSettings->m_saMacroData.InsertAt(0, _T("'start''theircall' de 'mycall'"));

		m_pSettings->m_saMacroTitle.InsertAt(1, _T("QSO BTU"));
		m_pSettings->m_saMacroData.InsertAt(1, _T("BTU 'theirname' 'theircall' de 'mycall' kn'stop'"));

		m_pSettings->m_saMacroTitle.InsertAt(2, _T("QSO Final"));
		m_pSettings->m_saMacroData.InsertAt(2, _T(" 73's sk sk 'theircall' de 'mycall'cl'stop'"));

		m_pSettings->m_saMacroTitle.InsertAt(3, _T("CQ"));
		m_pSettings->m_saMacroData.InsertAt(3, _T("'start'\r\nCQ CQ CQ de 'mycall' 'mycall'\r\nCQ CQ CQ de 'mycall' 'mycall'\r\nCQ CQ CQ de 'mycall' 'mycall' k k k'cwid''stop'"));

		m_pSettings->m_saMacroTitle.InsertAt(4, _T("Brag File"));
		m_pSettings->m_saMacroData.InsertAt(4, _T("<brag.txt>"));

		m_pSettings->m_saMacroTitle.InsertAt(5, _T("Undefined"));
		m_pSettings->m_saMacroData.InsertAt(5, _T(""));

		m_pSettings->m_saMacroTitle.InsertAt(6, _T("Undefined"));
		m_pSettings->m_saMacroData.InsertAt(6, _T(""));
	
		m_pSettings->m_saMacroTitle.InsertAt(7, _T("Undefined"));
		m_pSettings->m_saMacroData.InsertAt(7, _T(""));

		m_pSettings->m_saMacroTitle.InsertAt(8, _T("Undefined"));
		m_pSettings->m_saMacroData.InsertAt(8, _T(""));

		m_pSettings->m_saMacroTitle.InsertAt(9, _T("Undefined"));
		m_pSettings->m_saMacroData.InsertAt(9, _T(""));
		for( INT i=1; i<NUM_MACROSETS; i++)
		{
			for( INT j=0; j<NUM_MACROKEYS; j++)
			{
				m_pSettings->m_saMacroTitle.InsertAt(i*NUM_MACROKEYS + j, _T("Undefined"));
				m_pSettings->m_saMacroData.InsertAt(i*NUM_MACROKEYS + j, _T(""));
			}
		}
// Strings
		m_pSettings->m_sMyCall = _T("No Callsign Set");
		m_pSettings->m_PathName = _T("C:\\textfile.txt");
		m_pSettings->m_InPathName = _T("");
		m_pSettings->m_OutPathName = _T("");
		m_pSettings->m_PALPathName = _T("");
	}

	///////////////////////////////////////////////////////////////////////////
//  Init some variables that are not save in "Settings.dat"
	for( INT i=0; i<20; i++)
		m_SyncHist[i] = 0;
	m_pSettings->m_PALPathName = ReadPaletteFile(m_pSettings->m_PALPathName);
	m_SoundCardReset = FALSE;
	m_CWIdString = _T("de ") + m_pSettings->m_sMyCall;
	m_ClkError = 0;
	if(m_pSettings->m_FreqMax >3500)
		m_pSettings->m_FreqMax = 3500;
	if(m_pSettings->m_FreqMin < 200)
		m_pSettings->m_FreqMin = 200;
	m_pSettings->m_PlotViewState[INPUT_DISPVIEW].xcur = 1;
	m_pSettings->m_PlotViewState[INPUT_DISPVIEW].zoom = 1;
	m_pSettings->m_PlotViewState[SYNC_DISPVIEW].xcur = 1;
	m_pSettings->m_PlotViewState[SYNC_DISPVIEW].zoom = 1;
	if( m_pSettings->m_AFCLimit == 3000)
		m_SatMode = TRUE;
	else
		m_SatMode = FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CWinPSKDoc serialization to file
/////////////////////////////////////////////////////////////////////////////
void CWinPSKDoc::Serialize(CArchive& ar)
{
PCHAR pbuf;
INT length;
	if (ar.IsStoring())
	{
		length = m_pRcvEdit->GetTextLength();
		pbuf = new CHAR[length+10];
		m_pRcvEdit->SetSel(0,-1);
		m_pRcvEdit->GetSelText( pbuf );
		m_pRcvEdit->SetSel(length,length);
		for( INT i=0; i<length; i++)
			ar << pbuf[i];
		delete pbuf;
	}
	else
	{
		// add text loading code here maybe
	}

	// Calling the base class COleDocument enables serialization
	//  of the container document's COleClientItem objects.
	CDocument::Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CWinPSKDoc diagnostics

#ifdef _DEBUG
void CWinPSKDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWinPSKDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWinPSKDoc commands
/////////////////////////////////////////////////////////////////////////////
void CWinPSKDoc::OnCloseDocument() 
{
	m_pSettings->WriteToRegistry();
	CDocument::OnCloseDocument();
}



CString CWinPSKDoc::ReadPaletteFile(CString sPath)
{
CStdioFile cFile;
CString sRet= _T("");
CString str;
INT cRead;
INT i=0;
INT R,G,B;
	if( cFile.Open( sPath, CFile::modeRead) )
	{
		while( cFile.ReadString( str ) )
		{
			cRead = _stscanf (str, "%i %i %i", &R,&G,&B);
			m_ColorTbl[255-i] = R + (G<<8) + (B<<16); 
			i++;
		}
	}
	if(i==256)
	{
		sRet= sPath;
	}
	else
	{
		for( i=0; i<256; i++)
			m_ColorTbl[i] = DEFAULT_COLORTBL[i];
	}
	return sRet;
}
