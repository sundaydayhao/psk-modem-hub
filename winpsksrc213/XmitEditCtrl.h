//
#if !defined(AFX_XMITEDITCTRL_H__4BA14664_5DE9_11D3_B4E7_A03B50C10000__INCLUDED_)
#define AFX_XMITEDITCTRL_H__4BA14664_5DE9_11D3_B4E7_A03B50C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XmitEditCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXmitEditCtrl window

class CXmitEditCtrl : public CRichEditCtrl
{
// Construction
public:
	CXmitEditCtrl();

// Attributes
public:

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXmitEditCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateFormats();
	void ClearSentText();
	void InsertCString( CString Cstr );
	void SetCharFormat(LOGFONT lfFont, LOGFONT lfSentFont, COLORREF crText,
				COLORREF crSentText,COLORREF crBackground);
	void ClearAll();
	BOOL OKClearSel();
	BOOL OKCutClip();
	virtual ~CXmitEditCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CXmitEditCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnEditClear();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnEditUndo();
	afx_msg void OnUpdate();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_OnChangeLock;
	void InsertFile( CString& str, CString Filename);
	INT m_LastEndPosition;
	LONG m_XmitPos;				//position ptr of text that has been sent
	CHARFORMAT m_InsertFormat;
	CHARFORMAT m_RemovedFormat;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMITEDITCTRL_H__4BA14664_5DE9_11D3_B4E7_A03B50C10000__INCLUDED_)
