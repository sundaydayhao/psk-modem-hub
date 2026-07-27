// WaveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winpsk.h"
#include "WaveDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaveDlg dialog


CWaveDlg::CWaveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWaveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWaveDlg)
	m_InPathName = _T("");
	m_InWaveOffset = 0;
	m_OutPathName = _T("");
	m_RxSource = 0;
	m_LogInput = FALSE;
	m_LogTx = FALSE;
	m_AppendMode = FALSE;
	m_NoSoundCardOut = FALSE;
	m_OutWaveTimeLimit = 1;
	m_EchoInput = FALSE;
	//}}AFX_DATA_INIT
	m_IOMode = IOM_NORMAL;
}


void CWaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaveDlg)
	DDX_Control(pDX, IDC_INFILEDATA, m_InFileDataCtrl);
	DDX_Text(pDX, IDC_WAVEREADPATH, m_InPathName);
	DDX_Text(pDX, IDC_INOFFSET, m_InWaveOffset);
	DDV_MinMaxUInt(pDX, m_InWaveOffset, 0, 80000);
	DDX_Text(pDX, IDC_WAVEWRITEPATH, m_OutPathName);
	DDX_Radio(pDX, IDC_SOUNDSRC, m_RxSource);
	DDX_Check(pDX, IDC_LOGINPUT, m_LogInput);
	DDX_Check(pDX, IDC_LOGTX, m_LogTx);
	DDX_Check(pDX, IDC_OUTPUTAPPEND, m_AppendMode);
	DDX_Check(pDX, IDC_NOSOUNDOUT, m_NoSoundCardOut);
	DDX_Text(pDX, IDC_OUTLIMIT, m_OutWaveTimeLimit);
	DDV_MinMaxInt(pDX, m_OutWaveTimeLimit, 1, 1440);
	DDX_Check(pDX, IDC_ECHOINPUT, m_EchoInput);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWaveDlg, CDialog)
	//{{AFX_MSG_MAP(CWaveDlg)
	ON_BN_CLICKED(IDC_SELECTREADFILE, OnSelectreadfile)
	ON_BN_CLICKED(IDC_SELECTOUTFILE, OnSelectoutfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaveDlg message handlers
BOOL CWaveDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	if( m_IOMode & IOM_WAVEIN)
		m_RxSource = 1;
	else
		m_RxSource = 0;
	if( m_IOMode & IOM_WAVELOG)
		m_LogInput = TRUE;
	else
		m_LogInput = FALSE;
	if( m_IOMode & IOM_WAVETX)
		m_LogTx = TRUE;
	else
		m_LogTx = FALSE;
	if( m_IOMode & IOM_NOTXSOUND)
		m_NoSoundCardOut = TRUE;
	else
		m_NoSoundCardOut = FALSE;
	if( m_IOMode & IOM_ECHOINPUT)
		m_EchoInput = TRUE;
	else
		m_EchoInput = FALSE;
	
	if( (m_OutWaveTimeLimit < 1) || (m_OutWaveTimeLimit > 1440) )
		m_OutWaveTimeLimit = 1;
	GetInFileDetails(m_InPathName);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWaveDlg::OnOK() 
{
	UpdateData(TRUE);
	if(	m_RxSource == 1)
		m_IOMode |= IOM_WAVEIN;
	else
		m_IOMode &= ~IOM_WAVEIN;
	if(	m_LogInput)
		m_IOMode |= IOM_WAVELOG;
	else
		m_IOMode &= ~IOM_WAVELOG;
	if(	m_LogTx)
		m_IOMode |= IOM_WAVETX;
	else
		m_IOMode &= ~IOM_WAVETX;
	if(	m_NoSoundCardOut)
		m_IOMode |= IOM_NOTXSOUND;
	else
		m_IOMode &= ~IOM_NOTXSOUND;
	if(	m_EchoInput)
		m_IOMode |= IOM_ECHOINPUT;
	else
		m_IOMode &= ~IOM_ECHOINPUT;

	CDialog::OnOK();
}

void CWaveDlg::OnSelectreadfile() 
{
//  handler code here for getting Input filename
CString sFilter = "Wave Files (*.wav)|*.wav||";
	UpdateData(TRUE);
CFileDialog Dlgfile( TRUE, "wav", m_InPathName, 
			OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
			(LPCTSTR)sFilter, this );
	if( Dlgfile.DoModal() == IDOK)
	{
		m_InPathName = Dlgfile.GetPathName();
		GetInFileDetails(m_InPathName);
	}
	UpdateData(FALSE);
}


void CWaveDlg::OnSelectoutfile() 
{
//  handler code here for getting Output filename
CString sFilter = "Wave Files (*.wav)|*.wav||";
	UpdateData(TRUE);
CFileDialog Dlgfile( TRUE, "wav", m_OutPathName, 
			OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
			(LPCTSTR)sFilter, this );
	if( Dlgfile.DoModal() == IDOK)
	{
		m_OutPathName = Dlgfile.GetPathName();
	}
	UpdateData(FALSE);
}


void CWaveDlg::GetInFileDetails(CString spath)
{
INT err;
LONG length;
CString str;
CHAR errstr[50];
	err = fnSetInputWavePath((PCHAR)(LPCTSTR)m_InPathName, &length, 0);
	if(err!=0)
	{
		fnGetErrorString( errstr );
		str = errstr;
	}
	else
	{
		str.Format( "%d Second 8000Hz 16Bit Mono File", length);
	}
	m_InFileDataCtrl.SetWindowText(str);
}
