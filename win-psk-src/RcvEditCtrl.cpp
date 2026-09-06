// RcvEditCtrl.cpp : implementation file
//
//

#include "stdafx.h"
#include "winpsk.h"
#include "winpskdoc.h"
#include "winpskview.h"
#include "RcvView.h"
#include "RcvEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define QUEUE_MOD_MASK 0x00FF
#define ID_TIMER 6
#define RX_ID 0
#define TX_ID -1


/////////////////////////////////////////////////////////////////////////////
// CRcvEditCtrl

CRcvEditCtrl::CRcvEditCtrl()
{
	m_LineCount = 0;
	m_QHead = 0;
	m_QTail = 0;
	m_TimerID = 0;
	m_LastFont = RX_ID;
	m_CTRLKeyDown = FALSE;
	m_TXActive = FALSE;
}

CRcvEditCtrl::~CRcvEditCtrl()
{
}


BEGIN_MESSAGE_MAP(CRcvEditCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CRcvEditCtrl)
	ON_WM_CREATE()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_WM_RBUTTONUP()
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillfocus)
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRcvEditCtrl message handlers
int CRcvEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_bUseSlash = FALSE;
	return 0;
}

// Initializes the Font, font color, and background color for the
//  edit box
void CRcvEditCtrl::SetCharFormat(LOGFONT lf, LOGFONT lfs,  COLORREF crText, 
							COLORREF crSentText,COLORREF crBackground )
{
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
//
	GetDefaultCharFormat(m_SentFormat);
	m_SentFormat.cbSize=sizeof(CHARFORMAT);
	m_SentFormat.dwMask = CFM_BOLD|CFM_ITALIC|CFM_UNDERLINE|CFM_STRIKEOUT|
								CFM_CHARSET|CFM_COLOR|CFM_FACE|CFM_SIZE;
	m_SentFormat.crTextColor = crSentText;
	m_SentFormat.yHeight = -lfs.lfHeight*20;
	m_SentFormat.bCharSet = lfs.lfCharSet;
	m_SentFormat.dwEffects = 0;
	if(lfs.lfItalic)
		m_SentFormat.dwEffects |= CFE_ITALIC;
	if(lfs.lfUnderline)
		m_SentFormat.dwEffects |= CFE_UNDERLINE;
	if(lfs.lfStrikeOut)
		m_SentFormat.dwEffects |= CFE_STRIKEOUT;
	if(lfs.lfWeight > 500)
		m_SentFormat.dwEffects |= CFE_BOLD;
	m_SentFormat.bPitchAndFamily = lf.lfPitchAndFamily;
	strcpy( m_SentFormat.szFaceName, lf.lfFaceName );

	SetBackgroundColor( FALSE, crBackground );
	SetDefaultCharFormat(m_InsertFormat);
	SetSelectionCharFormat(m_InsertFormat);
}

///////////////////////////////////////////////////////////////////////////////
// Message handler when a new character is received and needs to be placed into
//    the receive edit box.
///////////////////////////////////////////////////////////////////////////////
void CRcvEditCtrl::OnRcvChar(UINT ch, LONG src)
{
CHARRANGE cr;
	if( !PutQue((BYTE)ch) || m_RcvCancel )	//if cancel selection flag is set
	{										// or que is full, then need to
		m_RcvCancel = FALSE;				// deselect any text.
		CancelSelection();
	}
	else
	{
		GetSel(cr);				// if a text selection is made
		if( cr.cpMax>cr.cpMin)	// then hold off anymore writes to this
		{						// edit box for about 10 seconds
			if( m_TimerID == 0)
				m_TimerID = SetTimer( ID_TIMER, 10000, NULL);	//start up timer
			return;
		}
	}
	if(	m_LastFont != src )		// if font is changing
	{
		if( src == RX_ID )
		{
			if( !m_TXActive )
			{
				SetSelectionCharFormat(m_InsertFormat);
				m_LastFont = src;
			}
			else
			{
				m_QHead = 0;	//clear out any remaining recv chars
				m_QTail = 0;
				m_LastFont = TX_ID;
			}
		}
		else
		{
			SetSelectionCharFormat(m_SentFormat);
			m_LastFont = src;
		}
	}
	while( CheckQue() )	// if any chars in que send em all to window
	{
		PutCharInWindow( GetQue() );
	}
}

