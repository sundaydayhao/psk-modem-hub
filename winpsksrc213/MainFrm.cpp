// MainFrm.cpp : implementation of the CMainFrame class
//
//
#include "stdafx.h"
#include "WinPSK.h"
#include "WinPSKDoc.h"
#include "RcvView.h"
#include "XmitView.h"
#include "SetDlg.h"
#include "WaveDlg.h"
#include "WinPSKView.h"
#include "MainFrm.h"
#include "Htmlhelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define FILE_TEXT_LIMIT 10000

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_INITMENUPOPUP()
	ON_WM_MENUSELECT()
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS, OnUpdateStatus)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(IDM_RCVBACKCOLOR, OnRcvbackcolor)
	ON_COMMAND(IDM_XMITBACKCOLOR, OnXmitbackcolor)
	ON_COMMAND(IDM_RCVFONT, OnRcvfont)
	ON_COMMAND(IDM_XMITFONT, OnXmitfont)
	ON_COMMAND(IDM_XMITSENTFONT, OnXmitsentfont)
	ON_WM_CLOSE()
	ON_COMMAND(IDM_CLEARRCV, OnClearrcv)
	ON_COMMAND(IDM_CLEARXMIT, OnClearxmit)
	ON_COMMAND(ID_SETUP, OnSetup)
	ON_COMMAND(ID_RXLVLSETUP, OnRxlvlsetup)
	ON_COMMAND(ID_TXLVLSETUP, OnTxlvlsetup)
	ON_COMMAND(IDM_TXTUNE, OnTxtune)
	ON_COMMAND(IDM_SENDFILE, OnSendfile)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STAYONTOP, OnUpdateViewStayontop)
	ON_COMMAND(ID_VIEW_STAYONTOP, OnViewStayontop)
	ON_COMMAND(IDM_SELWAVEFILES, OnSelwavefiles)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CFrameWnd::OnHelpFinder)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	IDS_STATUSBAR1,
	IDS_STATUSBAR2,
	IDS_STATUSBAR3,
	ID_SEPARATOR,
	ID_INDICATOR_STATUS
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_wp.length = 0;
	m_StayOnTop = FALSE;
}

CMainFrame::~CMainFrame()
{
}

///////////////////////////////////////////////////////////////
// create splitter views
///////////////////////////////////////////////////////////////
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	VERIFY(m_WndSplitter.CreateStatic( this,3,1));		//3 rows, 1 column
	VERIFY(m_WndSplitter.CreateView(0,0,
			RUNTIME_CLASS(CRcvView),CSize(0,0), pContext));
	
	VERIFY(m_WndSplitter.CreateView(1,0,
			RUNTIME_CLASS(CXmitView),CSize(0,0), pContext));
	
	VERIFY(m_WndSplitter.CreateView(2,0,
			RUNTIME_CLASS(CWinPSKView),CSize(0,0), pContext));
	return TRUE;
}

///////////////////////////////////////////////////////////////
// create the outer frame stuff
///////////////////////////////////////////////////////////////
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
//create status and toolbar	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD /*| WS_VISIBLE*/ | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	//  Delete these three lines if you don't want the toolbar to
	//  be dockable
//	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
//	EnableDocking(CBRS_ALIGN_ANY);
//	DockControlBar(&m_wndToolBar);

	CWinPSKView* pWnd = (CWinPSKView*)m_WndSplitter.GetPane(2,0);
	ASSERT_VALID( pWnd );
	CSize size = pWnd->GetTotalSize();	//get size of FormView window
	m_wp.length = sizeof(WINDOWPLACEMENT);
