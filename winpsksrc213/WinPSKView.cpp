// WinPSKView.cpp : implementation of the CWinPSKView class
//
//////////////////////////////////////////////////////////////////////
//

#include "stdafx.h"
#include "WinPSK.h"
#include "Macrodlg.h"
#include "WinPSKDoc.h"
#include "WinPSKView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FFT_SAMPLERATE 8000
#define FFT_SIZE 2048

#define SOUNCARD_NUM -1
#define SEARCH_RANGE 45

#define DLLSTAT_OFF -1			//DLL status/error codes
#define DLLSTAT_RX 0
#define DLLSTAT_TX 1
#define DLLSTAT_CPUSLOW 2
#define DLLSTAT_TXFINISHING 3

#define ID_TIMER 1
#define SATMODE (1<<7)

/////////////////////////////////////////////////////////////////////////////
// CWinPSKView

IMPLEMENT_DYNCREATE(CWinPSKView, CFormView)

BEGIN_MESSAGE_MAP(CWinPSKView, CFormView)
	//{{AFX_MSG_MAP(CWinPSKView)
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_DISPLAYTAB, OnSelchangeDisplaytab)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(UDN_DELTAPOS, IDC_ZOOM, OnDeltaposZoom)
	ON_NOTIFY(UDN_DELTAPOS, IDC_RXFREQSPIN, OnDeltaposRxFreqspin)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_SLOWAVE, OnSlowave)
	ON_BN_CLICKED(IDC_AFC, OnAfc)
	ON_BN_CLICKED(IDC_TRBUTTON, OnTrbutton)
	ON_BN_CLICKED(IDC_BPSK, OnBpsk)
	ON_BN_CLICKED(IDC_QPSK, OnQpsk)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_NET, OnNet)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
	ON_WM_CONTEXTMENU()
	ON_EN_KILLFOCUS(IDC_RXFREQ, OnKillfocusRxfreq)
	ON_EN_SETFOCUS(IDC_RXFREQ, OnSetfocusRxfreq)
	ON_EN_KILLFOCUS(IDC_TXFREQ, OnKillfocusTxfreq)
	ON_NOTIFY(UDN_DELTAPOS, IDC_TXFREQSPIN, OnDeltaposTxfreqspin)
	ON_EN_SETFOCUS(IDC_TXFREQ, OnSetfocusTxfreq)
	ON_BN_CLICKED(IDC_M1BUTTON, OnM1button)
	ON_BN_CLICKED(IDC_M2BUTTON, OnM2button)
	ON_BN_CLICKED(IDC_M3BUTTON, OnM3button)
	ON_BN_CLICKED(IDC_M4BUTTON, OnM4button)
	ON_BN_CLICKED(IDC_M5BUTTON, OnM5button)
	ON_BN_CLICKED(IDC_M6BUTTON, OnM6button)
	ON_BN_CLICKED(IDC_M7BUTTON, OnM7button)
	ON_BN_CLICKED(IDC_M8BUTTON, OnM8button)
	ON_BN_CLICKED(IDC_M9BUTTON, OnM9button)
	ON_BN_CLICKED(IDC_M10BUTTON, OnM10button)
	ON_BN_CLICKED(IDC_AUTOTXTSEL, OnAutotxtsel)
	ON_BN_CLICKED(IDC_CLRQSO, OnClrqso)
	ON_BN_CLICKED(IDC_DECFNSET, OnDecfnset)
	ON_BN_CLICKED(IDC_INCFNSET, OnIncfnset)
	ON_BN_CLICKED(IDC_SETHOME, OnSethome)
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_RUN, OnRun)
	ON_BN_CLICKED(IDC_TXFMODE, OnTxFreqMode)
	ON_BN_CLICKED(IDC_PSK3163, OnPsk3163)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify )
	ON_MESSAGE( MSG_DATARDY, OnDataRdy)
	ON_MESSAGE( MSG_PSKCHARRDY, OnCharRdy)
	ON_MESSAGE( MSG_STATUSCHANGE, OnStatusChange)
	ON_MESSAGE( MSG_IMDRDY, OnIMDRdy)
	ON_MESSAGE( MSG_CLKERROR, OnClkError)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinPSKView construction/destruction
/////////////////////////////////////////////////////////////////////////////
CWinPSKView::CWinPSKView()
	: CFormView(CWinPSKView::IDD)
{
	//{{AFX_DATA_INIT(CWinPSKView)
	m_fSlowAve = FALSE;
	m_UseAFC = FALSE;
	m_TXNet = FALSE;
	m_AutoTxtSel = TRUE;
	m_RxFreq = 1000;
	m_TxDisp = 1000;
	m_PSKMode = 0;
	m_XMaxCStr = _T("");
	m_XMinCStr = _T("");
	m_ZoomTxt = _T("");
	m_PSK63 = FALSE;
	//}}AFX_DATA_INIT
	m_pCPlotData = NULL;
	m_fRxFreqEditing = FALSE;
	m_fTxFreqEditing = FALSE;
	m_pSquelchCtrl = NULL;
	m_SearchRange = SEARCH_RANGE;
	m_IMDDispDelay = 0;
	m_ProgramState = STATE_RX;
	m_ScreenInitialized = FALSE;
	m_DLLRunning = FALSE;
	m_NewChannel = 0;
}

CWinPSKView::~CWinPSKView()
{
}

/////////////////////////////////////////////////////////////////////////////
void CWinPSKView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWinPSKView)
	DDX_Control(pDX, IDC_TXFMODE, m_TxFreqModeCtrl);
	DDX_Control(pDX, IDC_RUN, m_RunCtrl);
	DDX_Control(pDX, IDC_DEBUGTXT2, m_DebugCtrl2);
	DDX_Control(pDX, IDC_DEBUGTXT1, m_DebugCtrl1);
	DDX_Check(pDX, IDC_SLOWAVE, m_fSlowAve);
	DDX_Check(pDX, IDC_AFC, m_UseAFC);
	DDX_Check(pDX, IDC_NET, m_TXNet);
	DDX_Check(pDX, IDC_AUTOTXTSEL, m_AutoTxtSel);
	DDX_Text(pDX, IDC_RXFREQ, m_RxFreq);
	DDX_Text(pDX, IDC_TXFREQ, m_TxDisp);
	DDX_Radio(pDX, IDC_BPSK, m_PSKMode);
	DDX_Control(pDX, IDC_XMAXTXT, m_XMaxCntrl);
	DDX_Control(pDX, IDC_MACROSETTXT, m_MacroSetTxtCtrl);
	DDX_Control(pDX, IDC_QPSK, m_QPSKCtrl);
	DDX_Control(pDX, IDC_TRBUTTON, m_TRCntrl);
	DDX_Control(pDX, IDC_XMINTXT, m_XMinCntrl);
	DDX_Control(pDX, IDC_DISPLAYTAB, m_DispTabCtrl);
	DDX_Text(pDX, IDC_XMAXTXT, m_XMaxCStr);
	DDX_Text(pDX, IDC_XMINTXT, m_XMinCStr);
	DDX_Text(pDX, IDC_ZOOMTXT, m_ZoomTxt);
	DDX_Check(pDX, IDC_PSK3163, m_PSK63);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_M1BUTTON, m_MTitleCtrl[0]);	//F1 to F10 keys
	DDX_Control(pDX, IDC_M2BUTTON, m_MTitleCtrl[1]);
	DDX_Control(pDX, IDC_M3BUTTON, m_MTitleCtrl[2]);
	DDX_Control(pDX, IDC_M4BUTTON, m_MTitleCtrl[3]);
	DDX_Control(pDX, IDC_M5BUTTON, m_MTitleCtrl[4]);
	DDX_Control(pDX, IDC_M6BUTTON, m_MTitleCtrl[5]);
	DDX_Control(pDX, IDC_M7BUTTON, m_MTitleCtrl[6]);
	DDX_Control(pDX, IDC_M8BUTTON, m_MTitleCtrl[7]);
	DDX_Control(pDX, IDC_M9BUTTON, m_MTitleCtrl[8]);
	DDX_Control(pDX, IDC_M10BUTTON, m_MTitleCtrl[9]);
}

BOOL CWinPSKView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CFormView::PreCreateWindow(cs);
}


