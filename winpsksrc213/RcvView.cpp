// RcvView.cpp : implementation file
//
//

#include "stdafx.h"
#include "winpsk.h"
#include "WinPSKDoc.h"
#include "RcvView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRcvView

IMPLEMENT_DYNCREATE(CRcvView, CView)

CRcvView::CRcvView()
{
}

CRcvView::~CRcvView()
{
}


BEGIN_MESSAGE_MAP(CRcvView, CView)
	//{{AFX_MSG_MAP(CRcvView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRcvView drawing

void CRcvView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CRcvView diagnostics

#ifdef _DEBUG
void CRcvView::AssertValid() const
{
	CView::AssertValid();
}

void CRcvView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRcvView message handlers

void CRcvView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	CWinPSKDoc* pDoc = (CWinPSKDoc*) GetDocument();
	if (pDoc != NULL)
	{
		if (pDoc->m_pRcvEdit != NULL)
			pDoc->m_pRcvEdit->MoveWindow(0,0,cx,cy);
	}	
	
}

///////////////////////////////////////////////////////////////////////////////
//  Connect the Rx CRichEditCtrl to this view which is in a splitter pane
///////////////////////////////////////////////////////////////////////////////
void CRcvView::OnInitialUpdate() 
{
CRect rc;
	CView::OnInitialUpdate();
	CWinPSKDoc* pDoc = (CWinPSKDoc*) GetDocument();
	ASSERT_VALID( pDoc);
	if (pDoc != NULL)
	{
		GetClientRect( rc); 
		pDoc->m_pRcvEdit = new CRcvEditCtrl;
		pDoc->m_pRcvEdit->Create(WS_VISIBLE | WS_CHILD | ES_AUTOVSCROLL|
					WS_CLIPCHILDREN | WS_VSCROLL | ES_MULTILINE |  ES_READONLY |
					ES_LEFT | ES_WANTRETURN, rc, this, IDC_RICHEDIT1);
		pDoc->m_pRcvEdit->SetCharFormat(pDoc->m_pSettings->m_lfRcvFont,
			pDoc->m_pSettings->m_lfXmitFont,
			pDoc->m_pSettings->m_crRcvTextColor,
			pDoc->m_pSettings->m_crXmitSentTextColor,
			pDoc->m_pSettings->m_crRcvBackColor	);
		pDoc->m_pRcvEdit->SetZeroCharDisplay(pDoc->m_pSettings->m_UseSlash);
//		pDoc->m_hRcvEditCtrl = pDoc->m_pRcvEdit->m_hWnd;
	}	
}

///////////////////////////////////////////////////////////////////////////////
//  remove CRichEditCtrl 
///////////////////////////////////////////////////////////////////////////////
void CRcvView::OnDestroy() 
{
	CWinPSKDoc* pDoc = (CWinPSKDoc*) GetDocument();
	ASSERT_VALID( pDoc);
	if(pDoc != NULL)
	{
		if(pDoc->m_pRcvEdit)
		{
			delete pDoc->m_pRcvEdit;
			pDoc->m_pRcvEdit = NULL;
		}
	}	
	CView::OnDestroy();
}