// get screen size settings if they are saved in registry
	CRect rc;
	GetClientRect( rc );
	LoadBarState("ControlBars\\State");	//restore status/toolbar states
	m_WndSplitter.SetColumnInfo(0,size.cx,10);
	if( LoadWindowPlacement(&m_wp, &m_Pane0Height, &m_Pane1Height, &m_Pane2Height))
	{
		m_WndSplitter.SetRowInfo(0,m_Pane0Height,0);
		m_WndSplitter.SetRowInfo(1,m_Pane1Height,0);
		m_WndSplitter.SetRowInfo(2,m_Pane2Height,0);
	}
	else
	{
		// set default splitter sizes if none are loaded from the registry
		m_WndSplitter.SetRowInfo(2,size.cy,10);
		m_WndSplitter.SetRowInfo(0,(rc.bottom-size.cy)/2,10);
		m_WndSplitter.SetRowInfo(1,(rc.bottom-size.cy)/3,10);
		m_wp.length = 0;
		m_StayOnTop = FALSE;
	}
	SetActiveView( pWnd, TRUE);
	return 0;
}

void CMainFrame::ActivateFrame(int nCmdShow) 
{
	if(	m_wp.length != 0)	//if structure has been filled in
	{
		nCmdShow= m_wp.showCmd;
		AfxGetApp()->m_nCmdShow = nCmdShow;
		SetWindowPlacement(&m_wp);
	}
	else
		AfxGetApp()->m_nCmdShow = nCmdShow;
	CFrameWnd::ActivateFrame(nCmdShow);
}

///////////////////////////////////////////////////////////////
// get registry screen size settings
///////////////////////////////////////////////////////////////
BOOL CMainFrame::LoadWindowPlacement(LPWINDOWPLACEMENT pwp, INT* P0Height, INT* P1Height, INT* P2Height)
{
	CString strBuffer = AfxGetApp ()->GetProfileString ("WindowPos", "WindowPos");
	if (strBuffer.IsEmpty ())
		return FALSE;
	int cRead = _stscanf (strBuffer, "%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i",
			&pwp->flags, &pwp->showCmd,
			&pwp->ptMinPosition.x, &pwp->ptMinPosition.y,
			&pwp->ptMaxPosition.x, &pwp->ptMaxPosition.y,
			&pwp->rcNormalPosition.left, &pwp->rcNormalPosition.top,
			&pwp->rcNormalPosition.right, &pwp->rcNormalPosition.bottom,
			P0Height, P1Height, P2Height);

	if(pwp->flags & 0x8000)
		m_StayOnTop = TRUE;
	else
		m_StayOnTop = FALSE;
	pwp->flags &= ~0x8000;

	if (cRead != 13)
		return FALSE;
	return TRUE;
}

///////////////////////////////////////////////////////////////
// save screen size settings to registry
///////////////////////////////////////////////////////////////
void CMainFrame::SaveWindowPlacement(LPWINDOWPLACEMENT pwp, INT P0Height, INT P1Height, INT P2Height)
{
	// SaveWindowPlacement
	if(m_StayOnTop)
		pwp->flags |= 0x8000;
	else
		pwp->flags &= ~0x8000;
	CString strBuffer;
	strBuffer.Format ("%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i",
		pwp->flags, pwp->showCmd,
		pwp->ptMinPosition.x, pwp->ptMinPosition.y,
		pwp->ptMaxPosition.x, pwp->ptMaxPosition.y,
		pwp->rcNormalPosition.left, pwp->rcNormalPosition.top,
		pwp->rcNormalPosition.right, pwp->rcNormalPosition.bottom,
		P0Height, P1Height, P2Height);

	AfxGetApp()->WriteProfileString("WindowPos", "WindowPos", strBuffer);
}

///////////////////////////////////////////////////////////////
// save stuff if closing program
///////////////////////////////////////////////////////////////
void CMainFrame::OnClose() 
{
INT x;
	CRect rc;
	m_wndStatusBar.GetItemRect( 1, rc);
	WINDOWPLACEMENT wp;
	wp.length = sizeof(WINDOWPLACEMENT);
	if (GetWindowPlacement(&wp) )
	{
		m_WndSplitter.GetRowInfo( 0, m_Pane0Height, x );
		m_WndSplitter.GetRowInfo( 1, m_Pane1Height, x );
		m_WndSplitter.GetRowInfo( 2, m_Pane2Height, x );
		if( IsZoomed() )
			wp.flags |= WPF_RESTORETOMAXIMIZED;
		SaveWindowPlacement( &wp, m_Pane0Height, m_Pane1Height, m_Pane2Height );
	}

	SaveBarState("ControlBars\\State");
	CFrameWnd::OnClose();
}

