//////////////////////////////////////////////////////////////////////
//
#if !defined(AFX_XMITVIEW_H__479856CF_5FBD_11D3_B4E7_302F50C10000__INCLUDED_)
#define AFX_XMITVIEW_H__479856CF_5FBD_11D3_B4E7_302F50C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XmitView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXmitView view

class CXmitView : public CView
{
protected:
	CXmitView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CXmitView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXmitView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CXmitView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CXmitView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMITVIEW_H__479856CF_5FBD_11D3_B4E7_302F50C10000__INCLUDED_)