/////////////////////////////////////////////////////////////////////////////
//  Called to initialize the main FormView object.
/////////////////////////////////////////////////////////////////////////////
void CWinPSKView::OnInitialUpdate()
{
CSize size;
	CFormView::OnInitialUpdate();
	EnableToolTips(TRUE);
// save off original mouse cursor
	m_hOrigCursor =	(HCURSOR)::GetClassLong(GetSafeHwnd(),GCL_HCURSOR);
// get the system cross cursor for use when mouse goes over the plot
	m_hCross = AfxGetApp()->LoadStandardCursor(IDC_CROSS);

	m_TRCntrl.SetIcon(AfxGetApp()->LoadIcon(IDI_TRRCV));
	CWinPSKDoc* pDoc = (CWinPSKDoc*) GetDocument();
	ASSERT_VALID( pDoc);
	if (pDoc != NULL)
	{
		// See if need to Keep program "ON Top" of all others.
		CMainFrame* pFrm=(CMainFrame*)AfxGetMainWnd();
		pFrm->StayOnTop();	
		// create "theircall" edit box
		CRect cr;
		CWnd* pWnd = GetDlgItem( IDC_THEIRCALL );
		ASSERT_VALID(pWnd);
		pWnd->GetWindowRect( cr );
		ScreenToClient( cr);
		if(!pDoc->m_pTheirCallEdit)
		{
			pDoc->m_pTheirCallEdit = new CTheirCallCtrl;
			pDoc->m_pTheirCallEdit->Create( WS_VISIBLE | WS_BORDER | WS_CHILD |
					WS_GROUP , cr, this, IDC_RICHEDIT3);
		}
		// create "theirname" edit box
		pWnd = GetDlgItem( IDC_THEIRNAME );
		ASSERT_VALID(pWnd);
		pWnd->GetWindowRect( cr );
		ScreenToClient( cr);
		if(!pDoc->m_pTheirNameEdit)
		{
			pDoc->m_pTheirNameEdit = new CTheirNameCtrl;
			pDoc->m_pTheirNameEdit->Create( WS_VISIBLE | WS_BORDER | WS_CHILD |
					WS_GROUP , cr, this, IDC_RICHEDIT4);
		}
		// create "TextGrab" edit box
		pWnd = GetDlgItem( IDC_TEXTGRAB );
		ASSERT_VALID(pWnd);
		pWnd->GetWindowRect( cr );
		ScreenToClient( cr);
		if(!pDoc->m_pTextGrabEdit)
		{
			pDoc->m_pTextGrabEdit = new CTextGrabCtrl;
			pDoc->m_pTextGrabEdit->Create( WS_VISIBLE | WS_BORDER | WS_CHILD |
					WS_GROUP | ES_AUTOHSCROLL, cr, this, IDC_RICHEDIT5);
		}
		//instantiate display data plotting and processing  objects
		if(!m_pCPlotData)
		{
			m_pCPlotData = new CPlotData();		//initialize Data plotting resources
			ASSERT(m_pCPlotData);
		}
//
		pDoc->m_pWinPSKView = this;			//store this view ptr in Document class

		pWnd = GetDlgItem( IDC_PLOTAREA );
		ASSERT_VALID(pWnd);					//give ciew client area to Plot object
		pWnd->GetClientRect( cr );
		m_PlotWidth = cr.right;
		m_pCPlotData->SetPlotSize(cr); 

		CString str;
		str = _T("WinPsk 2.13 -- ") + pDoc->m_pSettings->m_sMyCall;
		AfxGetMainWnd()->SetWindowText( str );

		if(!m_pSquelchCtrl)
		{
			m_pSquelchCtrl = new CSquelchCtrl();
			ASSERT_VALID(m_pSquelchCtrl);
		}
		CRect r;
		GetDlgItem(IDC_SQUELCH)->GetWindowRect(r); 
		ScreenToClient(r);
		BOOL res = m_pSquelchCtrl->Create( "",WS_VISIBLE|WS_CHILD|WS_DLGFRAME,
											r, this, IDC_SQUELCH, (PCALLBACKFUNCT)SquelchThreshCallback);
		m_pSquelchCtrl->SetThreshold( pDoc->m_pSettings->m_SQThreshold);
		fnSetSquelchThreshold(pDoc->m_pSettings->m_SQThreshold,
				pDoc->m_pSettings->m_SquelchSpeed, RX_CHAN);
		m_pSquelchCtrl->SetLevel(0);

// Initialize controls to values in "pDoc->m_Settings"		
		if(	pDoc->m_pSettings->m_TxOffsetMode)
		{
			m_TxDisp = pDoc->m_pSettings->m_TxFreqOffset;
			m_TxFreqModeCtrl.SetWindowText( _T("TxOffset") );
		}
		else
		{
			m_TxDisp = pDoc->m_pSettings->m_TxFreq;
			m_TxFreqModeCtrl.SetWindowText( _T("Tx Freq") );
		}
		m_PSKMode = pDoc->m_pSettings->m_ModType;
		m_PSK63 = pDoc->m_pSettings->m_PSK63Mode;
		if(pDoc->m_pSettings->m_UseLSB)
			m_QPSKCtrl.SetWindowText(_T("QPSK (lsb)"));
		else
			m_QPSKCtrl.SetWindowText(_T("QPSK (usb)"));
		m_UseAFC = pDoc->m_pSettings->m_UseAFC;
		m_TXNet = pDoc->m_pSettings->m_TXNet;
		m_fSlowAve = pDoc->m_pSettings->m_fSlowAve;
		m_AutoTxtSel = pDoc->m_pSettings->m_AutoTxtSel;
		pDoc->m_pSettings->m_MacroSet--;
		OnIncfnset();
		if( (pDoc->m_pSettings->m_TabSel == FFT_DISPVIEW ) ||
					(pDoc->m_pSettings->m_TabSel == WATERFALL_DISPVIEW ) )
		{
			m_ZoomOK = TRUE;
			m_pCPlotData->SetPlotCursorIndex( pDoc->m_pSettings->
						m_PlotViewState[pDoc->m_pSettings->m_TabSel].xcur );
			m_pCPlotData->SetZoom( pDoc->m_pSettings->
						m_PlotViewState[pDoc->m_pSettings->m_TabSel].zoom,TRUE,
						pDoc->m_pSettings->m_FreqMin,
						pDoc->m_pSettings->m_FreqMax );
		}
		else
		{
			m_ZoomOK = FALSE;
			m_pCPlotData->SetZoom( 1, FALSE,
						pDoc->m_pSettings->m_FreqMin,
						pDoc->m_pSettings->m_FreqMax );
		}
		UpdatePlotInfo(PLOT_CHNG_ZOOM);
		UpdatePlotInfo(PLOT_CHNG_FREQ);
//  Add plot display TAB control initialization here
		TC_ITEM tci;
		tci.mask = TCIF_TEXT;
		str = _T("Spectrum");
		tci.pszText = (LPSTR)(LPCTSTR)str;
		tci.cchTextMax = str.GetLength();
		m_DispTabCtrl.InsertItem(FFT_DISPVIEW, &tci);
		str = _T("Waterfall");
		tci.pszText = (LPSTR)(LPCTSTR)str;
		tci.cchTextMax = str.GetLength();
		m_DispTabCtrl.InsertItem(WATERFALL_DISPVIEW, &tci);
		str = _T("Input");
		tci.pszText = (LPSTR)(LPCTSTR)str;
		tci.cchTextMax = str.GetLength();
		m_DispTabCtrl.InsertItem(INPUT_DISPVIEW, &tci);
		str = _T("Data Sync");
		tci.pszText = (LPSTR)(LPCTSTR)str;
		tci.cchTextMax = str.GetLength();
		m_DispTabCtrl.InsertItem(SYNC_DISPVIEW, &tci);

		m_DispTabCtrl.SetCurSel(pDoc->m_pSettings->m_TabSel);
		m_TimerID = SetTimer( ID_TIMER, 500, NULL);	//start up status timer
		if( m_TimerID == 0)
			AfxMessageBox( _T("Can't Start Status Timer"),MB_OK );
//
		INT tmp = 0;
		if(pDoc->m_pSettings->m_PSK63Mode)
			tmp |= PSK63MODE;
		if( pDoc->m_pSettings->m_ModType == BPSK_MODE )
		{
			fnSetRXPSKMode( BPSK_MODE|tmp, RX_CHAN);
		}
		else
		{
			if(pDoc->m_pSettings->m_UseLSB)
				fnSetRXPSKMode( IQPSK_MODE|tmp, RX_CHAN);
			else
				fnSetRXPSKMode( QPSK_MODE|tmp, RX_CHAN);
		}
		if(pDoc->m_pSettings->m_UseAFC)
		{
			fnSetAFCLimit(pDoc->m_pSettings->m_AFCLimit, RX_CHAN);
			m_SearchRange = SEARCH_RANGE;
		}
		else
		{
			fnSetAFCLimit( 0, RX_CHAN);
			m_SearchRange = 0;
		}
		fnSetClockErrorAdjustment(pDoc->m_pSettings->m_ClockAdj);
		fnSetCWIDString((PCHAR)(LPCTSTR)pDoc->m_CWIdString );
		if(pDoc->m_pSettings->m_PTTPort)
			fnSetComPort(pDoc->m_pSettings->m_PTTPort, 3);
		else
			fnSetComPort(0, 0);
		if(pDoc->m_pSettings->m_TxOffsetMode)
			fnSetTXFrequency( pDoc->m_pSettings->m_TxFreq +
								pDoc->m_pSettings->m_TxFreqOffset );
		else
			fnSetTXFrequency( pDoc->m_pSettings->m_TxFreq );
		fnSetRXFrequency( pDoc->m_pSettings->m_RxFreq , 0, RX_CHAN);
		m_RunCtrl.SetIcon(AfxGetApp()->LoadIcon(IDI_START));
		m_TRCntrl.EnableWindow(FALSE);
		pDoc->m_FilePercentDone = -1;
		if( (pDoc->m_pSettings->m_IOMode&(IOM_WAVEIN|IOM_WAVELOG|IOM_WAVETX) )
							== 0 )
		{
			StartDLL(pDoc);	//autostart if no wave files are involved
		}
	}
	m_ScreenInitialized = FALSE;
}


