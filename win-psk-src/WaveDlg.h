#if !defined(AFX_WAVERDDLG_H__A7D5BCBF_276D_11D5_BE04_00A0C9FB2C6E__INCLUDED_)
#define AFX_WAVERDDLG_H__A7D5BCBF_276D_11D5_BE04_00A0C9FB2C6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaveRdDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWaveDlg dialog

class CWaveDlg : public CDialog
{
// Construction
public:
	CWaveDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWaveDlg)
	enum { IDD = IDD_WAVEDIALOG };
	CStatic	m_InFileDataCtrl;
	CString	m_InPathName;
	UINT	m_InWaveOffset;
	CString	m_OutPathName;
	int		m_RxSource;
	BOOL	m_LogInput;
	BOOL	m_LogTx;
	BOOL	m_AppendMode;
	BOOL	m_NoSoundCardOut;
	int		m_OutWaveTimeLimit;
	BOOL	m_EchoInput;
	//}}AFX_DATA
	INT m_IOMode;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWaveDlg)
	afx_msg void OnSelectreadfile();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectoutfile();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void GetInFileDetails( CString spath);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAVERDDLG_H__A7D5BCBF_276D_11D5_BE04_00A0C9FB2C6E__INCLUDED_)