void CMainFrame::OnUpdateViewStayontop(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_StayOnTop);
}

void CMainFrame::OnViewStayontop() 
{
	if(	m_StayOnTop )
		m_StayOnTop = FALSE;
	else
		m_StayOnTop = TRUE;
	StayOnTop();
}

void CMainFrame::StayOnTop()
{
CWnd* pWnd = AfxGetMainWnd();
	ASSERT_VALID(pWnd);
	pWnd->SetWindowPos( (m_StayOnTop) ? &wndTopMost : &wndNoTopMost,
						0,0,0,0, SWP_NOMOVE|SWP_NOSIZE);
}

///////////////////////////////////////////////////////////////
// keep default title off the top system bar
///////////////////////////////////////////////////////////////
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style &= ~FWS_ADDTOTITLE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers for menu commands/actions

void CMainFrame::OnClearrcv() 
{
	CWinPSKDoc* pDoc = (CWinPSKDoc*)CMainFrame::GetActiveDocument();
	ASSERT_VALID(pDoc);
	pDoc->m_pRcvEdit->ClearAll();
}

///////////////////////////////////////////////////////////////
void CMainFrame::OnClearxmit() 
{
	CWinPSKDoc* pDoc = (CWinPSKDoc*)CMainFrame::GetActiveDocument();
	ASSERT_VALID(pDoc);
	fnAbortTX();
	pDoc->m_pXmitEdit->ClearAll();
}

///////////////////////////////////////////////////////////////
void CMainFrame::OnTxtune() 
{
CWinPSKView* pView = (CWinPSKView*)GetActiveView();
	ASSERT_VALID(pView);
	pView->OnTxtune();	
}
///////////////////////////////////////////////////////////////
void CMainFrame::OnRcvbackcolor() 
{
CWinPSKDoc* pDoc = (CWinPSKDoc*)CMainFrame::GetActiveDocument();
	ASSERT_VALID(pDoc);
	CColorDialog dlgColor(pDoc->m_pSettings->m_crRcvBackColor, CC_RGBINIT);
	if (dlgColor.DoModal() == IDOK)
	{
		pDoc->m_pSettings->m_crRcvBackColor = dlgColor.GetColor();
		pDoc->m_pRcvEdit->SetBackgroundColor( FALSE, pDoc->m_pSettings->m_crRcvBackColor );
	}
}
///////////////////////////////////////////////////////////////
void CMainFrame::OnRcvfont() 
{
CWinPSKDoc* pDoc = (CWinPSKDoc*)CMainFrame::GetActiveDocument();
	ASSERT_VALID(pDoc);
	CClientDC dc(NULL);
	LOGFONT lf = pDoc->m_pSettings->m_lfRcvFont;
	lf.lfHeight = -::MulDiv(-lf.lfHeight, dc.GetDeviceCaps(LOGPIXELSY), 72);
	CFontDialog dlg(&lf);
	dlg.m_cf.rgbColors = pDoc->m_pSettings->m_crRcvTextColor;
	if (dlg.DoModal() == IDOK)
	{
		pDoc->m_pSettings->m_crRcvTextColor = dlg.GetColor();
		lf.lfHeight = -::MulDiv(-lf.lfHeight, 72, dc.GetDeviceCaps(LOGPIXELSY));
		pDoc->m_pSettings->m_lfRcvFont = lf;
		pDoc->m_pRcvEdit->SetCharFormat(pDoc->m_pSettings->m_lfRcvFont,
			pDoc->m_pSettings->m_lfXmitFont,
			pDoc->m_pSettings->m_crRcvTextColor,
			pDoc->m_pSettings->m_crXmitSentTextColor,
			pDoc->m_pSettings->m_crRcvBackColor	);
	}
}