void CWinPSKView::StartDLL(CWinPSKDoc *pDoc)
{
INT err;
LONG length;
INT Mode = pDoc->m_pSettings->m_IOMode;
CHAR errstr[50];
	pDoc->m_FilePercentDone = -1;
	if(!m_DLLRunning)
	{
		if( Mode&IOM_WAVEIN)
		{
			err = fnSetInputWavePath((PCHAR)(LPCTSTR)pDoc->m_pSettings->m_InPathName,
									&length,
									pDoc->m_pSettings->m_InWaveOffset);
			if(err!=0)
			{
				fnGetErrorString( errstr );
				AfxMessageBox( errstr);
				return;
			}
		}
		if( Mode&(IOM_WAVELOG|IOM_WAVETX))
		{
			err = fnSetOutputWavePath((PCHAR)(LPCTSTR)pDoc->m_pSettings->m_OutPathName,
									pDoc->m_pSettings->m_OutWaveTimeLimit*60,
									pDoc->m_pSettings->m_AppendMode);
			if(err!=0)
			{
				fnGetErrorString( errstr );
				AfxMessageBox( errstr);
				return;
			}
		}
		
		err = fnStartSoundCardEx( m_hWnd, pDoc->m_pSettings->m_SoundcardNum, RX_CHAN+1, Mode );
		if( err )
		{
			fnGetErrorString( errstr );
			AfxMessageBox( errstr);
			if(err==21)		//default soundcardnumber if can't open
			{
				pDoc->m_pSettings->m_SoundcardNum = -1;
				err = fnStartSoundCardEx( m_hWnd, pDoc->m_pSettings->m_SoundcardNum, RX_CHAN+1, Mode );
			}
			if(err)
			{
				fnStopSoundCard();
				m_RunCtrl.SetIcon(AfxGetApp()->LoadIcon(IDI_START));
				m_TRCntrl.EnableWindow(FALSE);
				m_DLLRunning = FALSE;
				return;
//				ExitProcess(0);
			}
		}
		fnAbortTX();
		pDoc->m_pXmitEdit->ClearAll();
		m_DLLRunning = TRUE;
	}
	m_RunCtrl.SetIcon(AfxGetApp()->LoadIcon(IDI_STOP));
	m_TRCntrl.EnableWindow(TRUE);
}

void CWinPSKView::StopDLL()
{
	if(m_DLLRunning)
	{
		fnStopSoundCard();
		m_DLLRunning = FALSE;
		OnStatusChange(DLL_RX_STATUS, 0);
	}
	m_RunCtrl.SetIcon(AfxGetApp()->LoadIcon(IDI_START));
	m_TRCntrl.EnableWindow(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
//  Called to release all the View object resources.
/////////////////////////////////////////////////////////////////////////////
void CWinPSKView::OnDestroy()
{
	CWinPSKDoc* pDoc = (CWinPSKDoc*) GetDocument();
	KillTimer(m_TimerID);
	if(m_pCPlotData)
	{
		delete m_pCPlotData;
		m_pCPlotData = NULL;
	}
	if (pDoc != NULL)
	{
		if(pDoc->m_pTheirCallEdit)
		{
			delete pDoc->m_pTheirCallEdit;
			pDoc->m_pTheirCallEdit = NULL;
		}

	}	
	if( m_pSquelchCtrl )
	{
		delete m_pSquelchCtrl;
		m_pSquelchCtrl = NULL;
	}
	CFormView::OnDestroy();

}


/////////////////////////////////////////////////////////////////////////////
// CWinPSKView diagnostics
/////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void CWinPSKView::AssertValid() const
{
	CFormView::AssertValid();
}

void CWinPSKView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CWinPSKDoc* CWinPSKView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinPSKDoc)));
	return (CWinPSKDoc*)m_pDocument;
}
#endif //_DEBUG

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//					CWinPSKView message handlers
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//////////////////////////////////////////////////////////////////////
// Called when view screen coordinates have changed.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::OnDraw(CDC* pDC) 
{
	//go recalculate & draw the data plots
	if(m_pCPlotData)
		m_pCPlotData->OnDrawPlot( m_hWnd, this, (CWinPSKDoc*)GetDocument() );
	m_ScreenInitialized = TRUE;
}

//////////////////////////////////////////////////////////////////////
// Called when DLL has new FFT data to display every .256 seconds.
//////////////////////////////////////////////////////////////////////
afx_msg LRESULT CWinPSKView::OnDataRdy(UINT rxfreq, LONG SqLevel)
{
CWinPSKDoc* pDoc;
	pDoc = GetDocument();
	ASSERT_VALID( pDoc );
	if( (rxfreq != (UINT)m_RxFreq) && !m_fRxFreqEditing && !m_fTxFreqEditing)
	{
		m_RxFreq = (INT)rxfreq;
		pDoc->m_pSettings->m_RxFreq = m_RxFreq;
		UpdateData(FALSE);		//Put changed data into control
		UpdatePlotInfo(PLOT_CHNG_FREQ);
	}
	if(SqLevel < 10 )	//clamp min sq level to 10
		SqLevel = 10;
	m_pSquelchCtrl->SetLevel( SqLevel );
	//go draw the data plots
	if(m_pCPlotData && m_ScreenInitialized)
		m_pCPlotData->DrawPlot();

#if 0
double d1,d2;
INT i1;
CString str;
	i1=fnGetDebugData(&d1,&d2);
	str.Format("%g",d1);
	m_DebugCtrl1.SetWindowText(str);
	str.Format("%g",d2);
	m_DebugCtrl2.SetWindowText(str);
#endif
	return 0;
}

