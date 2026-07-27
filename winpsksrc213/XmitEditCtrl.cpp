// XmitEditCtrl.cpp : implementation file
//
//////////////////////////////////////////////////////////////////////
//

#include "stdafx.h"
#include "winpsk.h"
#include "winpskview.h"
#include "WinPSKDoc.h"
#include "XmitView.h"
#include "XmitEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FILE_TEXT_LIMIT 2000
#define DLL_AUTOSTOP 1
#define DLL_USECWID 2
/////////////////////////////////////////////////////////////////////////////
// CXmitEditCtrl
/////////////////////////////////////////////////////////////////////////////
CXmitEditCtrl::CXmitEditCtrl()
{
	m_XmitPos = 0;
	m_OnChangeLock = FALSE;
	m_LastEndPosition = 0;
}

CXmitEditCtrl::~CXmitEditCtrl()
{
}


BEGIN_MESSAGE_MAP(CXmitEditCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CXmitEditCtrl)
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Override of OnCreate, need to enable ENM_UPDATE messages
/////////////////////////////////////////////////////////////////////////////
int CXmitEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetEventMask( ENM_UPDATE);		//notify parent(CXmitView) of changes
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Initializes the Font, font color, and background color for the
//  edit box
/////////////////////////////////////////////////////////////////////////////
void CXmitEditCtrl::SetCharFormat(LOGFONT lf, LOGFONT lfs, COLORREF crText, 
					COLORREF crSentText, COLORREF crBackground )
{
	SetBackgroundColor( FALSE, crBackground );

	GetDefaultCharFormat(m_InsertFormat);
	m_InsertFormat.cbSize=sizeof(CHARFORMAT);
	m_InsertFormat.dwMask = CFM_BOLD|CFM_ITALIC|CFM_UNDERLINE|CFM_STRIKEOUT|
								CFM_CHARSET|CFM_COLOR|CFM_FACE|CFM_SIZE;
	m_InsertFormat.crTextColor = crText;
	m_InsertFormat.yHeight = -lf.lfHeight*20;
	m_InsertFormat.bCharSet = lf.lfCharSet;
	m_InsertFormat.dwEffects = 0;
	if(lf.lfItalic)
		m_InsertFormat.dwEffects |= CFE_ITALIC;
	if(lf.lfUnderline)
		m_InsertFormat.dwEffects |= CFE_UNDERLINE;
	if(lf.lfStrikeOut)
		m_InsertFormat.dwEffects |= CFE_STRIKEOUT;
	if(lf.lfWeight > 500)
		m_InsertFormat.dwEffects |= CFE_BOLD;
	m_InsertFormat.bPitchAndFamily = lf.lfPitchAndFamily;
	strcpy( m_InsertFormat.szFaceName, lf.lfFaceName );
	SetDefaultCharFormat(m_InsertFormat);
	SetSelectionCharFormat(m_InsertFormat);
//
	GetDefaultCharFormat(m_RemovedFormat);
	m_RemovedFormat.cbSize=sizeof(CHARFORMAT);
	m_RemovedFormat.dwMask = CFM_BOLD|CFM_ITALIC|CFM_UNDERLINE|CFM_STRIKEOUT|
								CFM_CHARSET|CFM_COLOR|CFM_FACE|CFM_SIZE;
	m_RemovedFormat.crTextColor = crSentText;
	m_RemovedFormat.yHeight = -lfs.lfHeight*20;
	m_RemovedFormat.bCharSet = lfs.lfCharSet;
	m_RemovedFormat.dwEffects = 0;
	if(lfs.lfItalic)
		m_RemovedFormat.dwEffects |= CFE_ITALIC;
	if(lfs.lfUnderline)
		m_RemovedFormat.dwEffects |= CFE_UNDERLINE;
	if(lfs.lfStrikeOut)
		m_RemovedFormat.dwEffects |= CFE_STRIKEOUT;
	if(lfs.lfWeight > 500)
		m_RemovedFormat.dwEffects |= CFE_BOLD;
	m_RemovedFormat.bPitchAndFamily = lf.lfPitchAndFamily;
	strcpy( m_RemovedFormat.szFaceName, lf.lfFaceName );
	m_XmitPos = 0;
}