///////////////////////////////////////////////////////////////
void CMainFrame::OnXmitfont() 
{
CWinPSKDoc* pDoc = (CWinPSKDoc*)CMainFrame::GetActiveDocument();
	ASSERT_VALID(pDoc);
	CClientDC dc(NULL);
	LOGFONT lf = pDoc->m_pSettings->m_lfXmitFont;
	lf.lfHeight = -::MulDiv(-lf.lfHeight, dc.GetDeviceCaps(LOGPIXELSY), 72);
	CFontDialog dlg(&lf);
	dlg.m_cf.rgbColors = pDoc->m_pSettings->m_crXmitTextColor;
	if (dlg.DoModal() == IDOK)
	{
		lf.lfHeight = -::MulDiv(-lf.lfHeight, 72, dc.GetDeviceCaps(LOGPIXELSY));
		pDoc->m_pSettings->m_crXmitTextColor = dlg.GetColor();
		pDoc->m_pSettings->m_lfXmitFont = lf;
		pDoc->m_pXmitEdit->SetCharFormat(pDoc->m_pSettings->m_lfXmitFont,
			pDoc->m_pSettings->m_lfXmitSentFont,
			pDoc->m_pSettings->m_crXmitTextColor,
			pDoc->m_pSettings->m_crXmitSentTextColor,
			pDoc->m_pSettings->m_crXmitBackColor);
	}
}

///////////////////////////////////////////////////////////////
void CMainFrame::OnXmitsentfont() 
{
CWinPSKDoc* pDoc = (CWinPSKDoc*)CMainFrame::GetActiveDocument();
	ASSERT_VALID(pDoc);
	CClientDC dc(NULL);
	LOGFONT lf = pDoc->m_pSettings->m_lfXmitSentFont;
	lf.lfHeight = -::MulDiv(-lf.lfHeight, dc.GetDeviceCaps(LOGPIXELSY), 72);
	CFontDialog dlg(&lf);
	dlg.m_cf.rgbColors = pDoc->m_pSettings->m_crXmitSentTextColor;
	if (dlg.DoModal() == IDOK)
	{
		lf.lfHeight = -::MulDiv(-lf.lfHeight, 72, dc.GetDeviceCaps(LOGPIXELSY));
		pDoc->m_pSettings->m_crXmitSentTextColor = dlg.GetColor();
		pDoc->m_pSettings->m_lfXmitSentFont = lf;
		pDoc->m_pXmitEdit->SetCharFormat(pDoc->m_pSettings->m_lfXmitFont,
			pDoc->m_pSettings->m_lfXmitSentFont,
			pDoc->m_pSettings->m_crXmitTextColor,
			pDoc->m_pSettings->m_crXmitSentTextColor,
			pDoc->m_pSettings->m_crXmitBackColor);
		pDoc->m_pRcvEdit->SetCharFormat(pDoc->m_pSettings->m_lfRcvFont,
			pDoc->m_pSettings->m_lfXmitFont,
			pDoc->m_pSettings->m_crRcvTextColor,
			pDoc->m_pSettings->m_crXmitSentTextColor,
			pDoc->m_pSettings->m_crRcvBackColor	);
	}
	
}

///////////////////////////////////////////////////////////////
void CMainFrame::OnXmitbackcolor() 
{
	CWinPSKDoc* pDoc = (CWinPSKDoc*)CMainFrame::GetActiveDocument();
	ASSERT_VALID(pDoc);
	CColorDialog dlgColor(pDoc->m_pSettings->m_crXmitBackColor, CC_RGBINIT);
	if (dlgColor.DoModal() == IDOK)
	{
		pDoc->m_pSettings->m_crXmitBackColor = dlgColor.GetColor();
		pDoc->m_pXmitEdit->SetBackgroundColor( FALSE, pDoc->m_pSettings->m_crXmitBackColor );
	}
	
}