//////////////////////////////////////////////////////////////////////
// Called when DLL has new character to display from rx or tx.
//////////////////////////////////////////////////////////////////////
afx_msg LRESULT CWinPSKView::OnCharRdy(UINT ch, LONG src)
{
CWinPSKDoc* pDoc = GetDocument();
CString str;
	ASSERT_VALID( pDoc );
	if( src < 0 )		//if src is xmitted char
	{
		if(pDoc->m_pSettings->m_EchoTXText )
		{
			if( !pDoc->m_SatMode)
				pDoc->m_pRcvEdit->OnRcvChar( ch, src);
			pDoc->m_pXmitEdit->UpdateFormats();
		}
	}
	else
	{
		if( pDoc->m_SatMode && (m_ProgramState!=STATE_RX))
			pDoc->m_pRcvEdit->OnRcvChar( ch, -1);
		else
			pDoc->m_pRcvEdit->OnRcvChar( ch, src);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////
// Called when DLL has new IMD data ready.
//////////////////////////////////////////////////////////////////////
afx_msg LRESULT CWinPSKView::OnIMDRdy(UINT IMD, LONG chan)
{
CWinPSKDoc* pDoc = GetDocument();
	ASSERT_VALID( pDoc );
	pDoc->m_IMD = IMD;
	if( chan&0x80 )
		pDoc->m_IMD = -(INT)IMD;
	pDoc->m_IMDRdy = TRUE;
	m_IMDDispDelay = 5;
	return 0;
}

//////////////////////////////////////////////////////////////////////
// Called when DLL has new clock error data.
//////////////////////////////////////////////////////////////////////
afx_msg LRESULT CWinPSKView::OnClkError(UINT error, LONG chan)
{
CWinPSKDoc* pDoc = GetDocument();
	ASSERT_VALID( pDoc );
	pDoc->m_ClkError = error;
	return 0;
}

//////////////////////////////////////////////////////////////////////
// Called when DLL has changed status or has an error.
//////////////////////////////////////////////////////////////////////
afx_msg LRESULT CWinPSKView::OnStatusChange(UINT status, LONG percent)
{
CHAR errstr[50];
CWinPSKDoc* pDoc = GetDocument();
	ASSERT_VALID( pDoc );

	switch(status)
	{
		case DLL_RX_STATUS:
			m_TRCntrl.SetIcon(AfxGetApp()->LoadIcon(IDI_TRRCV));
			if(	m_ProgramState != STATE_RX)
			{
				pDoc->m_pRcvEdit->TXStamp(pDoc->m_pSettings->m_EchoTXText,
									FALSE, pDoc->m_pSettings->m_UseLocalTime);
				pDoc->m_pXmitEdit->ClearSentText();
			}
			m_ProgramState = STATE_RX;
			break;
		case DLL_TX_STATUS:
			if(m_ProgramState != STATE_TUNE)
			{
				m_ProgramState = STATE_TX;
				m_TRCntrl.SetIcon(AfxGetApp()->LoadIcon(IDI_TRXMIT));
			}
			break;
		case DLL_FINISH_STATUS:
			m_TRCntrl.SetIcon(AfxGetApp()->LoadIcon(IDI_TXFINISH));
			m_ProgramState = STATE_FINISH;
			break;
		case DLL_SLOW_STATUS:
			pDoc->m_SoundCardReset = TRUE;
			break;
		case DLL_INWAVEDONE:
		case DLL_OUTWAVEDONE:
			StopDLL();
			break;
		case DLLSTAT_INWAVEFILESTATUS:
			if( percent > pDoc->m_FilePercentDone)
				pDoc->m_FilePercentDone = percent;
			break;
		case DLLSTAT_OUTWAVEFILESTATUS:
			if( percent > pDoc->m_FilePercentDone)
				pDoc->m_FilePercentDone = percent;
			break;
		default:
			if( status )
			{
				fnGetErrorString( errstr );
				AfxMessageBox( errstr);
				if(status==21)		//default soundcardnumber
					pDoc->m_pSettings->m_SoundcardNum = -1;
//				AfxGetMainWnd()->PostMessage(WM_CLOSE);
				m_RunCtrl.SetIcon(AfxGetApp()->LoadIcon(IDI_START));
				m_TRCntrl.EnableWindow(FALSE);
				m_DLLRunning = FALSE;
//				ExitProcess(0);
			}
			break;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////
// Called when mouse moves in view client window.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if( m_ZoomOK )
	{
		::SetClassLong(GetSafeHwnd(),GCL_HCURSOR,NULL);	//prevent flickering
		if( m_pCPlotData->CurInPlot(point) )
			::SetCursor( m_hCross );
		else
			::SetClassLong(GetSafeHwnd(),GCL_HCURSOR,(LONG)m_hOrigCursor);
	}
	CFormView::OnMouseMove(nFlags, point);
}

//////////////////////////////////////////////////////////////////////
// Called when left mouse button is clicked.
//  Frequency cursor is changed to the mouse position.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::OnLButtonDown(UINT nFlags, CPoint point) 
{
CWinPSKDoc* pDoc;
	if( m_ZoomOK )
	{
		if(m_pCPlotData->CurInPlot(point) )
		{
			pDoc = GetDocument();
			ASSERT_VALID( pDoc );
			m_fRxFreqEditing = FALSE;
			m_fTxFreqEditing = FALSE;
			m_pCPlotData->SetPlotCursorPos( point.x );
			m_pCPlotData->SetZoom( pDoc->m_pSettings->m_PlotViewState
										[pDoc->m_pSettings->m_TabSel].zoom,FALSE,
						pDoc->m_pSettings->m_FreqMin,
						pDoc->m_pSettings->m_FreqMax );
			UpdatePlotInfo(PLOT_CHNG_MOUSE);
		}
	}
	SetFocus();
	CFormView::OnLButtonDown(nFlags, point);
}

//////////////////////////////////////////////////////////////////////
// Called when right mouse button is clicked.
//  Frequency cursor is changed to the mouse position.
//  Input data is rewinded to the spot in the waterfall or specral view.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::OnRButtonDown(UINT nFlags, CPoint point) 
{
CWinPSKDoc* pDoc;
	if( m_ZoomOK )
	{
		if(m_pCPlotData->CurInPlot(point) )
		{
			pDoc = GetDocument();
			ASSERT_VALID( pDoc );
			m_fRxFreqEditing = FALSE;
			m_fTxFreqEditing = FALSE;
			m_pCPlotData->SetPlotCursorPos( point.x );
			m_pCPlotData->SetRewindPos( point.y );
			m_pCPlotData->SetZoom( pDoc->m_pSettings->m_PlotViewState
										[pDoc->m_pSettings->m_TabSel].zoom,FALSE,
						pDoc->m_pSettings->m_FreqMin,
						pDoc->m_pSettings->m_FreqMax );
			UpdatePlotInfo(PLOT_CHNG_MOUSE);
		}
	}
	SetFocus();	
	CFormView::OnRButtonDown(nFlags, point);
}


//////////////////////////////////////////////////////////////////////
// Called when Slow fft average button is pushed.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::OnSlowave() 
{
CWinPSKDoc* pDoc = GetDocument();
	ASSERT_VALID( pDoc );
	UpdateData(TRUE);		//Get value from control
	pDoc->m_pSettings->m_fSlowAve = m_fSlowAve;
	pDoc->m_FFTModeChange = TRUE;
}

//////////////////////////////////////////////////////////////////////
// Called when Auto Text check box is changed.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::OnAutotxtsel() 
{
	UpdateData(TRUE);		//Get value from control
	GetDocument()->m_pSettings->m_AutoTxtSel = m_AutoTxtSel;
}

//////////////////////////////////////////////////////////////////////
// Called when AFC button is pushed.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::OnAfc() 
{
CWinPSKDoc* pDoc = GetDocument();
	UpdateData(TRUE);		//Get value from control
	pDoc->m_pSettings->m_UseAFC = m_UseAFC;
	if(m_UseAFC)
	{
		fnSetAFCLimit(pDoc->m_pSettings->m_AFCLimit, RX_CHAN);
		m_SearchRange = SEARCH_RANGE;
	}
	else
	{
		fnSetAFCLimit( 0, RX_CHAN);
		m_SearchRange = 0;
	}
}

//////////////////////////////////////////////////////////////////////
// Called when left mouse button is DOUBLE clicked.
//  Frequency is cursor changed to the mouse position and the view is
//  centered about the cursor.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
CWinPSKDoc* pDoc;
	if( m_ZoomOK )
	{
		if(m_pCPlotData->CurInPlot(point) )
		{
			pDoc = GetDocument();
			ASSERT_VALID( pDoc );
			m_pCPlotData->SetPlotCursorPos( point.x );
			m_pCPlotData->SetZoom( pDoc->m_pSettings->m_PlotViewState
										[pDoc->m_pSettings->m_TabSel].zoom,FALSE,
						pDoc->m_pSettings->m_FreqMin,
						pDoc->m_pSettings->m_FreqMax );
			UpdatePlotInfo(PLOT_CHNG_MOUSE);
			m_pCPlotData->SetZoom(pDoc->m_pSettings->
							m_PlotViewState[pDoc->m_pSettings->m_TabSel].zoom,TRUE,
							pDoc->m_pSettings->m_FreqMin,
							pDoc->m_pSettings->m_FreqMax );
		}
	}
	CFormView::OnLButtonDblClk(nFlags, point);
}


//////////////////////////////////////////////////////////////////////
// Called when plot select tabs are clicked.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::OnSelchangeDisplaytab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	INT tab = m_DispTabCtrl.GetCurSel();
	CWinPSKDoc* pDoc = GetDocument();
	ASSERT_VALID( pDoc );
	//save old tab state
	pDoc->m_pSettings->m_PlotViewState[pDoc->m_pSettings->m_TabSel].xcur =
							m_pCPlotData->GetPlotCursorIndex();
	pDoc->m_pSettings->m_PlotViewState[pDoc->m_pSettings->m_TabSel].zoom =
							m_pCPlotData->GetZoom();
	pDoc->m_pSettings->m_TabSel = tab;	// save new tab
	if( (tab == FFT_DISPVIEW ) || (tab == WATERFALL_DISPVIEW ) )
		m_ZoomOK = TRUE;
	else
		m_ZoomOK = FALSE;
	if( m_ZoomOK )
	{
		m_pCPlotData->SetPlotCursorIndex( pDoc->m_pSettings->m_PlotViewState[tab].xcur );
		m_pCPlotData->SetZoom( pDoc->m_pSettings->m_PlotViewState[tab].zoom,TRUE,
								pDoc->m_pSettings->m_FreqMin,
								pDoc->m_pSettings->m_FreqMax );
	}
	UpdatePlotInfo(PLOT_CHNG_ZOOM);
	UpdatePlotInfo(PLOT_CHNG_FREQ);
	UpdateData(FALSE);					//Put changed data into control
	m_pCPlotData->ClearBuffers();
	pDoc->m_FFTModeChange = TRUE;
	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////
// Called when zoom spin control is clicked.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::OnDeltaposZoom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	ChngZoom( -pNMUpDown->iDelta );
	*pResult = 0;
}


//////////////////////////////////////////////////////////////////////
// Called when RX freq edit box gets focus.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::OnSetfocusRxfreq() 
{
	m_fRxFreqEditing = TRUE;
}

//////////////////////////////////////////////////////////////////////
// Called when center frequency edit control is changed.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::OnKillfocusRxfreq() 
{
CWinPSKDoc* pDoc = GetDocument();
	UpdateData(TRUE);		//Get value from control
	if(m_RxFreq < IndexToFreq( m_pCPlotData->GetXmin()) )
		m_RxFreq = IndexToFreq( m_pCPlotData->GetXmin());
	if(m_RxFreq > IndexToFreq( m_pCPlotData->GetXmax()) )
		m_RxFreq = IndexToFreq( m_pCPlotData->GetXmax());
	pDoc->m_pSettings->m_RxFreq = m_RxFreq;
	fnSetRXFrequency(m_RxFreq, 0, RX_CHAN);
	UpdateData(FALSE);		//Put changed data into control
	UpdatePlotInfo(PLOT_CHNG_FREQ);
	UpdatePlotInfo(PLOT_CHNG_ZOOM);
	m_fRxFreqEditing = FALSE;
	m_fTxFreqEditing = FALSE;
}


//////////////////////////////////////////////////////////////////////
// Called when TX freq edit box gets focus.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::OnSetfocusTxfreq() 
{
	m_fTxFreqEditing = TRUE;
}

//////////////////////////////////////////////////////////////////////
// Called when TX freq edit box Mode is changed.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::OnTxFreqMode() 
{
CWinPSKDoc* pDoc = GetDocument();
	if(pDoc->m_pSettings->m_TxOffsetMode )
	{
		pDoc->m_pSettings->m_TxOffsetMode = FALSE;
		m_TxFreqModeCtrl.SetWindowText( _T("Tx Freq") );
		m_TxDisp = pDoc->m_pSettings->m_TxFreq;
	}
	else
	{
		pDoc->m_pSettings->m_TxOffsetMode = TRUE;
		m_TxFreqModeCtrl.SetWindowText( _T("TxOffset") );
		m_TxDisp = pDoc->m_pSettings->m_TxFreqOffset;
	}
	UpdateData(FALSE);		//Put changed data into control
}
//////////////////////////////////////////////////////////////////////
// Called when TX freq edit box loses focus.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::OnKillfocusTxfreq() 
{
CWinPSKDoc* pDoc = GetDocument();
	UpdateData(TRUE);		//Get value from control
	if(pDoc->m_pSettings->m_TxOffsetMode )
	{
		if(m_TxDisp < -100 )
			m_TxDisp = -100;
		if(m_TxDisp > 100 )
			m_TxDisp = 100;
		pDoc->m_pSettings->m_TxFreqOffset = m_TxDisp;
	}
	else
	{
		if(m_TxDisp< IndexToFreq( m_pCPlotData->GetXmin()) )
			m_TxDisp = IndexToFreq( m_pCPlotData->GetXmin());
		if(m_TxDisp>IndexToFreq( m_pCPlotData->GetXmax()) )
			m_TxDisp = IndexToFreq( m_pCPlotData->GetXmax() );
		pDoc->m_pSettings->m_TxFreq = m_TxDisp;
	}
	UpdateData(FALSE);		//Put changed data into control
	m_fRxFreqEditing = FALSE;
	m_fTxFreqEditing = FALSE;

}

//////////////////////////////////////////////////////////////////////
// Called when TX freq Spin control is touched.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::OnDeltaposTxfreqspin(NMHDR* pNMHDR, LRESULT* pResult) 
{
CWinPSKDoc* pDoc = GetDocument();
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	m_TxDisp -= pNMUpDown->iDelta;
	if(pDoc->m_pSettings->m_TxOffsetMode )
	{
		if(m_TxDisp < -100 )
			m_TxDisp = -100;
		if(m_TxDisp > 100 )
			m_TxDisp = 100;
		pDoc->m_pSettings->m_TxFreqOffset = m_TxDisp;
	}
	else
	{
		if(m_TxDisp < IndexToFreq( m_pCPlotData->GetXmin()) )
			m_TxDisp = IndexToFreq( m_pCPlotData->GetXmin());
		if(m_TxDisp > IndexToFreq( m_pCPlotData->GetXmax()) )
			m_TxDisp = IndexToFreq( m_pCPlotData->GetXmax());
		pDoc->m_pSettings->m_TxFreq = m_TxDisp;
	}
	UpdateData(FALSE);		//Put changed data into control
	*pResult = 0;
	m_fRxFreqEditing = FALSE;
	m_fTxFreqEditing = FALSE;
}

//////////////////////////////////////////////////////////////////////
// Called when center frequency spin control is clicked.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::OnDeltaposRxFreqspin(NMHDR* pNMHDR, LRESULT* pResult) 
{
CWinPSKDoc* pDoc = GetDocument();
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if( !m_UseAFC)
	{
		m_RxFreq -= pNMUpDown->iDelta;
		if(m_RxFreq < IndexToFreq( m_pCPlotData->GetXmin()) )
			m_RxFreq = IndexToFreq( m_pCPlotData->GetXmin());
		if(m_RxFreq > IndexToFreq( m_pCPlotData->GetXmax()) )
			m_RxFreq = IndexToFreq( m_pCPlotData->GetXmax());
		pDoc->m_pSettings->m_RxFreq = m_RxFreq;
		fnSetRXFrequency( m_RxFreq, 0, RX_CHAN);
		UpdateData(FALSE);		//Put changed data into control
		UpdatePlotInfo(PLOT_CHNG_FREQ);
	}
	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////
//  Capture keystroke messages before they go to the controls so
// that custom functionality can be attached to certain keys.
//////////////////////////////////////////////////////////////////////
BOOL CWinPSKView::PreTranslateMessage(MSG* pMsg) 
{
BOOL fSkipMsg = FALSE;
CWinPSKDoc* pDoc;
	switch (pMsg->message)
	{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			switch (pMsg->wParam)
			{
				case VK_DOWN:
				case VK_LEFT:
				case VK_UP:
				case VK_RIGHT:
					pDoc = GetDocument();
					ASSERT_VALID( pDoc );
					m_fRxFreqEditing = FALSE;
					m_fTxFreqEditing = FALSE;
					switch( pDoc->m_pSettings->m_TabSel )
					{
						case FFT_DISPVIEW:
						case WATERFALL_DISPVIEW:
							if( pMsg->wParam == VK_UP )
							{
								pDoc->m_pSettings->m_UseAFC = m_UseAFC = TRUE;
								UpdateData(FALSE);		//Put value in control
								OnAfc();
							}
							if( pMsg->wParam == VK_DOWN )
							{
								pDoc->m_pSettings->m_UseAFC = m_UseAFC = FALSE;
								UpdateData(FALSE);		//Put value in control
								OnAfc();
							}
							if( pMsg->wParam == VK_RIGHT )
								FindClosestSignal(TRUE);
							if( pMsg->wParam == VK_LEFT )
								FindClosestSignal(FALSE);
							break;
					}
					fSkipMsg = TRUE;
					break;
				case VK_NEXT:
					ChngZoom( -1 );
					break;
				case VK_PRIOR:
					ChngZoom( +1 );
					break;
				case VK_F1:
				case VK_F2:
				case VK_F3:
				case VK_F4:
				case VK_F5:
				case VK_F6:
				case VK_F7:
				case VK_F8:
				case VK_F9:
				case VK_F10:
				case VK_F11:
				case VK_F12:
					if( (pMsg->message != WM_SYSKEYDOWN) ||
									(pMsg->wParam == VK_F10) )
					{
						ProcessFKeys(pMsg->wParam);
					}
					break;
			}
			break;
	}
	if( fSkipMsg )
		return TRUE;
	else
		return CFormView::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////
// Called when Net check box is touched.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::OnNet() 
{
CWinPSKDoc* pDoc = GetDocument();
	ASSERT_VALID( pDoc );
	UpdateData(TRUE);		//Get value from control
	pDoc->m_pSettings->m_TXNet = m_TXNet;
	if(m_TXNet )
	{
		if(pDoc->m_pSettings->m_TxOffsetMode)
		{
			pDoc->m_pSettings->m_TxFreq = m_RxFreq;
			fnSetTXFrequency( m_RxFreq + pDoc->m_pSettings->m_TxFreqOffset );
		}
		else
		{
			pDoc->m_pSettings->m_TxFreq = m_RxFreq;
			m_TxDisp = m_RxFreq;
			fnSetTXFrequency(m_RxFreq);
		}
		UpdateData(FALSE);		//Put changed data into control
	}
}

//////////////////////////////////////////////////////////////////////
// Called when TR Button is pushed.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::OnTrbutton() 
{
	ToggleTR();
}
//////////////////////////////////////////////////////////////////////
// Called when BPSK Box is touched.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::OnBpsk() 
{
	UpdateData(TRUE);		//Get value from control
	GetDocument()->m_pSettings->m_ModType = m_PSKMode;
	INT tmp = 0;
	if(GetDocument()->m_pSettings->m_PSK63Mode)
		tmp |= PSK63MODE;
	fnSetRXPSKMode(BPSK_MODE|tmp, RX_CHAN);
}

//////////////////////////////////////////////////////////////////////
// Called when PSK63 Box is touched.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::OnPsk3163() 
{
	UpdateData(TRUE);		//Get value from control
	GetDocument()->m_pSettings->m_PSK63Mode = m_PSK63;
	INT tmp = 0;
	if(m_PSK63)
		tmp |= PSK63MODE;
	switch(	GetDocument()->m_pSettings->m_ModType)
	{
		case BPSK_MODE:
			fnSetRXPSKMode(BPSK_MODE|tmp, RX_CHAN);
			break;
		case QPSK_MODE:
			fnSetRXPSKMode(QPSK_MODE|tmp, RX_CHAN);
			break;
		case IQPSK_MODE:
			fnSetRXPSKMode(IQPSK_MODE|tmp, RX_CHAN);
			break;
	}
}

//////////////////////////////////////////////////////////////////////
// Called when QPSK Box is touched.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::OnQpsk() 
{
CWinPSKDoc* pDoc = GetDocument();
	ASSERT_VALID( pDoc );
	UpdateData(TRUE);		//Get value from control
	pDoc->m_pSettings->m_ModType = m_PSKMode;
	INT tmp = 0;
	if(GetDocument()->m_pSettings->m_PSK63Mode)
		tmp |= PSK63MODE;
	if(pDoc->m_pSettings->m_UseLSB)
		fnSetRXPSKMode(IQPSK_MODE|tmp, RX_CHAN);
	else
		fnSetRXPSKMode(QPSK_MODE|tmp, RX_CHAN);
}


//////////////////////////////////////////////////////////////////////
// Called when TX Tune Button is pressed.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::OnTxtune() 
{
CWinPSKDoc* pDoc = GetDocument();
	ASSERT_VALID( pDoc );
	if(!m_DLLRunning)
		return;
	switch( m_ProgramState )
	{
		case STATE_RX:
			UpdateStatus();
			OnNet();
			if(pDoc->m_pSettings->m_TxOffsetMode)
			{
				fnSetTXFrequency( pDoc->m_pSettings->m_TxFreq + pDoc->m_pSettings->m_TxFreqOffset );
			}
			else
			{
				fnSetTXFrequency(pDoc->m_pSettings->m_TxFreq);
			}
			if( pDoc->m_SatMode )
				fnStartTX(TUNEID_MODE | SATMODE);
			else
				fnStartTX(TUNEID_MODE);
//			fnStartTX(TUNE_MODE);
			pDoc->m_pRcvEdit->TXStamp(pDoc->m_pSettings->m_EchoTXText,
							TRUE, pDoc->m_pSettings->m_UseLocalTime);
			m_ProgramState = STATE_TUNE;
			break;
		case STATE_TX:
			break;
		case STATE_FINISH:
			fnAbortTX();
			break;
		case STATE_TUNE:
			fnStopTX();
			m_ProgramState = STATE_FINISH;
			break;
	}
}


//////////////////////////////////////////////////////////////////////
//     General Functions for various chores
//////////////////////////////////////////////////////////////////////
void CWinPSKView::FindClosestSignal(BOOL LookRight)
{
CWinPSKDoc* pDoc = GetDocument();
LONG peak;
	if( !m_fRxFreqEditing && !m_fTxFreqEditing && m_UseAFC)
	{
		if(LookRight)
		{
			peak = fnGetClosestPeak(  pDoc->m_pSettings->m_RxFreq,
							IndexToFreq( m_pCPlotData->GetXmax() ));
		}
		else
		{
			peak = fnGetClosestPeak( pDoc->m_pSettings->m_RxFreq,
				IndexToFreq( m_pCPlotData->GetXmin() ));
		}
		fnSetRXFrequency(peak, m_SearchRange, RX_CHAN);
	}
	else	//just inc or dec frequency
	{
		if(LookRight)
			pDoc->m_pSettings->m_RxFreq++;
		else
			pDoc->m_pSettings->m_RxFreq--;
		if(pDoc->m_pSettings->m_RxFreq < IndexToFreq( m_pCPlotData->GetXmin()) )
			pDoc->m_pSettings->m_RxFreq = IndexToFreq( m_pCPlotData->GetXmin());
		if(pDoc->m_pSettings->m_RxFreq > IndexToFreq( m_pCPlotData->GetXmax()) )
			pDoc->m_pSettings->m_RxFreq = IndexToFreq( m_pCPlotData->GetXmax());
		fnSetRXFrequency(pDoc->m_pSettings->m_RxFreq, 0, RX_CHAN);
		m_RxFreq = pDoc->m_pSettings->m_RxFreq;
		UpdateData(FALSE);		//Put changed data into control
		UpdatePlotInfo(PLOT_CHNG_FREQ);
	}
}

//////////////////////////////////////////////////////////////////////
// Called to inc or dec zomm magnification from control or keystrokes.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::ChngZoom(INT delta)
{
INT tzoom;
	CWinPSKDoc* pDoc = GetDocument();
	ASSERT_VALID( pDoc );
// Get current zoom factor
	tzoom = pDoc->m_pSettings->m_PlotViewState[pDoc->m_pSettings->m_TabSel].zoom;
	tzoom += delta;		//change it
	if(tzoom < 1)		//clamp it
		tzoom = 1;
	if(tzoom >4)
		tzoom = 4;
	m_pCPlotData->SetZoom(tzoom,TRUE, pDoc->m_pSettings->m_FreqMin,
									pDoc->m_pSettings->m_FreqMax );
	pDoc->m_pSettings->m_PlotViewState[pDoc->m_pSettings->m_TabSel].zoom = tzoom;
	UpdatePlotInfo(PLOT_CHNG_ZOOM);
	pDoc->m_FFTModeChange = TRUE;
}

//////////////////////////////////////////////////////////////////////
// Converts display buffer index position to Frequency.
//////////////////////////////////////////////////////////////////////
INT CWinPSKView::IndexToFreq(INT Index)
{	//convert display buffer index to frequency Hz.
INT temp = ((Index)*FFT_SAMPLERATE*100)/(FFT_SIZE);	//100x freq
	if( (temp%100) > 50 )		//round to nearest fft index
		temp = temp/100 + 1;
	else
		temp = temp/100;
	return( temp);
}

//////////////////////////////////////////////////////////////////////
// Converts Frequency to display buffer index position.
//////////////////////////////////////////////////////////////////////
INT CWinPSKView::FreqToIndex(INT Freq)
{	//convert Frequency Hz to display buffer index.
INT temp = (Freq*FFT_SIZE*100)/FFT_SAMPLERATE;	//calc actual *100
	if( (temp%100) > 50 )		//round to nearest fft index
		temp = temp/100 + 1;
	else
		temp = temp/100;
	return( temp);
}

//////////////////////////////////////////////////////////////////////
// Updates the display controls depending on the view tab state and
//  where the change request is coming from(src = mouse,zoom,freq edit.)
//////////////////////////////////////////////////////////////////////
void CWinPSKView::UpdatePlotInfo(INT src)
{
CWnd* pWnd;
	CWinPSKDoc* pDoc = GetDocument();
	ASSERT_VALID( pDoc );
BOOL Full =	(pDoc->m_pSettings->m_PlotViewState[pDoc->m_pSettings->m_TabSel].zoom == 1);
	switch( src )
	{
		case PLOT_CHNG_MOUSE:
			switch( pDoc->m_pSettings->m_TabSel )
			{
				case FFT_DISPVIEW:
				case WATERFALL_DISPVIEW:
					m_RxFreq = IndexToFreq( m_pCPlotData->GetPlotCursorIndex() );
					pDoc->m_pSettings->m_RxFreq = m_RxFreq;
					fnSetRXFrequency(m_RxFreq, m_SearchRange, RX_CHAN);
					if(Full)
					{
						m_XMinCStr.Format(_T("%d Hz"), pDoc->m_pSettings->m_FreqMin );
						m_XMaxCStr.Format(_T("%d Hz"), pDoc->m_pSettings->m_FreqMax);
					}
					else
					{
						m_XMinCStr.Format(_T("%d Hz"), IndexToFreq( m_pCPlotData->GetXmin() ));
						m_XMaxCStr.Format(_T("%d Hz"), IndexToFreq( m_pCPlotData->GetXmax() ));
					}
					break;
				default:
					m_XMinCStr = _T("0");
					m_XMaxCStr = _T("0");
					break;
			}
			break;
		case PLOT_CHNG_ZOOM:
			switch( pDoc->m_pSettings->m_TabSel )
			{
				case FFT_DISPVIEW:
				case WATERFALL_DISPVIEW:
					if(Full)
					{
						m_XMinCStr.Format(_T("%d Hz"), pDoc->m_pSettings->m_FreqMin );
						m_XMaxCStr.Format(_T("%d Hz"), pDoc->m_pSettings->m_FreqMax);
					}
					else
					{
						m_XMinCStr.Format(_T("%d Hz"), IndexToFreq( m_pCPlotData->GetXmin() ));
						m_XMaxCStr.Format(_T("%d Hz"), IndexToFreq( m_pCPlotData->GetXmax() ));
					}
					m_ZoomTxt.Format(_T("Zoom X%d"),pDoc->m_pSettings->m_PlotViewState[pDoc->m_pSettings->m_TabSel].zoom );
					pWnd = GetDlgItem( IDC_ZOOM );
					pWnd->EnableWindow(TRUE);
					pWnd = GetDlgItem( IDC_SLOWAVE );
					pWnd->EnableWindow(TRUE);
					break;
				case INPUT_DISPVIEW:
					m_XMinCStr = _T("0 Sec");
					m_XMaxCStr.Format(_T("%3.2f Sec"), (float)m_PlotWidth/8000.0);
					m_ZoomTxt = "";
					pWnd = GetDlgItem( IDC_ZOOM );
					pWnd->EnableWindow(FALSE);
					pWnd = GetDlgItem( IDC_SLOWAVE );
					pWnd->EnableWindow(FALSE);
					break;
				default:
					m_XMinCStr = _T("");
					m_XMaxCStr = _T("");
					m_ZoomTxt = _T("");
					pWnd = GetDlgItem( IDC_ZOOM );
					pWnd->EnableWindow(FALSE);
					pWnd = GetDlgItem( IDC_SLOWAVE );
					pWnd->EnableWindow(FALSE);
					break;
			}
			break;
		case PLOT_CHNG_FREQ:
			switch( pDoc->m_pSettings->m_TabSel )
			{
				case FFT_DISPVIEW:
				case WATERFALL_DISPVIEW:
					m_pCPlotData->SetPlotCursorIndex(FreqToIndex(pDoc->m_pSettings->m_RxFreq) );
					m_pCPlotData->SetZoom( pDoc->m_pSettings->
							m_PlotViewState[pDoc->m_pSettings->m_TabSel].zoom,FALSE,
							pDoc->m_pSettings->m_FreqMin,
							pDoc->m_pSettings->m_FreqMax );
				break;
				default:
					break;
			}
			break;
	}
	UpdateData(FALSE);		//Put changed data into control
}


//////////////////////////////////////////////////////////////////////
// Called to process a Function key press.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::ProcessFKeys(WPARAM wParam)
{
	switch(wParam)
	{
		case VK_F1:
			OnM1button();
			break;
		case VK_F2:
			OnM2button();
			break;
		case VK_F3:
			OnM3button();
			break;
		case VK_F4:
			OnM4button();
			break;
		case VK_F5:
			OnM5button();
			break;
		case VK_F6:
			OnM6button();
			break;
		case VK_F7:
			OnM7button();
			break;
		case VK_F8:
			OnM8button();
			break;
		case VK_F9:
			OnM9button();
			break;
		case VK_F10:
			OnM10button();
			break;
		case VK_F11:
			OnIncfnset();
			break;
		case VK_F12:
			ToggleTR();
			break;
	}
}

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Called to process an RX/TX button command
//////////////////////////////////////////////////////////////////////
void CWinPSKView::ToggleTR()
{
CWinPSKDoc* pDoc = GetDocument();
INT mode;
	ASSERT_VALID( pDoc );
	if(!m_DLLRunning)
		return;
	switch( m_ProgramState )
	{
		case STATE_RX:
			OnNet();
			pDoc->m_pRcvEdit->CancelSelection();
			pDoc->m_pRcvEdit->TXStamp(pDoc->m_pSettings->m_EchoTXText,
								TRUE, pDoc->m_pSettings->m_UseLocalTime);
			if(pDoc->m_pSettings->m_TxOffsetMode)
			{
				fnSetTXFrequency( pDoc->m_pSettings->m_TxFreq + pDoc->m_pSettings->m_TxFreqOffset );
			}
			else
			{
				fnSetTXFrequency(pDoc->m_pSettings->m_TxFreq);
			}
			mode = pDoc->m_pSettings->m_ModType;
			if( (mode == QPSK_MODE) && pDoc->m_pSettings->m_UseLSB)
				mode = IQPSK_MODE;
			if( pDoc->m_SatMode )
				mode |= SATMODE;
			if( pDoc->m_pSettings->m_PSK63Mode )
				mode |= (1<<3);
			fnStartTX(mode);
//			m_ProgramState = STATE_TX;
			break;
		case STATE_TX:
//			pDoc->m_pXmitEdit->ClearSentText();
			fnStopTX();
//			m_ProgramState = STATE_FINISH;
			break;
		case STATE_FINISH:
			fnAbortTX();
			pDoc->m_pXmitEdit->ClearAll();
			break;
		case STATE_TUNE:
			fnStopTX();
//			m_ProgramState = STATE_FINISH;
			break;
	}
	pDoc->m_pXmitEdit->SetFocus();
}


//////////////////////////////////////////////////////////////////////
// Service timer for status updates.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent == m_TimerID )
		UpdateStatus();
	CFormView::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////
// Called to update the button and other status states.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::UpdateStatus()
{
CWinPSKDoc* pDoc;
static BOOL tog = FALSE;
	pDoc = GetDocument();
	ASSERT_VALID( pDoc );
	if(pDoc->m_MacroStartTX) //if macro needs to turn on tx state
	{
		if( m_ProgramState == STATE_RX)
			ToggleTR();
		pDoc->m_MacroStartTX = FALSE;
	}
	switch( m_ProgramState )
	{
		case STATE_TUNE:
			if(tog)
				m_TRCntrl.SetIcon(AfxGetApp()->LoadIcon(IDI_TUNE));
			else
				m_TRCntrl.SetIcon(AfxGetApp()->LoadIcon(IDI_TROFF));
			break;
		case STATE_TX:
			if(tog)
				m_TRCntrl.SetIcon(AfxGetApp()->LoadIcon(IDI_TRXMIT));
			else
				m_TRCntrl.SetIcon(AfxGetApp()->LoadIcon(IDI_TROFF));
			break;
		case STATE_FINISH:
			if(tog)
				m_TRCntrl.SetIcon(AfxGetApp()->LoadIcon(IDI_TXFINISH));
			else
				m_TRCntrl.SetIcon(AfxGetApp()->LoadIcon(IDI_TROFF));
			break;
	}
	tog = !tog;
	if(m_IMDDispDelay > 0)
	{
		m_IMDDispDelay--;
	}
	else
	{
		m_IMDDispDelay = 0;
		pDoc->m_IMDRdy = FALSE;
	}

}


////////////////////////////////////////////////////////////////////
////////////////////   ClipBoard Functionality    //////////////////
////////////////////////////////////////////////////////////////////
//  The receive window is read only so only copy operations are allowed
//  The transmit window can be pasted into but not copied from.
////////////////////////////////////////////////////////////////////
void CWinPSKView::OnEditCopy() 
{
CWinPSKDoc* pDoc = GetDocument();
CWnd* pWnd = GetFocus();
	if( pWnd == pDoc->m_pTheirCallEdit)
		pDoc->m_pTheirCallEdit->Copy();
	else
		if( pWnd == pDoc->m_pRcvEdit )
			pDoc->m_pRcvEdit->CopyClip();
}

void CWinPSKView::OnEditPaste() 
{
CWinPSKDoc* pDoc = GetDocument();
CWnd* pWnd = GetFocus();
	if( pWnd == pDoc->m_pTheirCallEdit)
		pDoc->m_pTheirCallEdit->PasteClip();
	else
		if( pWnd == pDoc->m_pXmitEdit )
		{
			pDoc->m_pXmitEdit->Paste();
			pDoc->m_pXmitEdit->SetFocus();
		}
}

void CWinPSKView::OnEditClear() 
{
CWinPSKDoc* pDoc = GetDocument();
CWnd* pWnd = GetFocus();
	if( pWnd == pDoc->m_pTheirCallEdit)
		pDoc->m_pTheirCallEdit->Clear();
	else
		if( pWnd == pDoc->m_pXmitEdit )
			pDoc->m_pXmitEdit->Clear();
}

void CWinPSKView::OnEditCut() 
{
CWinPSKDoc* pDoc = GetDocument();
CWnd* pWnd = GetFocus();
	if( pWnd == pDoc->m_pTheirCallEdit)
		pDoc->m_pTheirCallEdit->Cut();
	else
		if( pWnd == pDoc->m_pXmitEdit )
			pDoc->m_pXmitEdit->Cut();
}

void CWinPSKView::OnEditUndo() 
{
CWinPSKDoc* pDoc = GetDocument();
CWnd* pWnd = GetFocus();
	if( pWnd == pDoc->m_pTheirCallEdit)
		pDoc->m_pTheirCallEdit->Undo();
	else
		if( pWnd == pDoc->m_pXmitEdit )
			pDoc->m_pXmitEdit->Undo();
}
///////////////////  Indicate what clipboard operations are OK
void CWinPSKView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
CWinPSKDoc* pDoc = GetDocument();
CWnd* pWnd = GetFocus();
LONG s,e;
	if( pWnd == pDoc->m_pTheirCallEdit)
	{
		pDoc->m_pTheirCallEdit->GetSel(s,e);
		pCmdUI->Enable( e > s );
	}
	else
		if( pWnd == pDoc->m_pRcvEdit )
			pCmdUI->Enable( pDoc->m_pRcvEdit->OKCopy() );
		else
			pCmdUI->Enable( FALSE );
}

void CWinPSKView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
CWinPSKDoc* pDoc = GetDocument();
CWnd* pWnd = GetFocus();
LONG s,e;
	if( pWnd == pDoc->m_pTheirCallEdit)
	{
		pDoc->m_pTheirCallEdit->GetSel(s,e);
		pCmdUI->Enable( e > s );
	}
	else
		if( pWnd == pDoc->m_pXmitEdit )
			pCmdUI->Enable( pDoc->m_pXmitEdit->OKCutClip() );
		else
			pCmdUI->Enable( FALSE );
}

void CWinPSKView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
CWinPSKDoc* pDoc = GetDocument();
CWnd* pWnd = GetFocus();
	if( pWnd != pDoc->m_pRcvEdit)
		pCmdUI->Enable( ::IsClipboardFormatAvailable(CF_TEXT) );
	else
		pCmdUI->Enable( FALSE );
}

