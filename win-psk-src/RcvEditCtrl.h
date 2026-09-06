//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//
#if !defined(AFX_RCVEDITCTRL_H__4BA14663_5DE9_11D3_B4E7_A03B50C10000__INCLUDED_)
#define AFX_RCVEDITCTRL_H__4BA14663_5DE9_11D3_B4E7_A03B50C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RcvEditCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRcvEditCtrl window

class CRcvEditCtrl : public CRichEditCtrl
{
// Construction
public:
	CRcvEditCtrl();

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRcvEditCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRcvEditCtrl();
	void SetCharFormat(LOGFONT lfFont, LOGFONT lfs, COLORREF crText,
				COLORREF crSentText,COLORREF crBackground );
	void OnRcvChar(UINT ch, LONG src);

	// Generated message map functions
protected:
	//{{AFX_MSG(CRcvEditCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEditCopy();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKillfocus();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void TXStamp(BOOL m_EchoTXText, BOOL TXStart, BOOL UseLocalTime);
	void SetZeroCharDisplay( BOOL bSlash){m_bUseSlash = bSlash;};
	void CopyClip();
	void CancelSelection();
	void ClearAll();
	BOOL OKCopy();

	BOOL m_RcvCancel;

private:
	void PutCharInWindow( BYTE ch );
	BYTE GetQue();
	BOOL CheckQue();
	BOOL PutQue( BYTE ch );

	BOOL m_TXActive;
	BOOL m_bUseSlash;
	BOOL m_CTRLKeyDown;
	INT m_QHead;
	INT m_QTail;
	INT m_LastFont;
	INT m_LineCount;
	UINT m_TimerID;
	BYTE m_RcvQue[256];
	CHARFORMAT m_InsertFormat;
	CHARFORMAT m_SentFormat;
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CTheirCallCtrl window

class CTheirCallCtrl : public CRichEditCtrl
{
// Construction
public:
	CTheirCallCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTheirCallCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void PasteClip();
	void ReFormat();
	void InsertCString(CString Cstr);
	void ClearAll();
	virtual ~CTheirCallCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTheirCallCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdate();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	INT  m_OldLength;
	CHARFORMAT m_CharFormat;


};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CTheirNameCtrl window

class CTheirNameCtrl : public CRichEditCtrl
{
// Construction
public:
	CTheirNameCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTheirCallCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void ClearAll();
	void PasteClip();
	void InsertCString(CString Cstr);
	void ReFormat();
	virtual ~CTheirNameCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTheirNameCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdate();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	INT  m_OldLength;
	CHARFORMAT m_CharFormat;


};
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CTextGrabCtrl window

class CTextGrabCtrl : public CRichEditCtrl
{
// Construction
public:
	CTextGrabCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextGrabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void PasteClip();
	void ReFormat();
	void InsertCString(CString Cstr);
	void ClearAll();
	virtual ~CTextGrabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTextGrabCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdate();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	INT  m_OldLength;
	CHARFORMAT m_CharFormat;


};
/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RCVEDITCTRL_H__4BA14663_5DE9_11D3_B4E7_A03B50C10000__INCLUDED_)
