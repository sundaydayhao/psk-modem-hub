// WinPSK.cpp : Defines the class behaviors for the application.
//
//////////////////////////////////////////////////////////////////////
//

#include "stdafx.h"
#include "WinPSK.h"

#include "MainFrm.h"
#include "WinPSKDoc.h"
#include "WinPSKView.h"
#include <locale.h>
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWinPSKApp

BEGIN_MESSAGE_MAP(CWinPSKApp, CWinApp)
	//{{AFX_MSG_MAP(CWinPSKApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard help commands
	ON_COMMAND(ID_HELP_TOPICS, CWinPSKApp::OnHelpFinder)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinPSKApp construction

CWinPSKApp::CWinPSKApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWinPSKApp object

CWinPSKApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CWinPSKApp initialization

BOOL CWinPSKApp::InitInstance()
{
// code to keep a second instance of this program from running and
//    if one already is running, make it visable.
	::CreateMutex( NULL, TRUE, m_pszExeName);
	if( GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CWnd* pPrevWnd = CWnd::GetDesktopWindow()->GetWindow(GW_CHILD);
		while( pPrevWnd )	//go through all windows and find the other instance
		{
			if( ::GetProp( pPrevWnd->GetSafeHwnd(), m_pszExeName) )
			{
				if( pPrevWnd->IsIconic() )		//if minimized restore it
					pPrevWnd->ShowWindow(SW_RESTORE);
				pPrevWnd->SetForegroundWindow();	//put in front
				pPrevWnd->GetLastActivePopup()->SetForegroundWindow(); //if has poppup
				return FALSE;
			}
			pPrevWnd = pPrevWnd->GetWindow(GW_HWNDNEXT);		//try next one
		}
		return FALSE;
	}
	// CG: The following block was inserted by 'Status Bar' component.
	{
		//Set up date and time defaults so they're the same as system defaults
		setlocale(LC_ALL, "");
	}

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();
	AfxInitRichEdit();

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	if( fnGetDLLVersion() < 117 )	//is dll version < 1.17
	{
		AfxMessageBox(_T("Requires Ver 1.17 or greater PSKCore.dll file."));
		return FALSE;
	}

//
	// The registry key under which the screen settings are stored.
	SetRegistryKey(_T("AE4JY Software"));

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CWinPSKDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CWinPSKView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

//
// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();

	TCHAR szpath[MAX_PATH];
	TCHAR szDir[_MAX_DIR];
	TCHAR szDrive[_MAX_DRIVE];
	::GetModuleFileName(NULL, szpath, MAX_PATH);
	_splitpath(szpath, szDrive, szDir, NULL, NULL);
	CString str = szDir;
	free((void*)m_pszHelpFilePath);
	str += _T("WinPsk.chm");
	m_pszHelpFilePath =_tcsdup( str);
// for use in detecting another instance of this program running
	::SetProp( m_pMainWnd->GetSafeHwnd(), m_pszExeName, (HANDLE)1);
	return TRUE;
}

int CWinPSKApp::ExitInstance() 
{
	fnStopSoundCard();
	return CWinApp::ExitInstance();
}
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
/////////////////////////////////////////////////////////////////////////////
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL OnInitDialog();
	LONGLONG CAboutDlg::GetDriveFreeSpace(CString sPathName);
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CWinPSKApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CWinPSKApp message handlers


BOOL CAboutDlg::OnInitDialog()
{
char buf[20];
CString strFreeDiskSpace;
CString strFmt;
char drive;
	CDialog::OnInitDialog();

	// Fill disk free information
	drive = _getdrive()-1 + _T('A');
	CString sDrive = drive;
	sDrive = sDrive + ":\\"; // use current default drive
	LONGLONG size = GetDriveFreeSpace( sDrive);
	size /= 1000000;
	strFmt.LoadString(CG_IDS_DISK_SPACE);
	sprintf(buf, strFmt, size,drive);
	strFreeDiskSpace = buf;
	SetDlgItemText(IDC_DISK_SPACE, strFreeDiskSpace);
	FLOAT ver = (FLOAT)fnGetDLLVersion();
	strFmt.Format(_T("PSKCore.dll Version = %3.2f"),ver/100.0);
	SetDlgItemText(IDC_COREVERSION, strFmt);
	return TRUE;
}
/////////////////////////////////////////////////////////////////
// returns free disk space on drive specified by sPathName.
// The extra logic is for dealing with early Win95 versions that don't
// support the extended disk space function "GetDiskFreeSpaceEx(..)".
/////////////////////////////////////////////////////////////////
LONGLONG CAboutDlg::GetDriveFreeSpace(CString sPathName)
{
typedef BOOL (CALLBACK* LPFNDLLFUNC)(LPCTSTR, PULARGE_INTEGER,
					 PULARGE_INTEGER, PULARGE_INTEGER);

LONGLONG SpaceFree;
	SpaceFree = 0;					// see if it exists
	HMODULE hKernel = GetModuleHandle( _T("kernel32.dll"));
	LPFNDLLFUNC pFunct = (LPFNDLLFUNC)GetProcAddress( hKernel,"GetDiskFreeSpaceExA");
	if(pFunct != NULL)		//if function is in kernel32.dll
	{
		ULARGE_INTEGER TotalSpace;
		ULARGE_INTEGER FreeSpace;
		pFunct( sPathName.Left(3), &FreeSpace, &TotalSpace, NULL);
		SpaceFree = FreeSpace.QuadPart;
	}
	else					//else is Win95 pre OSR2 so use old function
	{
		DWORD SectorsPerCluster, BytesPerSector;
		DWORD NumberOfFreeClusters, TotalNumberOfClusters;
		if( GetDiskFreeSpace( sPathName.Left(3),
					&SectorsPerCluster, &BytesPerSector,
					&NumberOfFreeClusters, &TotalNumberOfClusters ) ) 
		SpaceFree = (LONGLONG)(NumberOfFreeClusters*SectorsPerCluster*BytesPerSector);
	}
	return SpaceFree;
}