void CWinPSKView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
CWinPSKDoc* pDoc = GetDocument();
CWnd* pWnd = GetFocus();
	if( pWnd == pDoc->m_pTheirCallEdit)
		pCmdUI->Enable( pDoc->m_pTheirCallEdit->CanUndo() );
	else
		if( pWnd == pDoc->m_pXmitEdit )
			pCmdUI->Enable( pDoc->m_pXmitEdit->CanUndo() );	
		else
			pCmdUI->Enable( FALSE );
}


void CWinPSKView::OnUpdateEditClear(CCmdUI* pCmdUI) 
{
CWinPSKDoc* pDoc = GetDocument();
CWnd* pWnd = GetFocus();
LONG s,e;
	if( pWnd == pDoc->m_pTheirCallEdit)
	{
		pDoc->m_pTheirCallEdit->GetSel(s,e);
		pCmdUI->Enable( e > s );
	}
	else
		if( pWnd == pDoc->m_pXmitEdit )
			pCmdUI->Enable( pDoc->m_pXmitEdit->OKClearSel() );
		else
			pCmdUI->Enable( FALSE );

}

////////////////////////////////////////////////////////////////////
////////////////   Macro Key Functionality    //////////////////
////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Called when Right mouse Button is clicked on a macro button.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	switch( pWnd->GetDlgCtrlID() )
	{
		case IDC_M1BUTTON:
			MacroDlg(0);
			break;
		case IDC_M2BUTTON:
			MacroDlg(1);
			break;
		case IDC_M3BUTTON:
			MacroDlg(2);
			break;
		case IDC_M4BUTTON:
			MacroDlg(3);
			break;
		case IDC_M5BUTTON:
			MacroDlg(4);
			break;
		case IDC_M6BUTTON:
			MacroDlg(5);
			break;
		case IDC_M7BUTTON:
			MacroDlg(6);
			break;
		case IDC_M8BUTTON:
			MacroDlg(7);
			break;
		case IDC_M9BUTTON:
			MacroDlg(8);
			break;
		case IDC_M10BUTTON:
			MacroDlg(9);
			break;
		default:
			break;
	}
}


