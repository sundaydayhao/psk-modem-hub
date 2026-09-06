// SquelchCtrl.cpp : implementation file
//
//

#include "stdafx.h"
#include "winpsk.h"
#include "SquelchCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MIN_POS 0						//control range
#define MAX_POS 100

#define SQON_COLOR RGB(255,255,0)		//yellow squelch on
#define SQOFF_COLOR RGB(128,128,128)	//yucky grey squelch off
#define THRESH_COLOR RGB(0,192,192)		//light cyan threshold bar color
#define BACK_COLOR RGB(0,0,0)			// black background
/////////////////////////////////////////////////////////////////////////////
// CSquelchCtrl

CSquelchCtrl::CSquelchCtrl()
{
	m_nLevel = 0;
	m_Threshold = 0;
	m_pParentWnd = NULL;
	m_pCallback = NULL;
}

CSquelchCtrl::~CSquelchCtrl()
{
}


BEGIN_MESSAGE_MAP(CSquelchCtrl, CWnd)
	//{{AFX_MSG_MAP(CSquelchCtrl)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
//  Creates the window
///////////////////////////////////////////////////////////////////////////
BOOL CSquelchCtrl::Create( LPCTSTR lpszWindowName, DWORD dwStyle,
						  const RECT& rect, CWnd* pParentWnd, UINT nID,
 							PCALLBACKFUNCT pCallback)
{
	m_pParentWnd = pParentWnd;
	m_pCallback = pCallback;
	m_rPlot = rect;			// client plot screen rectangle coordinates
	static CString className = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW);
	return CWnd::Create( className, lpszWindowName, dwStyle,
							rect, pParentWnd, nID);
}

///////////////////////////////////////////////////////////////////////////
//  Set signal level
///////////////////////////////////////////////////////////////////////////
void CSquelchCtrl::SetLevel(INT nLevel)
{
CRect r;
	m_nLevel = (m_rPlot.Height()*(MAX_POS-nLevel))/(MAX_POS-MIN_POS);
	GetClientRect(r);
	InvalidateRect(r);
}

///////////////////////////////////////////////////////////////////////////
//  Set threshold level
///////////////////////////////////////////////////////////////////////////
void CSquelchCtrl::SetThreshold(INT nLevel)
{
	m_Threshold = (m_rPlot.Height()*(MAX_POS-nLevel))/(MAX_POS-MIN_POS);
}

///////////////////////////////////////////////////////////////////////////
//  calc the current threshold value from control position
///////////////////////////////////////////////////////////////////////////
INT CSquelchCtrl::GetThreshold()
{
	return (MAX_POS - (m_Threshold*MAX_POS)/m_rPlot.Height() );
}


/////////////////////////////////////////////////////////////////////////////
// CSquelchCtrl message handlers
//
//       Called to draw the control
/////////////////////////////////////////////////////////////////////////////
void CSquelchCtrl::OnPaint() 
{
CDC MemDC;
CPaintDC dc(this); // device context for painting
//create various rectangles for all the viewable things
	GetClientRect( m_rClient );	

	if( m_bmImage.m_hObject != NULL )	// delete old bitmap object
		m_bmImage.DeleteObject();
//create a new bitmap with current screen DC settings
	m_bmImage.CreateCompatibleBitmap(&dc,m_rClient.Width(),m_rClient.Height() );
//create memory DC pointer to the image bitmap
	MemDC.CreateCompatibleDC( &dc);
	CBitmap* pbmOld = MemDC.SelectObject( &m_bmImage );

//Create 2 rectangles for the meter bar depending upon m_nLevel. 	
	CRect rAct = m_rClient;
	CRect rNAct = m_rClient;
	rAct.top = m_nLevel;
	rNAct.bottom = m_nLevel;

//Fill active meter bar with bar color depending on squelch state
	if( rAct.top < m_Threshold )
	{
		m_SquelchOn = TRUE;
		MemDC.FillSolidRect(rAct,SQON_COLOR);
	}
	else
	{
		m_SquelchOn = FALSE;
		MemDC.FillSolidRect(rAct,SQOFF_COLOR);
	}
//Fill non active meter bar with background color
	MemDC.FillSolidRect(rNAct,BACK_COLOR);
// Draw threshold line/rectangle thingy
	CRect rThresh( m_rClient.left, m_Threshold, m_rClient.right, m_Threshold+1);
	MemDC.FillSolidRect(rThresh,THRESH_COLOR);
	rThresh.SetRect( m_rClient.Width()/3, m_Threshold-3,
				2*m_rClient.Width()/3, m_Threshold+4);
	MemDC.FillSolidRect(rThresh,THRESH_COLOR);
// Copy bitmap into screen context
	dc.BitBlt(	m_rClient.left, m_rClient.top,		//dest left,top
				m_rClient.right, m_rClient.bottom,	//dest width, Height
				&MemDC,							//src dc
				0,0,							//src left, top
				SRCCOPY);
//deselect bitmap
	MemDC.SelectObject( pbmOld );	// from DC}
}


/////////////////////////////////////////////////////////////////////////////
// change threshold when mouse button is clicked in the control
/////////////////////////////////////////////////////////////////////////////
void CSquelchCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
CRect r;
INT thresh;
	m_Threshold = point.y;
	thresh = GetThreshold();			//round off m_Threshold
	SetThreshold(thresh);
	GetClientRect(r);
	InvalidateRect(r);
	if( m_pParentWnd && m_pCallback)
		(m_pParentWnd->*m_pCallback)( thresh );
	CWnd::OnLButtonDown(nFlags, point);
}
