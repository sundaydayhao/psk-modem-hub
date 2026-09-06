// SetDlg.cpp : implementation file
//
//

#include "stdafx.h"
#include "winpsk.h"
#include "SetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetDlg dialog


CSetDlg::CSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetDlg)
	m_UseSlash = FALSE;
	m_UseLSB = FALSE;
	m_EchoTXText = FALSE;
	m_UseLocalTime = FALSE;
	m_PTTPort = -1;
	m_ClockAdj = 0;
	m_FreqMin = 0;
	m_FreqMax = 0;
	m_AFCLimit = 0;
	m_SoundcardNum = -1;
	m_SquelchSpeed = 75;
	m_sCallSign = _T("");
	m_SatMode = FALSE;
	//}}AFX_DATA_INIT
}


void CSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetDlg)
	DDX_Control(pDX, IDC_PALTXT, m_PALTxtCtrl);
	DDX_Check(pDX, IDC_USESLASH, m_UseSlash);
	DDX_Check(pDX, IDC_USELSB, m_UseLSB);
	DDX_Check(pDX, IDC_ECHOTXTEXT, m_EchoTXText);
	DDX_Check(pDX, IDC_USELOCALTIME, m_UseLocalTime);
	DDX_Radio(pDX, IDC_PTTNONE, m_PTTPort);
	DDX_Text(pDX, IDC_CLKADJ, m_ClockAdj);
	DDV_MinMaxInt(pDX, m_ClockAdj, -20000, 20000);
	DDX_Text(pDX, IDC_STARTFREQ, m_FreqMin);
	DDX_Text(pDX, IDC_ENDFREQ, m_FreqMax);
	DDX_Text(pDX, IDC_AFCLIMIT, m_AFCLimit);
	DDX_Radio(pDX, IDC_FREESC, m_SoundcardNum);
	DDX_Text(pDX, IDC_SQSPEED, m_SquelchSpeed);
	DDX_Text(pDX, IDC_CALLSIGNEDIT, m_sCallSign);
	DDX_Check(pDX, IDC_SATMODE, m_SatMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetDlg, CDialog)
	//{{AFX_MSG_MAP(CSetDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CLKADJSPIN, OnDeltaposClkadjspin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_STARTFREQSPIN, OnDeltaposStartfreqspin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ENDFREQSPIN, OnDeltaposEndfreqspin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_AFCSPIN, OnDeltaposAfcspin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SQSPIN, OnDeltaposSqspin)
	ON_BN_CLICKED(IDC_SATMODE, OnSatmode)
	ON_BN_CLICKED(IDC_PALBUTTON, OnPalbutton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



///////////////////////////////////////////////////////////////////////////////
//  Initialize check box states with current settings
///////////////////////////////////////////////////////////////////////////////
BOOL CSetDlg::OnInitDialog() 
{
INT IDC_Array[] = 
		{IDC_PTTNONE,IDC_PTTCOM1,IDC_PTTCOM2,IDC_PTTCOM3,IDC_PTTCOM4,
		IDC_PTTCOM5,IDC_PTTCOM6,IDC_PTTCOM7,IDC_PTTCOM8};
	CDialog::OnInitDialog();
CWnd* pWnd;
	for(INT i=1; i<=8; i++)
	{
		pWnd = GetDlgItem( IDC_Array[i] );
		if( fnSetComPort( i, 0) )
		{
			pWnd->EnableWindow(TRUE);
			if( m_PTTPort == i )
				pWnd->CheckDlgButton( IDC_Array[i], TRUE );
		}
		else
		{
			pWnd->EnableWindow(FALSE);
			if(m_PTTPort==i)
			{
				m_PTTPort = 0;
			}
		}
	}
	if(m_AFCLimit == 3000)
	{
		m_SatMode = TRUE;
		pWnd = GetDlgItem( IDC_AFCLIMIT ); ASSERT_VALID(pWnd);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem( IDC_ECHOTXTEXT ); ASSERT_VALID(pWnd);
		pWnd->EnableWindow(FALSE);
	}
	else
	{
		pWnd = GetDlgItem( IDC_AFCLIMIT ); ASSERT_VALID(pWnd);
		pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem( IDC_ECHOTXTEXT ); ASSERT_VALID(pWnd);
		pWnd->EnableWindow(TRUE);
		m_SatMode = FALSE;
	}
	m_PALPathName = m_pDoc->ReadPaletteFile(m_PALPathName);
	m_PALTxtCtrl.SetWindowText( GetFileName(m_PALPathName) );
	UpdateData(FALSE);		//Put changed data into control
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
//  update the clock adjustment value when spin control is used.
///////////////////////////////////////////////////////////////////////////////
void CSetDlg::OnDeltaposClkadjspin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);		//Get data from control
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	m_ClockAdj -= (100*pNMUpDown->iDelta);
	if(m_ClockAdj > 20000)
		m_ClockAdj = 20000;
	if(m_ClockAdj < -20000)
		m_ClockAdj = -20000;
	UpdateData(FALSE);		//Put changed data into control
	*pResult = 0;
}