///////////////////////////////////////////////////////////////
// called during idle time to update the status bar
///////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateStatus(CCmdUI* pCmdUI)
{
CString sBuf;
UINT nID, nStyle;
int nWidth, nIndex;
CSize szExtent;
	CClientDC dc(&m_wndStatusBar);
	CWinPSKDoc* pDoc = (CWinPSKDoc*)CMainFrame::GetActiveDocument();
	ASSERT_VALID(pDoc);
	CFont* pOldFont = dc.SelectObject(m_wndStatusBar.GetFont());
// Update status pane 1

	sBuf = _T(" No IMD Reading ");
	szExtent = dc.GetTextExtent(sBuf, sBuf.GetLength() );
	if(pDoc->m_IMDRdy)
	{
		if(pDoc->m_IMD < 0 )
			sBuf.Format(_T(" IMD = %d "), -pDoc->m_IMD);
		else
			sBuf.Format(_T(" IMD > %d "), pDoc->m_IMD);
	}
	nIndex = m_wndStatusBar.CommandToIndex(IDS_STATUSBAR1);
	m_wndStatusBar.GetPaneInfo(nIndex, nID, nStyle, nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex, nID, nStyle, szExtent.cx);
	m_wndStatusBar.SetPaneText(nIndex, sBuf);
	
// Update status pane 2
	sBuf.Format("Clk ppm=%d", pDoc->m_ClkError);
	nIndex = m_wndStatusBar.CommandToIndex(IDS_STATUSBAR2);
//	szExtent = dc.GetTextExtent(sBuf, sBuf.GetLength() );
	m_wndStatusBar.GetPaneInfo(nIndex, nID, nStyle, nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex, nID, nStyle, szExtent.cx);
	m_wndStatusBar.SetPaneText(nIndex, sBuf);

// Update status pane 3
	nIndex = m_wndStatusBar.CommandToIndex(IDS_STATUSBAR3);
	if(pDoc->m_FilePercentDone >= 0 )
	{
		sBuf.Format(" Percent Complete = %d%% ", pDoc->m_FilePercentDone);
		szExtent = dc.GetTextExtent(sBuf, sBuf.GetLength() );
	}
	else
	{
		sBuf = _T(" Wave Files Off ");
	}
	m_wndStatusBar.GetPaneInfo(nIndex, nID, nStyle, nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex, nID, nStyle, szExtent.cx);
	m_wndStatusBar.SetPaneText(nIndex, sBuf);


// Update time/date pane 4
CTime time = CTime::GetCurrentTime();
	if( pDoc->m_pSettings->m_UseLocalTime)
		sBuf = time.Format(_T(" %#d %b %Y     %#I:%M:%S %p "));
	else
		sBuf = time.FormatGmt(_T(" %#d %b %Y     %#H:%M:%S UTC "));
	nIndex = m_wndStatusBar.CommandToIndex(ID_INDICATOR_STATUS);
	szExtent = dc.GetTextExtent(sBuf, sBuf.GetLength() );
	m_wndStatusBar.GetPaneInfo(nIndex, nID, nStyle, nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex, nID, nStyle, szExtent.cx);
	m_wndStatusBar.SetPaneText(nIndex, sBuf);

	dc.SelectObject(pOldFont);	//restore font
	pCmdUI->Enable(TRUE);
}