/////////////////////////////////////////////////////////////////////////////
//Clears the entire xmit edit screen
/////////////////////////////////////////////////////////////////////////////
void CXmitEditCtrl::ClearAll()
{
BYTE temp[2]={0,0};
	SetSel(0,-1);
	ReplaceSel( (CHAR*)temp );
	SetSelectionCharFormat(m_InsertFormat);
	m_XmitPos = 0;
	m_LastEndPosition = GetWindowTextLength();
	fnClearTXBuffer();
}

/////////////////////////////////////////////////////////////////////////////
//Clears just the text that has already been sent
/////////////////////////////////////////////////////////////////////////////
void CXmitEditCtrl::ClearSentText()
{
BYTE temp[2]={0,0};
	SetSel(0,m_XmitPos);
	ReplaceSel( (CHAR*)temp );
	SetSelectionCharFormat(m_InsertFormat);
	m_XmitPos = 0;
}

/////////////////////////////////////////////////////////////////////////////
//restores format of all the box on drag and drop events
/////////////////////////////////////////////////////////////////////////////
void CXmitEditCtrl::OnUpdate() 
{
CHARRANGE cr;	
INT TotalLength;
INT xyz;
	TotalLength = GetWindowTextLength();
	if(	!m_OnChangeLock)
	{
		if( (GetFocus() != this) ||  // if is from drag-drop or paste
			((m_LastEndPosition) < TotalLength))
		{
			if( GetModify() )
			{
				SetFocus();
				SetSel(m_LastEndPosition,TotalLength);
				m_LastEndPosition = TotalLength;
				xyz = fnSendTXString( (PCHAR)(LPCTSTR)GetSelText() );
				m_XmitPos = TotalLength - fnGetTXCharsRemaining();
				cr.cpMax = m_XmitPos;
				cr.cpMin = 0;
				SetSel(cr);						// select text already sent
				SetSelectionCharFormat(m_RemovedFormat);	//format it
				SetSel(m_XmitPos,TotalLength);
				SetSelectionCharFormat(m_InsertFormat);		//return to normal
				SetSel(TotalLength,TotalLength);
			}
		}
	}
	m_LastEndPosition = TotalLength;
	m_OnChangeLock = FALSE;
	SetModify(FALSE);
}

void CXmitEditCtrl::UpdateFormats()
{
CHARRANGE cr;	
INT TotalLength = GetWindowTextLength();
	m_OnChangeLock = TRUE;
	HideSelection( TRUE,FALSE );	//keeps flicker to minimum
	m_XmitPos = TotalLength - fnGetTXCharsRemaining();
	cr.cpMax = m_XmitPos;
	cr.cpMin = 0;
	SetSel(cr);						// select text already sent
	SetSelectionCharFormat(m_RemovedFormat);	//format it
	SetSel(m_XmitPos,TotalLength);
	SetSelectionCharFormat(m_InsertFormat);		//return to normal
	SetSel(TotalLength,TotalLength);
	HideSelection( FALSE,FALSE );
}

