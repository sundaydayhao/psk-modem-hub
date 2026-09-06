//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//
#if !defined(AFX_RCVVIEW_H__479856CE_5FBD_11D3_B4E7_302F50C10000__INCLUDED_)
#define AFX_RCVVIEW_H__479856CE_5FBD_11D3_B4E7_302F50C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RcvView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRcvView view

class CRcvView : public CView
{
protected:
	CRcvView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CRcvView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRcvView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CRcvView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CRcvView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RCVVIEW_H__479856CE_5FBD_11D3_B4E7_302F50C10000__INCLUDED_)