///////////////////////////////////////////////////////////////
void CMainFrame::OnSetup() 
{
BOOL SatMode;
CWinPSKDoc* pDoc = (CWinPSKDoc*)CMainFrame::GetActiveDocument();
	ASSERT_VALID(pDoc);
CWinPSKView* pView = (CWinPSKView*)GetActiveView();
	ASSERT_VALID(pView);
CSetDlg dlg;
	dlg.m_pDoc = pDoc;
	dlg.m_sCallSign = pDoc->m_pSettings->m_sMyCall;
	dlg.m_UseSlash = pDoc->m_pSettings->m_UseSlash;
	dlg.m_PTTPort = pDoc->m_pSettings->m_PTTPort;
	dlg.m_ClockAdj = pDoc->m_pSettings->m_ClockAdj;
	dlg.m_UseLSB = pDoc->m_pSettings->m_UseLSB;
	dlg.m_UseLocalTime = pDoc->m_pSettings->m_UseLocalTime;
	dlg.m_EchoTXText = pDoc->m_pSettings->m_EchoTXText;
	dlg.m_FreqMin = pDoc->m_pSettings->m_FreqMin;
	dlg.m_FreqMax = pDoc->m_pSettings->m_FreqMax;
	dlg.m_AFCLimit = pDoc->m_pSettings->m_AFCLimit;
	dlg.m_SquelchSpeed = pDoc->m_pSettings->m_SquelchSpeed;
	dlg.m_SoundcardNum = pDoc->m_pSettings->m_SoundcardNum+1;
	dlg.m_PALPathName = pDoc->m_pSettings->m_PALPathName;
	if (dlg.DoModal() == IDOK)
	{
		pDoc->m_pSettings->m_sMyCall = dlg.m_sCallSign;
		pDoc->m_CWIdString = _T("de ") + dlg.m_sCallSign;
		fnSetCWIDString((PCHAR)(LPCTSTR)pDoc->m_CWIdString );

		CString str;
		str = _T("WinPSK 2.13 -- ") + dlg.m_sCallSign;
		AfxGetMainWnd()->SetWindowText( str );
		pDoc->m_pSettings->m_UseSlash = dlg.m_UseSlash;
		pDoc->m_pRcvEdit->SetZeroCharDisplay(pDoc->m_pSettings->m_UseSlash);
		pDoc->m_pSettings->m_UseLSB = dlg.m_UseLSB;
		pDoc->m_pSettings->m_UseLocalTime = dlg.m_UseLocalTime;
		pDoc->m_pSettings->m_EchoTXText = dlg.m_EchoTXText;
		pDoc->m_pSettings->m_FreqMin = dlg.m_FreqMin;
		pDoc->m_pSettings->m_FreqMax = dlg.m_FreqMax;
		pDoc->m_pSettings->m_AFCLimit = dlg.m_AFCLimit;
		pDoc->m_pSettings->m_SquelchSpeed = dlg.m_SquelchSpeed;
		pDoc->m_pSettings->m_PTTPort = dlg.m_PTTPort;
		pDoc->m_pSettings->m_PALPathName = dlg.m_PALPathName;
		pDoc->m_pSettings->m_SoundcardNum = dlg.m_SoundcardNum - 1;
		if(pDoc->m_pSettings->m_UseLSB)
			pView->m_QPSKCtrl.SetWindowText(_T("QPSK (lsb)"));
		else
			pView->m_QPSKCtrl.SetWindowText(_T("QPSK (usb)"));
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
		if( pDoc->m_pSettings->m_ClockAdj != dlg.m_ClockAdj )
		{
			pDoc->m_pSettings->m_ClockAdj = dlg.m_ClockAdj;
			fnSetClockErrorAdjustment(pDoc->m_pSettings->m_ClockAdj);
		}
		pView->m_pCPlotData->SetZoom( pDoc->m_pSettings->
					m_PlotViewState[pDoc->m_pSettings->m_TabSel].zoom,TRUE,
					pDoc->m_pSettings->m_FreqMin,
					pDoc->m_pSettings->m_FreqMax );
		pView->UpdatePlotInfo(PLOT_CHNG_ZOOM);
		SatMode = pDoc->m_SatMode;
		if( pDoc->m_pSettings->m_AFCLimit == 3000)
			pDoc->m_SatMode = TRUE;
		else
			pDoc->m_SatMode = FALSE;
		if(SatMode != pDoc->m_SatMode)
			pView->StopDLL();
	}
	if(pDoc->m_pSettings->m_PTTPort)
		fnSetComPort(pDoc->m_pSettings->m_PTTPort, 3);
	else
		fnSetComPort(0, 0);
	if(pDoc->m_pSettings->m_UseAFC)
	{
		fnSetAFCLimit(pDoc->m_pSettings->m_AFCLimit, RX_CHAN);
	}
}
////////////////////////////////////////////////////
//  Invoke Window's mixer control for Rx audio level adjustment
void CMainFrame::OnRxlvlsetup() 
{
	::WinExec(_T("SndVol32.exe /r"), SW_SHOW);	
}

////////////////////////////////////////////////////
//  Invoke Window's mixer control for Tx audio level adjustment
void CMainFrame::OnTxlvlsetup() 
{
	::WinExec(_T("SndVol32.exe /p"), SW_SHOW);	
}