/////////////////////////////////////////////////////////////////////////////
// CXmitEditCtrl message handlers
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// bring up the clear/cut/paste/undo menu on right clicks to this edit box
/////////////////////////////////////////////////////////////////////////////
void CXmitEditCtrl::OnRButtonUp(UINT nFlags, CPoint point) 
{
CMenu Menu;
	VERIFY(Menu.LoadMenu(IDR_XMITCLIPMENU));
	CRichEditCtrl::OnRButtonUp(nFlags, point);

	CMenu* pPopup = Menu.GetSubMenu(0);
	ASSERT( pPopup);
// Display the edit shortcut menu items that are valid.
	if( OKCutClip() )
	{
		pPopup->EnableMenuItem(ID_EDIT_CUT, MF_ENABLED|MF_BYCOMMAND);
		pPopup->EnableMenuItem(ID_EDIT_CLEAR, MF_ENABLED|MF_BYCOMMAND);
	}
	else
	{
		pPopup->EnableMenuItem(ID_EDIT_CUT, MF_GRAYED);
		pPopup->EnableMenuItem(ID_EDIT_CLEAR, MF_GRAYED);
	}
	if(::IsClipboardFormatAvailable(CF_TEXT))
		pPopup->EnableMenuItem(ID_EDIT_PASTE, MF_ENABLED|MF_BYCOMMAND);
	else
		pPopup->EnableMenuItem(ID_EDIT_PASTE, MF_GRAYED);
	if(	CanUndo() )
		pPopup->EnableMenuItem(ID_EDIT_UNDO, MF_ENABLED|MF_BYCOMMAND);
	else
		pPopup->EnableMenuItem(ID_EDIT_UNDO, MF_GRAYED);

	ClientToScreen( &point );
	pPopup->TrackPopupMenu(	TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
						point.x, point.y, this); 
}

////////////////////////////////////////////////////////////////////////
//    Clipboard operation functions
////////////////////////////////////////////////////////////////////////
void CXmitEditCtrl::OnEditClear() 
{
	Clear();	
}

void CXmitEditCtrl::OnEditCut() 
{
	Cut();
}

void CXmitEditCtrl::OnEditPaste() 
{
	Paste();
}

void CXmitEditCtrl::OnEditUndo() 
{
	Undo();
}
BOOL CXmitEditCtrl::OKCutClip()
{
CHARRANGE cr;	
	GetSel(cr);
	return( cr.cpMax>cr.cpMin);
}

BOOL CXmitEditCtrl::OKClearSel()
{
CHARRANGE cr;	
	GetSel(cr);
	return( cr.cpMax>cr.cpMin);

}