//////////////////////////////////////////////////////////////////////
// Called to bring up a macro setup dialog.
//////////////////////////////////////////////////////////////////////
void CWinPSKView::MacroDlg(INT key)
{
CWinPSKDoc* pDoc = GetDocument();
CMacroDlg dlg;
INT index = key + pDoc->m_pSettings->m_MacroSet*NUM_MACROKEYS;
	dlg.m_MacroKey = key;
	dlg.m_MacroSet = pDoc->m_pSettings->m_MacroSet;
	dlg.m_sMacroTitle = pDoc->m_pSettings->m_saMacroTitle[index];
	dlg.m_sData = pDoc->m_pSettings->m_saMacroData[index];
	if (dlg.DoModal() == IDOK)
	{
		CString str;
		str = dlg.m_sData.Left(200);	//limit to 200 characters
		dlg.m_sData = str;
		pDoc->m_pSettings->m_saMacroData.SetAt(index,dlg.m_sData);
		pDoc->m_pSettings->m_saMacroTitle.SetAt(index,dlg.m_sMacroTitle);
		str.Format(_T("F%d="), key+1);
		m_MTitleCtrl[key].SetWindowText( str+dlg.m_sMacroTitle);
		UpdateData(FALSE);
	}
}

///////////////////////////////////////////////////////////////////////
// Routines called when macro buttons are left clicked to execute
///////////////////////////////////////////////////////////////////////
void CWinPSKView::OnM1button() 
{
CWinPSKDoc* pDoc = GetDocument();
	pDoc->m_pXmitEdit->InsertCString( pDoc->m_pSettings->m_saMacroData[
					0+pDoc->m_pSettings->m_MacroSet*NUM_MACROKEYS] );
}

