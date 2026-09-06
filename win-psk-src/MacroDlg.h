//
#if !defined(AFX_MACRODLG_H__54E1C160_6432_11D3_B4E7_903750C10000__INCLUDED_)
#define AFX_MACRODLG_H__54E1C160_6432_11D3_B4E7_903750C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMacroDlg dialog

class CMacroDlg : public CDialog
{
// Construction
public:
	INT m_MacroSet;
	INT m_MacroKey;
	CMacroDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMacroDlg)
	enum { IDD = IDD_MACRODIALOG };
	CString	m_sData;
	CString	m_sMacroTitle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMacroDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMacroDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MACRODLG_H__54E1C160_6432_11D3_B4E7_903750C10000__INCLUDED_)
