// MainFrm.h : interface of the CMainFrame class
//
//////////////////////////////////////////////////////////////////////
//

#if !defined(AFX_MAINFRM_H__EFF3B528_3631_11D3_B48C_E03851C10000__INCLUDED_)
#define AFX_MAINFRM_H__EFF3B528_3631_11D3_B48C_E03851C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_nTimePaneNo;
	void StayOnTop();
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
private:
	BOOL m_StayOnTop;
	BOOL ReadFile(CString &str, CString PathName);
	INT m_Pane0Height;
	INT m_Pane1Height;
	INT m_Pane2Height;
	UINT m_TimerID;
	WINDOWPLACEMENT m_wp;
protected:  // control bar embedded members
	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT pwp, INT* P0Height, INT* P1Height, INT* P2Height);
	void SaveWindowPlacement(LPWINDOWPLACEMENT pwp, INT P0Height, INT P1Height, INT P2Height);
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	afx_msg void OnUpdateStatus(CCmdUI* pCmdUI);
	CSplitterWnd m_WndSplitter;
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRcvbackcolor();
	afx_msg void OnXmitbackcolor();
	afx_msg void OnRcvfont();
	afx_msg void OnXmitfont();
	afx_msg void OnXmitsentfont();
	afx_msg void OnClose();
	afx_msg void OnClearrcv();
	afx_msg void OnClearxmit();
	afx_msg void OnSetup();
	afx_msg void OnRxlvlsetup();
	afx_msg void OnTxlvlsetup();
	afx_msg void OnTxtune();
	afx_msg void OnSendfile();
	afx_msg void OnUpdateViewStayontop(CCmdUI* pCmdUI);
	afx_msg void OnViewStayontop();
	afx_msg void OnSelwavefiles();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__EFF3B528_3631_11D3_B48C_E03851C10000__INCLUDED_)