void CWinPSKView::OnM2button() 
{
CWinPSKDoc* pDoc = GetDocument();
	pDoc->m_pXmitEdit->InsertCString( pDoc->m_pSettings->m_saMacroData[
					1+pDoc->m_pSettings->m_MacroSet*NUM_MACROKEYS] );
}

void CWinPSKView::OnM3button() 
{
CWinPSKDoc* pDoc = GetDocument();
	pDoc->m_pXmitEdit->InsertCString( pDoc->m_pSettings->m_saMacroData[
					2+pDoc->m_pSettings->m_MacroSet*NUM_MACROKEYS] );
}

void CWinPSKView::OnM4button() 
{
CWinPSKDoc* pDoc = GetDocument();
	pDoc->m_pXmitEdit->InsertCString( pDoc->m_pSettings->m_saMacroData[
					3+pDoc->m_pSettings->m_MacroSet*NUM_MACROKEYS] );
}

void CWinPSKView::OnM5button() 
{
CWinPSKDoc* pDoc = GetDocument();
	pDoc->m_pXmitEdit->InsertCString( pDoc->m_pSettings->m_saMacroData[
					4+pDoc->m_pSettings->m_MacroSet*NUM_MACROKEYS] );
}


void CWinPSKView::OnM6button() 
{
CWinPSKDoc* pDoc = GetDocument();
	pDoc->m_pXmitEdit->InsertCString( pDoc->m_pSettings->m_saMacroData[
					5+pDoc->m_pSettings->m_MacroSet*NUM_MACROKEYS] );
	
}

