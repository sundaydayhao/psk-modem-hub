// WinPSKView.h : interface of the CWinPSKView class
//
//////////////////////////////////////////////////////////////////////
//

#if !defined(AFX_WINPSKVIEW_H__EFF3B52C_3631_11D3_B48C_E03851C10000__INCLUDED_)
#define AFX_WINPSKVIEW_H__EFF3B52C_3631_11D3_B48C_E03851C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PlotData.h"
#include "squelchctrl.h"

#define PLOT_CHNG_MOUSE 0
#define PLOT_CHNG_ZOOM 1
#define PLOT_CHNG_FREQ 2

#define STATE_RX 0
#define STATE_TX 1
#define STATE_FINISH 2
#define	STATE_TUNE 3

#define DLL_RX_STATUS 0
#define DLL_TX_STATUS 1
#define DLL_SLOW_STATUS 2
#define DLL_FINISH_STATUS 3
#define DLL_INWAVEDONE 4
#define DLL_OUTWAVEDONE 5
#define DLLSTAT_INWAVEFILESTATUS 6
#define DLLSTAT_OUTWAVEFILESTATUS 7

class CWinPSKView : public CFormView
{
protected: // create from serialization only
	CWinPSKView();
	DECLARE_DYNCREATE(CWinPSKView)

public:
	CPlotData* m_pCPlotData;
	INT m_ProgramState;
	//{{AFX_DATA(CWinPSKView)
	enum { IDD = IDD_WINPSK_FORM };
	CButton	m_TxFreqModeCtrl;
	CButton	m_RunCtrl;
	CStatic	m_DebugCtrl2;
	CStatic	m_DebugCtrl1;
	BOOL	m_fSlowAve;
	BOOL	m_UseAFC;
	BOOL	m_TXNet;
	BOOL	m_AutoTxtSel;
	INT		m_RxFreq;
	INT		m_TxDisp;
	INT		m_PSKMode;
	CStatic	m_XMaxCntrl;
	CStatic	m_MacroSetTxtCtrl;
	CButton	m_QPSKCtrl;
	CButton	m_TRCntrl;
	CStatic	m_XMinCntrl;
	CTabCtrl	m_DispTabCtrl;
	CString	m_XMaxCStr;
	CString	m_XMinCStr;
	CString	m_ZoomTxt;
	BOOL	m_PSK63;
	//}}AFX_DATA
	CButton	m_MTitleCtrl[NUM_MACROKEYS];	//button ctrl array to access macro title strings
// Attributes
public:
	CWinPSKDoc* GetDocument();
	void OnTxtune();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinPSKView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SquelchThreshCallback( INT threshold);
	BOOL m_DLLRunning;
	void StopDLL();
	void StartDLL( CWinPSKDoc* pDoc);
	void UpdatePlotInfo(INT src);
	void ProcessFKeys(WPARAM wParam);
	virtual ~CWinPSKView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWinPSKView)
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeDisplaytab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDeltaposZoom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposRxFreqspin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSlowave();
	afx_msg void OnAfc();
	afx_msg void OnTrbutton();
	afx_msg void OnBpsk();
	afx_msg void OnQpsk();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNet();
	afx_msg void OnEditPaste();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditClear();
	afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnKillfocusRxfreq();
	afx_msg void OnSetfocusRxfreq();
	afx_msg void OnKillfocusTxfreq();
	afx_msg void OnDeltaposTxfreqspin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusTxfreq();
	afx_msg void OnM1button();
	afx_msg void OnM2button();
	afx_msg void OnM3button();
	afx_msg void OnM4button();
	afx_msg void OnM5button();
	afx_msg void OnM6button();
	afx_msg void OnM7button();
	afx_msg void OnM8button();
	afx_msg void OnM9button();
	afx_msg void OnM10button();
	afx_msg void OnAutotxtsel();
	afx_msg void OnClrqso();
	afx_msg void OnDecfnset();
	afx_msg void OnIncfnset();
	afx_msg void OnSethome();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRun();
	afx_msg void OnTxFreqMode();
	afx_msg void OnPsk3163();
	//}}AFX_MSG
	afx_msg LRESULT OnDataRdy(UINT x, LONG y);
	afx_msg LRESULT OnCharRdy(UINT ch, LONG src);
	afx_msg LRESULT OnStatusChange(UINT status, LONG x);
	afx_msg LRESULT OnIMDRdy(UINT IMD, LONG chan);
	afx_msg LRESULT OnClkError(UINT error, LONG chan);
	
	DECLARE_MESSAGE_MAP()
	BOOL OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );

private:
	INT m_NewChannel;
//functions
	void ToggleTR();
	void FindClosestSignal(BOOL LookRight);
	void UpdateStatus();
	void ChngZoom(INT delta);
	void MacroDlg( INT id);
	INT IndexToFreq(INT index);
	INT FreqToIndex(INT Freq);
// variables
	BOOL m_ScreenInitialized;
	BOOL m_ZoomOK;
	BOOL m_fRxFreqEditing;
	BOOL m_fTxFreqEditing;
	INT m_PlotWidth;
	INT m_SearchRange;
	INT m_IMDDispDelay;
	UINT m_TimerID;
	CHAR m_ToolTipStr[80];
	CSquelchCtrl* m_pSquelchCtrl;
	HCURSOR m_hOrigCursor;
	HCURSOR m_hCross;

};

#ifndef _DEBUG  // debug version in WinPSKView.cpp
inline CWinPSKDoc* CWinPSKView::GetDocument()
   { return (CWinPSKDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINPSKVIEW_H__EFF3B52C_3631_11D3_B48C_E03851C10000__INCLUDED_)