///////////////////////////////////////////////////////////////////////////////
//returns TRUE if there is a character in the receive Queue
///////////////////////////////////////////////////////////////////////////////
BOOL CRcvEditCtrl::CheckQue()
{
	return( m_QHead != m_QTail );
}

///////////////////////////////////////////////////////////////////////////////
//returns TRUE if the character is put in the Queue
// returns FALSE if the que is full
///////////////////////////////////////////////////////////////////////////////
BOOL CRcvEditCtrl::PutQue(BYTE ch)
{
INT tmpHead = m_QHead+1;
	tmpHead = tmpHead&QUEUE_MOD_MASK;
	if( tmpHead == m_QTail )
		return FALSE;			//que is full
	else
	m_RcvQue[m_QHead] = ch;
	m_QHead = tmpHead;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// returns the next character from the queue.
// user must call CheckQue() before calling GetQue().
///////////////////////////////////////////////////////////////////////////////
BYTE CRcvEditCtrl::GetQue()
{
BYTE ch = m_RcvQue[m_QTail];
	m_QTail = (m_QTail+1)&QUEUE_MOD_MASK;
	return ch;
}

///////////////////////////////////////////////////////////////////////////////
// Places ch into the rich edit window after dealing with backspace, and
//		CR LF's and slash zero characters.
///////////////////////////////////////////////////////////////////////////////
void CRcvEditCtrl::PutCharInWindow(BYTE ch)
{
INT TotalLength = GetWindowTextLength();
BYTE temp[3]={0,0,0};
INT cnt = 0;
	temp[0] = ch;
	if( temp[0] == BACK_SPACE_CODE )	//deal with a backspace character
	{
		if(TotalLength>1)
		{
			SetSel(TotalLength-1,TotalLength);
			temp[0] = 0;
			ReplaceSel( (CHAR*)temp );
		}
	}
	else		//here with printable text so put it on screen
	{
		if( (temp[0] == 0x0D) /*|| (temp[0] == 0x0A)*/ || (temp[0] >= ' ') )
		{
			if( temp[0] == 0x0D)	//CR
				temp[1] = 0x0A;
			if(temp[0] == '0' && m_bUseSlash)// convert zero to slash zero
				temp[0] = (BYTE)216;
			SetSel(TotalLength,TotalLength);
			ReplaceSel( (CHAR*)temp );
			if( (cnt = GetLineCount() ) != m_LineCount)
			{
				LineScroll(cnt-m_LineCount,0);
				m_LineCount = cnt;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Clear the whole edit box
///////////////////////////////////////////////////////////////////////////////
void CRcvEditCtrl::ClearAll()
{
BYTE temp[2]={0,0};
	SetSelectionCharFormat(m_InsertFormat);
	SetSel(0,-1);
	ReplaceSel( (CHAR*)temp );
	m_LastFont = TX_ID;
}

///////////////////////////////////////////////////////////////////////////////
//  Cancels any text selection
///////////////////////////////////////////////////////////////////////////////
void CRcvEditCtrl::CancelSelection()
{
INT TotalLength = GetWindowTextLength();
	if( (TotalLength>1) )
		SetSel(TotalLength,TotalLength);
	m_RcvCancel = FALSE;
	if( m_TimerID == ID_TIMER)
	{
		KillTimer(m_TimerID);
		m_TimerID = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
//  Clipboard functions
///////////////////////////////////////////////////////////////////////////////
void CRcvEditCtrl::OnEditCopy() 
{
	CopyClip();
}

void CRcvEditCtrl::CopyClip()
{
	Copy();
	m_RcvCancel = TRUE;
//Nasty Casting to set window focus to Xmit window;
	((CWinPSKDoc*)((CRcvView*)( (CRcvView*)GetParent() )->
				GetDocument()))->m_pXmitEdit->SetFocus();
}

BOOL CRcvEditCtrl::OKCopy()
{
CHARRANGE cr;	
	GetSel(cr);
	return( cr.cpMax>cr.cpMin);
}

///////////////////////////////////////////////////////////////////////////////
//  Intercept some key strokes
///////////////////////////////////////////////////////////////////////////////
BOOL CRcvEditCtrl::PreTranslateMessage(MSG* pMsg) 
{
CRcvView* pView;
CWinPSKDoc* pDoc;
// If a key is pressed while focus is in the receive edit window,
//    pass the key message on to the Xmit window and set focus there too;
	if( (pMsg->message == WM_KEYDOWN) || (pMsg->message == WM_SYSKEYDOWN) )
	{
		switch (pMsg->wParam)
		{
			case VK_CONTROL:		//key exclusions not passed on
				m_CTRLKeyDown = TRUE;
				break;
			default:
				if( !m_CTRLKeyDown || ((pMsg->message == WM_SYSKEYDOWN)
											&& (pMsg->wParam==VK_F10)))
				{
					pView = (CRcvView*)GetParent();
					ASSERT_VALID(pView);
					pDoc = (CWinPSKDoc*)pView->GetDocument();
					ASSERT_VALID(pDoc);
					pDoc->m_pXmitEdit->PostMessage(pMsg->message ,
										pMsg->wParam,
										pMsg->lParam);
					pDoc->m_pXmitEdit->SetFocus();
					POINT Point;
					::GetCursorPos(&Point);
					::SetCursorPos(Point.x, Point.y);
				}
				break;
		}
	}
	else
		if(pMsg->message == WM_KEYUP)
			m_CTRLKeyDown = FALSE;
	return CRichEditCtrl::PreTranslateMessage(pMsg);
}

///////////////////////////////////////////////////////////////////////////////
// bring up the clipboard copy menu on right clicks to this edit box
///////////////////////////////////////////////////////////////////////////////
void CRcvEditCtrl::OnRButtonUp(UINT nFlags, CPoint point) 
{
CRcvView* pView;
CWinPSKDoc* pDoc;
CHARRANGE cr;	
	pView = (CRcvView*)GetParent();
	ASSERT_VALID(pView);
	pDoc = (CWinPSKDoc*)pView->GetDocument();
	ASSERT_VALID(pDoc);
	CRichEditCtrl::OnRButtonUp(nFlags, point);
	if( pDoc->m_pSettings->m_AutoTxtSel)
	{
		CString str;	
		GetSel(cr);
		if(  (cr.cpMax-cr.cpMin) > 1 )		//if selected text is more than one character
		{
			str = GetSelText();
			CancelSelection();
			pDoc->m_pTextGrabEdit->InsertCString( str );
		}
	}
	else
	{
		CMenu Menu;
		VERIFY(Menu.LoadMenu(IDR_RCVCLIPMENU));

		CMenu* pPopup = Menu.GetSubMenu(0); 
		// Display the edit copy shortcut menu if anything is selected.
		GetSel(cr);
		if( cr.cpMax>cr.cpMin)
			pPopup->EnableMenuItem(ID_EDIT_COPY, MF_ENABLED|MF_BYCOMMAND);
		else
			pPopup->EnableMenuItem(ID_EDIT_COPY, MF_GRAYED);
		ClientToScreen( &point );
		pPopup->TrackPopupMenu(	TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
						point.x, point.y, this); 
	}
}

//////////////////////////////////////////////////////////////
//  Here if someone double clicks in the rx window.
//    If it is on a word, it is selected.
//////////////////////////////////////////////////////////////
void CRcvEditCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
CString str;	
CRcvView* pView;
CWinPSKDoc* pDoc;
CHARRANGE cr;	
	CRichEditCtrl::OnLButtonDblClk(nFlags, point);
	pView = (CRcvView*)GetParent();
	ASSERT_VALID(pView);
	pDoc = (CWinPSKDoc*)pView->GetDocument();
	ASSERT_VALID(pDoc);
	if( pDoc->m_pSettings->m_AutoTxtSel)
	{
		GetSel(cr);
		if(  (cr.cpMax-cr.cpMin) > 1 )	//if selected text is more than one character
		{
			str = GetSelText();	// if any character is numeric, assume is a callsign
			if( str.FindOneOf(_T("0123456789\330") ) >= 0 )	// \330 is the slash zero
				pDoc->m_pTheirCallEdit->InsertCString( str );
			else											// else assume a name
				pDoc->m_pTheirNameEdit->InsertCString( str );
		}
	}
}



void CRcvEditCtrl::OnKillfocus() 
{
	m_RcvCancel = TRUE;
}


///////////////////////////////////////////////////////////////////////////////
//  Service selection timeout timer
///////////////////////////////////////////////////////////////////////////////
void CRcvEditCtrl::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent == m_TimerID )
	{
		KillTimer(m_TimerID);
		m_TimerID = 0;
		m_RcvCancel = FALSE;				// deselect any text.
		CancelSelection();
	}
	CRichEditCtrl::OnTimer(nIDEvent);
}

///////////////////////////////////////////////////////////////////////////////
//  Inserts a time/date stamp in the RX text stream
///////////////////////////////////////////////////////////////////////////////
void CRcvEditCtrl::TXStamp(BOOL m_EchoTXText, BOOL TXStart, BOOL UseLocalTime)
{
CString Preamb;
CString str;
INT cnt;
INT TotalLength = GetWindowTextLength();
CTime time = CTime::GetCurrentTime();
CRcvView* pView;
CWinPSKDoc* pDoc;
	pView = (CRcvView*)GetParent();
	ASSERT_VALID(pView);
	pDoc = (CWinPSKDoc*)pView->GetDocument();
	ASSERT_VALID(pDoc);
	if(UseLocalTime)
		str = time.Format(_T(" - %#d %b %Y %#I:%M:%S %p >>"));
	else
		str = time.FormatGmt(_T(" - %d %b %Y %H:%M:%S UTC>>"));

	if(pDoc->m_pSettings->m_UseSlash)
		str.Replace(  _T("0") , _T("\330") );	//convert any 0's to slash Zeros
	if(m_EchoTXText)
	{
		if(TXStart)
		{
			m_TXActive = TRUE;
			Preamb = _T("\n<<TX Started");
			SetSelectionCharFormat(m_SentFormat);
			m_LastFont = TX_ID;
		}
		else
		{
			Preamb = _T("\n<<TX Ended ");
			SetSelectionCharFormat(m_SentFormat);
			m_TXActive = FALSE;
		}
		str += _T("\n");
	}
	else
	{
		if(TXStart)
		{
			m_TXActive = TRUE;
			Preamb = _T("\n<<TX Started");
			SetSelectionCharFormat(m_SentFormat);
			m_LastFont = TX_ID;
		}
		else
		{
			Preamb = _T("<<TX Ended");
			m_TXActive = FALSE;
			str += _T("\n");
		}

	}
	str = Preamb + str;
	SetSel(TotalLength,TotalLength);
	ReplaceSel( str );
	if( (cnt = GetLineCount() ) != m_LineCount)
	{
		LineScroll(cnt-m_LineCount,0);
		m_LineCount = cnt;
	}

}
/////////////////////////////////////////////////////////////////////////////
//*******************   CTheirCallCtrl   Class   ****************************
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CTheirCallCtrl

CTheirCallCtrl::CTheirCallCtrl()
{
	m_OldLength = 0;
}

CTheirCallCtrl::~CTheirCallCtrl()
{
}


BEGIN_MESSAGE_MAP(CTheirCallCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CTheirCallCtrl)
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTheirCallCtrl message handlers
int CTheirCallCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetEventMask( ENM_UPDATE);		//enable notify of changes
//
	GetDefaultCharFormat(m_CharFormat);
	m_CharFormat.cbSize=sizeof(CHARFORMAT);
	m_CharFormat.dwMask = CFM_BOLD|CFM_OFFSET|CFM_CHARSET|CFM_COLOR|CFM_FACE|CFM_SIZE;
	m_CharFormat.crTextColor = RGB(0,0,255);
	m_CharFormat.yHeight = 7*20;		//8 pt font
	m_CharFormat.yOffset = 40;			//no idea why this has to be 40
	m_CharFormat.bCharSet = 0;			//otherwise font is not centered
	m_CharFormat.dwEffects = 0;
	m_CharFormat.bPitchAndFamily = FF_SWISS | VARIABLE_PITCH;
	strcpy( m_CharFormat.szFaceName, "Arial" );

	SetSelectionCharFormat(m_CharFormat);
	SetDefaultCharFormat(m_CharFormat);
//
	LimitText(50);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Called to make text upper case and replace existing text
///////////////////////////////////////////////////////////////////////////////
void CTheirCallCtrl::ReFormat()
{
INT TotalLength = GetWindowTextLength();
CString str;
INT start;
CHARRANGE cr;
	if(GetFocus() != this )	// if is a drag-drop event
	{
		GetSel(cr);
		start = cr.cpMax - TotalLength + m_OldLength;
		SetSel( start, cr.cpMax);
		str = GetSelText();
	}
	else					//is typed or clipboard operation
	{
		SetSel(0,TotalLength);
		str = GetSelText();
	}
	str.MakeUpper();
	SetSel(0,TotalLength);
	ReplaceSel(str,FALSE);
	SetSelectionCharFormat(m_CharFormat);
	SetDefaultCharFormat(m_CharFormat);
	TotalLength = GetWindowTextLength();
	if(TotalLength>10)		//limit ext to 10 chars
	{
		SetSel(10 ,TotalLength);
		str = "";
		ReplaceSel(str,FALSE);
		TotalLength = 10;
	}
	SetSel(TotalLength ,TotalLength);
	m_OldLength = TotalLength;
	CWinPSKDoc* pDoc = (CWinPSKDoc*)((CWinPSKView*)GetParent() )->GetDocument();
	pDoc->m_sTheirCall = str;
}

void CTheirCallCtrl::InsertCString(CString Cstr)
{
	SetFocus();
	SetSel(0,-1);
	ReplaceSel( Cstr);
}

void CTheirCallCtrl::ClearAll()
{
BYTE temp[2]={0,0};
	SetFocus();
	SetSel(0,-1);
	ReplaceSel( (CHAR*)temp );
}
///////////////////////////////////////////////////////////////////////////////
//  Service OnUpdate message
///////////////////////////////////////////////////////////////////////////////
void CTheirCallCtrl::OnUpdate() 
{
	if( GetModify() )
	{
		ReFormat();
	}
	SetModify(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
//  Service clipboard paste operation
///////////////////////////////////////////////////////////////////////////////
void CTheirCallCtrl::PasteClip()
{
INT TotalLength = GetWindowTextLength();
	SetSel(0,TotalLength);
	Clear();
	Paste();
}

/////////////////////////////////////////////////////////////////////////////
//*******************   CTheirNameCtrl   Class   ****************************
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CTheirNameCtrl

CTheirNameCtrl::CTheirNameCtrl()
{
	m_OldLength = 0;
}

CTheirNameCtrl::~CTheirNameCtrl()
{
}


BEGIN_MESSAGE_MAP(CTheirNameCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CTheirNameCtrl)
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTheirNameCtrl message handlers
int CTheirNameCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetEventMask( ENM_UPDATE);		//enable notify of changes
//
	GetDefaultCharFormat(m_CharFormat);
	m_CharFormat.cbSize=sizeof(CHARFORMAT);
	m_CharFormat.dwMask = CFM_BOLD|CFM_OFFSET|CFM_CHARSET|CFM_COLOR|CFM_FACE|CFM_SIZE;
	m_CharFormat.crTextColor = RGB(0,0,255);
	m_CharFormat.yHeight = 7*20;		//7 pt font
	m_CharFormat.yOffset = 40;			//no idea why this has to be 40
	m_CharFormat.bCharSet = 0;			//otherwise font is not centered
	m_CharFormat.dwEffects = 0;
	m_CharFormat.bPitchAndFamily = FF_SWISS | VARIABLE_PITCH;
	strcpy( m_CharFormat.szFaceName, "Arial" );

	SetSelectionCharFormat(m_CharFormat);
	SetDefaultCharFormat(m_CharFormat);
//
	LimitText(50);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Called to make first letter upper case and replace existing text
///////////////////////////////////////////////////////////////////////////////
void CTheirNameCtrl::ReFormat()
{
INT TotalLength = GetWindowTextLength();
CString str;
CString strtemp;
TCHAR ctemp;
INT start;
CHARRANGE cr;
	if(GetFocus() != this )	// if is a drag-drop event
	{
		GetSel(cr);
		start = cr.cpMax - TotalLength + m_OldLength;
		SetSel( start, cr.cpMax);
		str = GetSelText();
	}
	else					//is typed or clipboard operation
	{
		SetSel(0,TotalLength);
		str = GetSelText();
	}
	if( !str.IsEmpty() )
	{
		str.MakeLower();
		strtemp = str.Left(1);	//get first character
		strtemp.MakeUpper();	//convert to Upper case
		ctemp = strtemp.GetAt(0);
		str.SetAt(0,ctemp);
	}

	SetSel(0,TotalLength);
	ReplaceSel(str,FALSE);
	SetSelectionCharFormat(m_CharFormat);
	SetDefaultCharFormat(m_CharFormat);
	TotalLength = GetWindowTextLength();
	if(TotalLength>18)		//limit ext to 18 chars
	{
		SetSel(18 ,TotalLength);
		str = "";
		ReplaceSel(str,FALSE);
		TotalLength = 18;
	}
	SetSel(TotalLength ,TotalLength);
	m_OldLength = TotalLength;
	CWinPSKDoc* pDoc = (CWinPSKDoc*)((CWinPSKView*)GetParent() )->GetDocument();
	pDoc->m_sTheirName = str;
}


void CTheirNameCtrl::InsertCString(CString Cstr)
{
	SetFocus();
	SetSel(0,-1);
	ReplaceSel( Cstr);
}

///////////////////////////////////////////////////////////////////////////////
//  Service OnUpdate message
///////////////////////////////////////////////////////////////////////////////
void CTheirNameCtrl::OnUpdate() 
{
	if( GetModify() )
	{
		ReFormat();
	}
	SetModify(FALSE);
}

void CTheirNameCtrl::ClearAll()
{
BYTE temp[2]={0,0};
	SetFocus();
	SetSel(0,-1);
	ReplaceSel( (CHAR*)temp );
}

///////////////////////////////////////////////////////////////////////////////
//  Service clipboard paste operation
///////////////////////////////////////////////////////////////////////////////
void CTheirNameCtrl::PasteClip()
{
INT TotalLength = GetWindowTextLength();
	SetSel(0,TotalLength);
	Clear();
	Paste();
}

/////////////////////////////////////////////////////////////////////////////
//*******************   CTextGrabCtrl   Class   ****************************
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CTextGrabCtrl

CTextGrabCtrl::CTextGrabCtrl()
{
	m_OldLength = 0;
}

CTextGrabCtrl::~CTextGrabCtrl()
{
}


BEGIN_MESSAGE_MAP(CTextGrabCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CTextGrabCtrl)
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextGrabCtrl message handlers
int CTextGrabCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetEventMask( ENM_UPDATE);		//enable notify of changes
//
	GetDefaultCharFormat(m_CharFormat);
	m_CharFormat.cbSize=sizeof(CHARFORMAT);
	m_CharFormat.dwMask = CFM_BOLD|CFM_OFFSET|CFM_CHARSET|CFM_COLOR|CFM_FACE|CFM_SIZE;
	m_CharFormat.crTextColor = RGB(0,0,255);
	m_CharFormat.yHeight = 7*20;		//7 pt font
	m_CharFormat.yOffset = 40;			//no idea why this has to be 40
	m_CharFormat.bCharSet = 0;			//otherwise font is not centered
	m_CharFormat.dwEffects = 0;
	m_CharFormat.bPitchAndFamily = FF_SWISS | VARIABLE_PITCH;
	strcpy( m_CharFormat.szFaceName, "Arial" );

	SetSelectionCharFormat(m_CharFormat);
	SetDefaultCharFormat(m_CharFormat);
//
	LimitText(50);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Called to make first letter upper case and replace existing text
///////////////////////////////////////////////////////////////////////////////
void CTextGrabCtrl::ReFormat()
{
INT TotalLength = GetWindowTextLength();
CString str;
CString strtemp;
TCHAR ctemp;
INT start;
CHARRANGE cr;
	if(GetFocus() != this )	// if is a drag-drop event
	{
		GetSel(cr);
		start = cr.cpMax - TotalLength + m_OldLength;
		SetSel( start, cr.cpMax);
		str = GetSelText();
	}
	else					//is typed or clipboard operation
	{
		SetSel(0,TotalLength);
		str = GetSelText();
	}
	if( !str.IsEmpty() )
	{
		strtemp = str.Left(1);	//get first character
		strtemp.MakeUpper();	//convert to Upper case
		ctemp = strtemp.GetAt(0);
		str.SetAt(0,ctemp);
	}

	SetSel(0,TotalLength);
	ReplaceSel(str,FALSE);
	SetSelectionCharFormat(m_CharFormat);
	SetDefaultCharFormat(m_CharFormat);
	TotalLength = GetWindowTextLength();
	if(TotalLength>45)		//limit ext to 45 chars
	{
		SetSel(45 ,TotalLength);
		str = "";
		ReplaceSel(str,FALSE);
		TotalLength = 45;
	}
	SetSel(TotalLength ,TotalLength);
	m_OldLength = TotalLength;
	CWinPSKDoc* pDoc = (CWinPSKDoc*)((CWinPSKView*)GetParent() )->GetDocument();
	pDoc->m_sTextGrab = str;
}

void CTextGrabCtrl::InsertCString(CString Cstr)
{
	SetFocus();
	SetSel(0,-1);
	ReplaceSel( Cstr);
}

void CTextGrabCtrl::ClearAll()
{
BYTE temp[2]={0,0};
	SetFocus();
	SetSel(0,-1);
	ReplaceSel( (CHAR*)temp );
}

///////////////////////////////////////////////////////////////////////////////
//  Service OnUpdate message
///////////////////////////////////////////////////////////////////////////////
void CTextGrabCtrl::OnUpdate() 
{
	if( GetModify() )
	{
		ReFormat();
	}
	SetModify(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
//  Service clipboard paste operation
///////////////////////////////////////////////////////////////////////////////
void CTextGrabCtrl::PasteClip()
{
INT TotalLength = GetWindowTextLength();
	SetSel(0,TotalLength);
	Clear();
	Paste();
}