void CMainFrame::OnSendfile() 
{
//  handler code here for getting text filename
CWinPSKDoc* pDoc = (CWinPSKDoc*)CMainFrame::GetActiveDocument();
	ASSERT_VALID(pDoc);
CString sFilter = _T("Text Files (*.txt)|*.txt||");
CString str;
	CFileDialog DlgInfile( TRUE, _T("txt"), pDoc->m_pSettings->m_PathName, 
			OFN_LONGNAMES|OFN_NOCHANGEDIR|OFN_HIDEREADONLY,
			(LPCTSTR)sFilter, this );
	if( DlgInfile.DoModal() == IDOK)
	{
		pDoc->m_pSettings->m_PathName = DlgInfile.GetPathName();
		if(ReadFile( str, pDoc->m_pSettings->m_PathName))
		{
			pDoc->m_pXmitEdit->InsertCString( str );
		}
	}
	
}

/////////////////////////////////////////////////////////////////////////////
//  Reads "PathName" file into CString str
//     returns FALSE if any errors, TRUE if OK
/////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::ReadFile(CString &str, CString PathName)
{
CFile cFile;
PBYTE pBuf = NULL;
BOOL retval = FALSE;
	if( cFile.Open( PathName, CFile::modeRead) )
	{
		DWORD length = cFile.GetLength();
		if(length >= FILE_TEXT_LIMIT)
			length = FILE_TEXT_LIMIT-1;
		pBuf = new BYTE[FILE_TEXT_LIMIT+1];
		if( cFile.Read( pBuf, length) == length )
		{
			pBuf[length] = 0;
			str = pBuf;
			retval = TRUE;
		}
		if( pBuf )
			delete pBuf;
	}
	return retval;
}


void CMainFrame::OnSelwavefiles() 
{
CWaveDlg dlg;
CWinPSKDoc* pDoc = (CWinPSKDoc*)CMainFrame::GetActiveDocument();
	ASSERT_VALID(pDoc);
CWinPSKView* pView = (CWinPSKView*)GetActiveView();
	ASSERT_VALID(pView);
	dlg.m_InPathName = pDoc->m_pSettings->m_InPathName;
	dlg.m_OutPathName = pDoc->m_pSettings->m_OutPathName;
	dlg.m_InWaveOffset = pDoc->m_pSettings->m_InWaveOffset;
	dlg.m_OutWaveTimeLimit = pDoc->m_pSettings->m_OutWaveTimeLimit;
	dlg.m_IOMode = pDoc->m_pSettings->m_IOMode;
	dlg.m_AppendMode = pDoc->m_pSettings->m_AppendMode;

	pView->StopDLL();
	
	if (dlg.DoModal() == IDOK)
	{
		pDoc->m_pSettings->m_InPathName = dlg.m_InPathName;
		pDoc->m_pSettings->m_OutPathName = dlg.m_OutPathName;
		pDoc->m_pSettings->m_InWaveOffset = dlg.m_InWaveOffset;
		pDoc->m_pSettings->m_OutWaveTimeLimit = dlg.m_OutWaveTimeLimit;
		pDoc->m_pSettings->m_IOMode = dlg.m_IOMode;
		pDoc->m_pSettings->m_AppendMode = dlg.m_AppendMode;
	}	
	if( dlg.m_IOMode == IOM_NORMAL)
		pView->StartDLL(pDoc);
}

void CMainFrame::WinHelp(DWORD dwData, UINT nCmd) 
{
CWinApp* theApp = AfxGetApp();
CString helpFilePath = theApp->m_pszHelpFilePath;
	switch(dwData)
	{
		case IDR_MAINFRAME:
		case IDD_ABOUTBOX:
			HtmlHelp( m_hWnd, helpFilePath, HH_DISPLAY_TOPIC, dwData);
			break;
		default:
			HtmlHelp( m_hWnd, helpFilePath, HH_DISPLAY_TOPIC, dwData);
			break;
	}
}

BOOL CMainFrame::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	return TRUE;
}