////////////////////////////////////////////////////////////////////
// Intercept key messages to this rich edit control
////////////////////////////////////////////////////////////////////
BOOL CXmitEditCtrl::PreTranslateMessage(MSG* pMsg) 
{
CXmitView* pView;
CWinPSKDoc* pDoc;
	switch (pMsg->message)
	{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			switch (pMsg->wParam)
			{
				case VK_F1:
				case VK_F2:
				case VK_F3:
				case VK_F4:
				case VK_F5:
				case VK_F6:
				case VK_F7:
				case VK_F8:
				case VK_F9:
				case VK_F10:
				case VK_F11:
				case VK_F12:
					pView = (CXmitView*)GetParent();
					ASSERT_VALID(pView);
					pDoc = (CWinPSKDoc*)pView->GetDocument();
					if( (pMsg->message != WM_SYSKEYDOWN) ||
										(pMsg->wParam == VK_F10) )
					{
						((CWinPSKView*)pDoc->m_pWinPSKView)->ProcessFKeys(pMsg->wParam);
						return TRUE;  //don't pass these keys on.
					}
					break;
				case VK_RIGHT:
				case VK_LEFT:
					pView = (CXmitView*)GetParent();
					ASSERT_VALID(pView);
					pDoc = (CWinPSKDoc*)pView->GetDocument();
					if(((CWinPSKView*)pDoc->m_pWinPSKView)->m_ProgramState==STATE_RX)
					{
						((CWinPSKView*)pDoc->m_pWinPSKView)->SetFocus();
						return TRUE;  //don't pass these keys on.
					}
					else
					{
						if(pMsg->wParam == VK_LEFT)
							pMsg->wParam = VK_BACK;
					}
					break;
			}
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			switch (pMsg->wParam)
			{
				case VK_RIGHT:
				case VK_LEFT:
				case VK_F10:
					return TRUE;	//don't let system have F10
					break;
			}
			break;
	}
	return CRichEditCtrl::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// Called to insert a CString into the TX text from a macro call.
/////////////////////////////////////////////////////////////////////////////
void CXmitEditCtrl::InsertCString(CString Cstr)
{
CString sExpanded = Cstr;
LONG TotalLength = GetWindowTextLength();
INT FileStart,FileEnd;
CString FileName;
CString str;
CWinPSKDoc* pDoc = (CWinPSKDoc*)((CXmitView*)GetParent() )->GetDocument();
	SetFocus();
BOOL Done = FALSE;
	do
	{
		FileStart = sExpanded.Find( _T("<"),0);
		FileEnd = sExpanded.Find( _T(">"),FileStart);
		if( FileEnd > FileStart )	//need to insert a file??
		{
			FileName = sExpanded.Mid(FileStart+1, FileEnd-FileStart-1);
			sExpanded.Replace( FileName, _T("") );
			InsertFile( sExpanded, FileName);
		}	
		else
		{
			Done = TRUE;
		}
	}while( !Done);
	
	sExpanded.Replace( _T("'mycall'"), pDoc->m_pSettings->m_sMyCall );
	sExpanded.Replace( _T("'theircall'"), pDoc->m_sTheirCall );
	sExpanded.Replace( _T("'theirname'"), pDoc->m_sTheirName );
	sExpanded.Replace( _T("'textgrab'"), pDoc->m_sTextGrab );
	CTime time = CTime::GetCurrentTime();
	if( pDoc->m_pSettings->m_UseLocalTime )
		str = time.Format(_T("%#d %b %Y %#I:%M:%S %p"));
	else
		str = time.FormatGmt(_T("%#d %b %Y %H:%M:%S UTC"));
	sExpanded.Replace( _T("'datetime'"), str );

	if( sExpanded.Replace( _T("'stop'"), _T("") ) )
		fnSendTXCharacter( DLL_AUTOSTOP,TRUE);
	if( sExpanded.Replace( _T("'cwid'"), _T("") ) )
		fnSendTXCharacter( DLL_USECWID,TRUE);
	if( sExpanded.Replace( _T("'start'"), _T("") ) )
		pDoc->m_MacroStartTX = TRUE;

	m_OnChangeLock = TRUE;
	SetSel(TotalLength,TotalLength);
	ReplaceSel( sExpanded );
	sExpanded.Replace( _T("\330"), _T("0") );	//convert any slash Zeros to 0
	fnSendTXString( (PCHAR)(LPCTSTR)sExpanded );
	UpdateFormats();
	TotalLength = GetWindowTextLength();
	SetSel(TotalLength,TotalLength);
	m_LastEndPosition = TotalLength;
}


/////////////////////////////////////////////////////////////////////////////
//  Reads "FileName" file into TX text stream
/////////////////////////////////////////////////////////////////////////////
void CXmitEditCtrl::InsertFile(CString &str, CString FileName)
{
CFile cFile;
PBYTE pBuf = NULL;
CString sTmp;
	if( cFile.Open( FileName, CFile::modeRead) )
	{
		DWORD length = cFile.GetLength();
		if( length > FILE_TEXT_LIMIT )
		{
			FileName += _T("-- Is too long\nLimit is 2K Bytes.");
			AfxMessageBox( FileName );
			str.Replace( _T("<>"), _T("") );
		}
		else
		{
			pBuf = new BYTE[FILE_TEXT_LIMIT+1];
			if( cFile.Read( pBuf, length) != length )
			{
				FileName += _T("-- Can't be read");
				AfxMessageBox( FileName );
				str.Replace( _T("<>"), _T("") );
			}
			else
			{
				pBuf[length] = 0;
				sTmp = pBuf;
				str.Replace( _T("<>"), sTmp );
			}
		}
	}
	else
	{
		FileName += _T("-- Can't be opened");
		AfxMessageBox( FileName );
		str.Replace( _T("<>"), _T("") );
	}
	if( pBuf )
		delete pBuf;
}

void CXmitEditCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_OnChangeLock = TRUE;
	for(UINT i=0; i<nRepCnt; i++)
		fnSendTXCharacter( nChar, FALSE);
	m_LastEndPosition = GetWindowTextLength();
	CRichEditCtrl::OnChar(nChar, nRepCnt, nFlags);
}
