//////////////////////////////////////////////////////////////////////
//
#if !defined(AFX_SQUELCHCTRL_H__C221E2A5_69E3_11D3_A25A_00A0C996E7F5__INCLUDED_)
#define AFX_SQUELCHCTRL_H__C221E2A5_69E3_11D3_A25A_00A0C996E7F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SquelchCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSquelchCtrl window
typedef void(CWnd::*PCALLBACKFUNCT)(int threshold); //callback format

class CSquelchCtrl : public CWnd
{
// Construction
public:
	CSquelchCtrl();

// Attributes
public:

// Operations
public:
	BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect,
							CWnd* pParentWnd, UINT nID,
							PCALLBACKFUNCT pCallback);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSquelchCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetLevel( INT nLevel );
	void SetThreshold( INT nLevel);
	BOOL SquelchOpen(){return m_SquelchOn;};
	INT GetThreshold();
	virtual ~CSquelchCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSquelchCtrl)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CWnd* m_pParentWnd;
	PCALLBACKFUNCT m_pCallback;
	BOOL m_SquelchOn;
	INT m_Scale;
	INT m_Threshold;
	INT m_nLevel;
	CRect m_rPlot;			// client plot screen rectangle coordinates
	CBitmap m_bmImage;		// bitmap image of status window
	CRect m_rClient;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SQUELCHCTRL_H__C221E2A5_69E3_11D3_A25A_00A0C996E7F5__INCLUDED_)
