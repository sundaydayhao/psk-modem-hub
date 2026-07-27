// MacroDlg.cpp : implementation file
//
//
#include "stdafx.h"
#include "winpsk.h"
#include "MacroDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMacroDlg dialog


CMacroDlg::CMacroDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMacroDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMacroDlg)
	m_sData = _T("");
	m_sMacroTitle = _T("");
	//}}AFX_DATA_INIT
}


void CMacroDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMacroDlg)
	DDX_Text(pDX, IDC_MACROEDIT, m_sData);
	DDV_MaxChars(pDX, m_sData, 199);
	DDX_Text(pDX, IDC_TITLE_EDIT, m_sMacroTitle);
	DDV_MaxChars(pDX, m_sMacroTitle, 25);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMacroDlg, CDialog)
	//{{AFX_MSG_MAP(CMacroDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMacroDlg message handlers

BOOL CMacroDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString str;
	str.Format(_T("F%d Set %d   Macro Setup Menu"), m_MacroKey+1, m_MacroSet+1);
	SetWindowText( str );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
