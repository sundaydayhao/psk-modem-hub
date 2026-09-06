// XmitView.cpp : implementation file
//
//////////////////////////////////////////////////////////////////////
//

#include "stdafx.h"
#include "winpsk.h"
#include "WinPSKDoc.h"
#include "XmitView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXmitView

IMPLEMENT_DYNCREATE(CXmitView, CView)

CXmitView::CXmitView()
{
}

CXmitView::~CXmitView()
{
}


BEGIN_MESSAGE_MAP(CXmitView, CView)
	//{{AFX_MSG_MAP(CXmitView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXmitView drawing

void CXmitView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CXmitView diagnostics

#ifdef _DEBUG
void CXmitView::AssertValid() const
{
	CView::AssertValid();
}

void CXmitView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CXmitView message handlers
/////////////////////////////////////////////////////////////////////////////
void CXmitView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	CWinPSKDoc* pDoc = (CWinPSKDoc*) GetDocument();
	if (pDoc != NULL)
	{
		if (pDoc->m_pXmitEdit != NULL)
			pDoc->m_pXmitEdit->MoveWindow(0,0,cx,cy);
	}	
	
}

/////////////////////////////////////////////////////////////////////////////
//	Attach a TX CRichEditCtrl to this view
/////////////////////////////////////////////////////////////////////////////
void CXmitView::OnInitialUpdate() 
{
CRect rc;
	CView::OnInitialUpdate();
	CWinPSKDoc* pDoc = (CWinPSKDoc*) GetDocument();
	if (pDoc != NULL)
	{
		GetClientRect( rc); 
		pDoc->m_pXmitEdit = new CXmitEditCtrl;
		pDoc->m_pXmitEdit->Create(WS_VISIBLE | WS_CHILD | ES_AUTOVSCROLL|
					WS_CLIPCHILDREN | WS_VSCROLL | ES_MULTILINE |  /*ES_READONLY |*/
					ES_LEFT | ES_WANTRETURN, rc, this, IDC_RICHEDIT2);

		pDoc->m_pXmitEdit->SetCharFormat(pDoc->m_pSettings->m_lfXmitFont,
			pDoc->m_pSettings->m_lfXmitSentFont,
			pDoc->m_pSettings->m_crXmitTextColor,
			pDoc->m_pSettings->m_crXmitSentTextColor,
			pDoc->m_pSettings->m_crXmitBackColor );
		pDoc->m_hXmitEditCtrl = pDoc->m_pXmitEdit->m_hWnd;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	Get rid of the TX CRichEditCtrl attached to this view and the view itself
/////////////////////////////////////////////////////////////////////////////
void CXmitView::OnDestroy() 
{
	CView::OnDestroy();
	CWinPSKDoc* pDoc = (CWinPSKDoc*) GetDocument();
	if (pDoc != NULL)
	{
		if(pDoc->m_pXmitEdit)
		{
			delete pDoc->m_pXmitEdit;
			pDoc->m_pXmitEdit = NULL;
		}

	}	
	
}