void CSetDlg::OnDeltaposStartfreqspin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);		//Get data from control
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	m_FreqMin -= (100*pNMUpDown->iDelta);
	if(m_FreqMin >= m_FreqMax-200)
		m_FreqMin = m_FreqMax-200;
	if(m_FreqMin < 200)
		m_FreqMin = 200;
	UpdateData(FALSE);		//Put changed data into control
	*pResult = 0;
}

void CSetDlg::OnDeltaposEndfreqspin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);		//Get data from control
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	m_FreqMax -= (100*pNMUpDown->iDelta);
	if(m_FreqMax <= m_FreqMin+200)
		m_FreqMax = m_FreqMin+200;
	if(m_FreqMax >3500)
		m_FreqMax = 3500;
	UpdateData(FALSE);		//Put changed data into control
	*pResult = 0;
}

void CSetDlg::OnDeltaposAfcspin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);		//Get data from control
	if(!m_SatMode)
	{
		NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
		m_AFCLimit -= (1*pNMUpDown->iDelta);
		if(m_AFCLimit < 5)
			m_AFCLimit = 5;
		if(m_AFCLimit > 250)
			m_AFCLimit = 250;
		UpdateData(FALSE);		//Put changed data into control
	}
	
	*pResult = 0;
}

void CSetDlg::OnDeltaposSqspin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	UpdateData(TRUE);		//Get data from control
	m_SquelchSpeed -= (1*pNMUpDown->iDelta);
	if(m_SquelchSpeed < 10)
		m_SquelchSpeed = 10;
	if(m_SquelchSpeed > 200)
		m_SquelchSpeed = 200;
	UpdateData(FALSE);		//Put changed data into control
	
	*pResult = 0;
}

void CSetDlg::OnSatmode() 
{
CWnd* pWnd;
	pWnd = GetDlgItem( IDC_AFCLIMIT ); ASSERT_VALID(pWnd);
	UpdateData(TRUE);		//Get data from control
	if(m_SatMode)
	{
		m_AFCLimit = 3000;
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem( IDC_ECHOTXTEXT ); ASSERT_VALID(pWnd);
		pWnd->EnableWindow(FALSE);
	}
	else
	{
		m_AFCLimit = 250;
		pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem( IDC_ECHOTXTEXT ); ASSERT_VALID(pWnd);
		pWnd->EnableWindow(TRUE);
	}
	UpdateData(FALSE);		//Put changed data into control
}

void CSetDlg::OnPalbutton() 
{
CString sFilter = _T("Palette Files (*.pal)|*.pal||");
CString str;
CFileDialog DlgInfile( TRUE, _T("pal"), m_PALPathName, 
			OFN_LONGNAMES|OFN_NOCHANGEDIR|OFN_HIDEREADONLY,
			(LPCTSTR)sFilter, this );
	if( DlgInfile.DoModal() == IDOK)
	{
		m_PALPathName = DlgInfile.GetPathName();
		m_pDoc->ReadPaletteFile(m_PALPathName);
	}
	m_PALTxtCtrl.SetWindowText( GetFileName(m_PALPathName) );
}

CString CSetDlg::GetFileName(CString sPath)
{
CFile cFile;
CString fname = _T("Using Default");
	if( cFile.Open( sPath, CFile::modeRead) )
	{
		fname = cFile.GetFileName();
	}
	return fname;
}
