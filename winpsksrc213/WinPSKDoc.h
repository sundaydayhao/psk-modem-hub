// WinPSKDoc.h : interface of the CWinPSKDoc class
//
//////////////////////////////////////////////////////////////////////
//

#if !defined(AFX_WINPSKDOC_H__EFF3B52A_3631_11D3_B48C_E03851C10000__INCLUDED_)
#define AFX_WINPSKDOC_H__EFF3B52A_3631_11D3_B48C_E03851C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RcvEditCtrl.h"
#include "XmitEditCtrl.h"
#include "Settings.h"

class CWinPSKDoc : public CDocument
{
protected: // create from serialization only
	CWinPSKDoc();
	DECLARE_DYNCREATE(CWinPSKDoc)

// Attributes
public:
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinPSKDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	CString ReadPaletteFile( CString sPath );
	INT m_FilePercentDone;
	virtual ~CWinPSKDoc();
	void ClearTxQue();
// variables 
	BOOL m_SatMode;
	BOOL m_IMDRdy;
	BOOL m_SoundCardReset;
	BOOL m_FFTModeChange;
	BOOL m_MacroStartTX;
	BOOL m_TXNet;
	INT m_DispXmax;
	INT m_DispYmax;
	INT m_DispCFreq;
	INT m_FormHeight;
	INT m_SyncHist[21];
	INT m_SquelchLevel;
	INT m_IMD;
	INT m_ClkError;
	LONG m_ColorTbl[256];
	CRcvEditCtrl* m_pRcvEdit;
	CXmitEditCtrl* m_pXmitEdit;
	CTheirCallCtrl* m_pTheirCallEdit;
	CTheirNameCtrl* m_pTheirNameEdit;
	CTextGrabCtrl* m_pTextGrabEdit;
	CFormView* m_pWinPSKView;
	CSettings* m_pSettings;
	HWND m_hRcvEditCtrl;
	HWND m_hXmitEditCtrl;
	CString m_CWIdString;
	CString m_sTheirCall;
	CString m_sTheirName;
	CString m_sTextGrab;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWinPSKDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINPSKDOC_H__EFF3B52A_3631_11D3_B48C_E03851C10000__INCLUDED_)
