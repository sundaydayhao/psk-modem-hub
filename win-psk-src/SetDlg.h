//////////////////////////////////////////////////////////////////////
//
#if !defined(AFX_SetDlg_H__B2BDAD73_6792_11D3_A258_00A0C996E7F5__INCLUDED_)
#define AFX_SetDlg_H__B2BDAD73_6792_11D3_A258_00A0C996E7F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetDlg.h : header file
//
#include "WinPSKDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CSetDlg dialog

class CSetDlg : public CDialog
{
// Construction
public:
	CSetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetDlg)
	enum { IDD = IDD_GENSETUP };
	CStatic	m_PALTxtCtrl;
	BOOL	m_UseSlash;
	BOOL	m_UseLSB;
	BOOL	m_EchoTXText;
	BOOL	m_UseLocalTime;
	int		m_PTTPort;
	int		m_ClockAdj;
	int		m_FreqMin;
	int		m_FreqMax;
	int		m_AFCLimit;
	int		m_SoundcardNum;
	int		m_SquelchSpeed;
	CString	m_sCallSign;
	BOOL	m_SatMode;
	//}}AFX_DATA

	CString m_PALPathName;
	CWinPSKDoc* m_pDoc;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposClkadjspin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposStartfreqspin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposEndfreqspin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposAfcspin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSqspin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSatmode();
	afx_msg void OnPalbutton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString GetFileName(CString sPath);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SetDlg_H__B2BDAD73_6792_11D3_A258_00A0C996E7F5__INCLUDED_)