void CWinPSKView::OnM7button() 
{
CWinPSKDoc* pDoc = GetDocument();
	pDoc->m_pXmitEdit->InsertCString( pDoc->m_pSettings->m_saMacroData[
					6+pDoc->m_pSettings->m_MacroSet*NUM_MACROKEYS] );
	
}

void CWinPSKView::OnM8button() 
{
CWinPSKDoc* pDoc = GetDocument();
	pDoc->m_pXmitEdit->InsertCString( pDoc->m_pSettings->m_saMacroData[
					7+pDoc->m_pSettings->m_MacroSet*NUM_MACROKEYS] );
	
}

void CWinPSKView::OnM9button() 
{
CWinPSKDoc* pDoc = GetDocument();
	pDoc->m_pXmitEdit->InsertCString( pDoc->m_pSettings->m_saMacroData[
					8+pDoc->m_pSettings->m_MacroSet*NUM_MACROKEYS] );
	
}

void CWinPSKView::OnM10button() 
{
CWinPSKDoc* pDoc = GetDocument();
	pDoc->m_pXmitEdit->InsertCString( pDoc->m_pSettings->m_saMacroData[
					9+pDoc->m_pSettings->m_MacroSet*NUM_MACROKEYS] );
	
}


void CWinPSKView::OnClrqso() 
{
CWinPSKDoc* pDoc = GetDocument();
	pDoc->m_pTextGrabEdit->ClearAll();
	pDoc->m_pTheirNameEdit->ClearAll();
	pDoc->m_pTheirCallEdit->ClearAll();
}

BOOL CWinPSKView::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
UINT nID = pNMHDR->idFrom;
CWinPSKDoc* pDoc = GetDocument();
CString str;
CString str2;
    if (pTTT->uFlags & TTF_IDISHWND)
    {
        nID = ::GetDlgCtrlID((HWND)nID);
        if(!nID)
			return(FALSE);
        pTTT->hinst =NULL;
		switch( nID )		//convert to macro key index 0-9
		{
			case IDC_M1BUTTON:
				nID = 0;
				break;
			case IDC_M2BUTTON:
				nID = 1;
				break;
			case IDC_M3BUTTON:
				nID = 2;
				break;
			case IDC_M4BUTTON:
				nID = 3;
				break;
			case IDC_M5BUTTON:
				nID = 4;
				break;
			case IDC_M6BUTTON:
				nID = 5;
				break;
			case IDC_M7BUTTON:
				nID = 6;
				break;
			case IDC_M8BUTTON:
				nID = 7;
				break;
			case IDC_M9BUTTON:
				nID = 8;
				break;
			case IDC_M10BUTTON:
				nID = 9;
				break;
			case IDC_SQUELCH:
				return(FALSE);
				break;
			default:
				return(FALSE);
				break;
		}
		str2 = pDoc->m_pSettings->m_saMacroData[nID +
					pDoc->m_pSettings->m_MacroSet*NUM_MACROKEYS];
		str = str2.Left(79);		//just get first 79 characters
		strcpy( pTTT->szText, (PCHAR)(LPCTSTR)str );
        return(TRUE);
    }    
	return(FALSE);
}


void CWinPSKView::OnDecfnset() 
{
CWinPSKDoc* pDoc = GetDocument();
CString str;
	if( --pDoc->m_pSettings->m_MacroSet < 0 )
	{
		pDoc->m_pSettings->m_MacroSet = NUM_MACROSETS-1;
	}
	str.Format(_T("Macro Set = %d"), pDoc->m_pSettings->m_MacroSet+1);
	m_MacroSetTxtCtrl.SetWindowText( str );
	for( INT i=0; i<NUM_MACROKEYS; i++)
	{
		str.Format(_T("F%d="), i+1);
		str = str + pDoc->m_pSettings->m_saMacroTitle[i +
						pDoc->m_pSettings->m_MacroSet*NUM_MACROKEYS];
		m_MTitleCtrl[i].SetWindowText(str);
	}
}

void CWinPSKView::OnIncfnset() 
{
CWinPSKDoc* pDoc = GetDocument();
CString str;
	if( ++pDoc->m_pSettings->m_MacroSet >= NUM_MACROSETS )
	{
		pDoc->m_pSettings->m_MacroSet = 0;
	}
	str.Format(_T("Macro Set = %d"), pDoc->m_pSettings->m_MacroSet+1);
	m_MacroSetTxtCtrl.SetWindowText( str );
	for( INT i=0; i<NUM_MACROKEYS; i++)
	{
		str.Format(_T("F%d="), i+1);
		str = str + pDoc->m_pSettings->m_saMacroTitle[i +
						pDoc->m_pSettings->m_MacroSet*NUM_MACROKEYS];
		m_MTitleCtrl[i].SetWindowText(str);
	}	
}

void CWinPSKView::OnSethome() 
{
CWinPSKDoc* pDoc = GetDocument();
CString str;
	pDoc->m_pSettings->m_MacroSet = 0;
	str.Format(_T("Macro Set = %d"), pDoc->m_pSettings->m_MacroSet+1);
	m_MacroSetTxtCtrl.SetWindowText( str );
	for( INT i=0; i<NUM_MACROKEYS; i++)
	{
		str.Format(_T("F%d="), i+1);
		str = str + pDoc->m_pSettings->m_saMacroTitle[i +
						pDoc->m_pSettings->m_MacroSet*NUM_MACROKEYS];
		m_MTitleCtrl[i].SetWindowText(str);
	}		
}

void CWinPSKView::OnRun() 
{
CWinPSKDoc* pDoc = GetDocument();
	if(m_DLLRunning)
		StopDLL();
	else
		StartDLL(pDoc);
}

//Called by squelch control when someone clicks on the threshold
void CWinPSKView::SquelchThreshCallback(INT threshold)
{
CWinPSKDoc* pDoc = GetDocument();
	ASSERT(pDoc);
	pDoc->m_pSettings->m_SQThreshold = threshold;
	fnSetSquelchThreshold(pDoc->m_pSettings->m_SQThreshold,
				pDoc->m_pSettings->m_SquelchSpeed, RX_CHAN);
}

