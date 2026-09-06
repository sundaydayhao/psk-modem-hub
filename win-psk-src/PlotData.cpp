// PlotData.cpp: implementation of the CPlotData class.
//
//

#include "stdafx.h"
#include "winpsk.h"
#include "Settings.h"
#include "WinPSKView.h"
#include "PlotData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPlotData::CPlotData()
{
	m_pDoc = NULL;
	m_DispHead = 0;
	m_DispTail = 0;
	m_BufToggle = FALSE;
	m_NeedToClrBufs = FALSE;
	m_LastOvrLoad = FALSE;
//allocate memory space
	m_pDispBuf = new INT[DISPBUF_SIZE+10];
	ASSERT( m_pDispBuf );
	ClearBuffers();
	m_TranslateTbl = new INT[DISPBUF_SIZE+10];
	ASSERT( m_TranslateTbl );
//allocate FFT object
	m_CursPos = 0;
	m_pPenR = new CPen;		// create some colored pens for drawing
	m_pPenG = new CPen;
	m_pPenY = new CPen;
	m_pPenW = new CPen;
	m_pPenC = new CPen;
	m_pPenB = new CPen;
	m_pPenR->CreatePen( PS_SOLID,0, RGB(255,0,0) );
	m_pPenG->CreatePen( PS_SOLID,0, RGB(0,255,0) );
	m_pPenY->CreatePen( PS_DOT,0, RGB(255,255,0) );
	m_pPenW->CreatePen( PS_SOLID,0, RGB(128,128,128) );
	m_pPenC->CreatePen( PS_SOLID,0, RGB(0,255,255) );
	m_pPenB->CreatePen( PS_SOLID,0, RGB(0,0,255) );
}

CPlotData::~CPlotData()
{
	m_pPenR->DeleteObject(); delete m_pPenR;
	m_pPenG->DeleteObject(); delete m_pPenG;
	m_pPenY->DeleteObject(); delete m_pPenY;
	m_pPenW->DeleteObject(); delete m_pPenW;
	m_pPenC->DeleteObject(); delete m_pPenC;
	m_pPenB->DeleteObject(); delete m_pPenB;
	if(m_pDispBuf)
	{
		delete m_pDispBuf;
		m_pDispBuf = NULL;
	}
	if(m_TranslateTbl)
	{
		delete m_TranslateTbl;
		m_TranslateTbl = NULL;
	}	
// Delete two ping pong bitmap buffers for waterfall display
	if( m_bmWaterA.m_hObject != NULL )	// delete old bitmap object
		m_bmWaterA.DeleteObject();
	if( m_bmWaterB.m_hObject != NULL )	// delete old bitmap object
		m_bmWaterB.DeleteObject();
}


//////////////////////////////////////////////////////////////////////
//  Called from the Windows view class when the window has changed
//      and needs to update the screen coordinates.
//////////////////////////////////////////////////////////////////////
void CPlotData::OnDrawPlot(HWND hWnd, CFormView* pView, CWinPSKDoc* pDoc)
{
CRect rScrn;
	m_pDoc = pDoc;
	m_hViewWnd = hWnd;
			//calculate new plot area coordinates
// Get pixel size of plot area bitmap
	ASSERT_VALID( pView );
	CWnd* pWnd = pView->GetDlgItem( IDC_PLOTAREA );
	ASSERT_VALID( pWnd);
	pWnd->GetWindowRect( m_rPlot );
	pView->GetWindowRect(rScrn);
	m_rPlot.top -= (rScrn.top+1);
	m_rPlot.left -= (rScrn.left+1);
	m_rPlot.bottom = m_rPlot.top+m_rPlotsize.bottom;
	m_rPlot.right = m_rPlot.left+m_rPlotsize.right;
// Create two ping pong bitmap buffers for waterfall display
	if( m_bmWaterA.m_hObject != NULL )	// delete old bitmap object
		m_bmWaterA.DeleteObject();
	if( m_bmWaterB.m_hObject != NULL )	// delete old bitmap object
		m_bmWaterB.DeleteObject();
//create a waterfall ping pong bitmap with current screen DC settings
CClientDC ClientDC(CWnd::FromHandle(m_hViewWnd) );
	m_bmWaterA.CreateCompatibleBitmap( &ClientDC, m_rPlot.Width(), m_rPlotsize.bottom );
	m_bmWaterB.CreateCompatibleBitmap( &ClientDC, m_rPlot.Width(), m_rPlotsize.bottom );
	m_NeedToClrBufs = TRUE;		// so waterfal buffers can get cleared
//
	m_pDoc->m_FFTModeChange = TRUE;
}


//////////////////////////////////////////////////////////////////////
//  Called to clear plot surface, draw cursor, and call specific plot
//         routine.
//////////////////////////////////////////////////////////////////////
void CPlotData::DrawPlot()
{
// erases plot surface
// plots data points
INT i;
CDC MemDC;
CPen* pOldPen;
CClientDC ClientDC(CWnd::FromHandle(m_hViewWnd) );
	m_InputOverload = FALSE;
	if( m_bmPlot.m_hObject != NULL )	// delete old bitmap object
		m_bmPlot.DeleteObject();
//create a new bitmap with current screen DC settings
	m_bmPlot.CreateCompatibleBitmap( &ClientDC, m_rPlot.Width(),
					 m_rPlot.Height() );
//create memory DC pointer to the plot bitmap
	MemDC.CreateCompatibleDC( &ClientDC );
CBitmap* pbmOld = MemDC.SelectObject( &m_bmPlot );

// set bitmap background to black using slightly faster ExtTextOut function
	MemDC.SetBkColor( RGB(0, 0, 0) );
	MemDC.ExtTextOut( 0, 0,ETO_OPAQUE, &m_rPlotsize,NULL,0,NULL);

	pOldPen = MemDC.SelectObject(m_pPenY);		//save original pen.
	if(	m_pDoc->m_FFTModeChange)
	{
		m_pDoc->m_FFTModeChange = FALSE;
		if( m_pDoc->m_pSettings->m_fSlowAve)
			m_FFTAveVal = 6;
		else
			m_FFTAveVal = 2;
		if( m_pDoc->m_pSettings->m_TabSel == FFT_DISPVIEW)
			fnSetFFTMode( m_FFTAveVal, m_rPlotsize.bottom, 1 );
		else
			fnSetFFTMode( m_FFTAveVal, 255, 20 );
	}
// draw selected data plot
	switch( m_pDoc->m_pSettings->m_TabSel )
	{
		case FFT_DISPVIEW:
			// Draw vertical yellow dotted plot cursor line
			MemDC.MoveTo( m_CursPos, 0);
			MemDC.LineTo( m_CursPos, m_rPlotsize.bottom );
			MemDC.SelectObject(m_pPenW);
			for( i=10; i<100; i+=10 )	//draw 10dB tic marks
			{
				MemDC.MoveTo( 0, (m_rPlotsize.bottom*i)/100 );
				MemDC.LineTo( m_rPlotsize.right, (m_rPlotsize.bottom*i)/100 );
			}
			if( ((CWinPSKView*)m_pDoc->m_pWinPSKView)->m_ProgramState == STATE_RX)
				MemDC.SelectObject(m_pPenG);
			else
				MemDC.SelectObject(m_pPenC);
			PlotFFTData( &MemDC );
			MemDC.SelectObject(m_pPenW);
			PlotOverlayVectData( &MemDC );
			break;
		case WATERFALL_DISPVIEW:
			// Draw vertical yellow dotted plot cursor line
			MemDC.MoveTo( m_CursPos, 0);
			MemDC.LineTo( m_CursPos, m_rPlotsize.bottom );
			if( ((CWinPSKView*)m_pDoc->m_pWinPSKView)->m_ProgramState == STATE_RX)
				MemDC.SelectObject(m_pPenG);
			else
				MemDC.SelectObject(m_pPenC);
			PlotFFTWater( &MemDC);
			MemDC.SelectObject(m_pPenW);
			PlotOverlayVectDataWaterfall( &MemDC );
			break;
		case INPUT_DISPVIEW:
			MemDC.SelectObject(m_pPenW);
			for( i=10; i<100; i+=10 )	//draw 10dB tic marks
			{
				MemDC.MoveTo( 0, (m_rPlotsize.bottom*i)/100 );
				MemDC.LineTo( m_rPlotsize.right, (m_rPlotsize.bottom*i)/100 );
			}
			MemDC.SelectObject(m_pPenG);
			PlotInputData( &MemDC );
			break;
		case SYNC_DISPVIEW:
			PlotSyncData( &MemDC);
			break;
		default:
			break;
	}
	if(m_InputOverload && m_LastOvrLoad) //need two in a row
	{
		MemDC.SetTextColor( RGB(255,0,0) );
		MemDC.TextOut( m_rPlotsize.right/4, m_rPlotsize.bottom/8,
			_T("Reduce Audio Level"), 18 );
	}
	m_LastOvrLoad = m_InputOverload;
	if( m_pDoc->m_SoundCardReset)
	{
		MemDC.SetTextColor( RGB(255,0,0) );
		MemDC.TextOut( m_rPlotsize.right/4, m_rPlotsize.bottom/8,
			_T("CPU Too Slow"), 12 );
		m_pDoc->m_SoundCardReset = FALSE;
	}
// Copy memory bitmap into screen device context for flicker free update
	ClientDC.BitBlt( m_rPlot.left, m_rPlot.top,
					m_rPlotsize.right,
					m_rPlotsize.bottom, &MemDC,
					0,0,SRCCOPY);
	MemDC.SelectObject(pOldPen);	//deselect pens and bitmap
	MemDC.SelectObject( pbmOld );	// from memory DC
}



//////////////////////////////////////////////////////////////////////
//		Perform FFT and plot two dimensional results
//////////////////////////////////////////////////////////////////////
void CPlotData::PlotFFTData( CDC* pDC)
{
INT x,y,k1;
	k1 = m_rPlotsize.bottom;		//y range
	m_InputOverload = fnGetFFTData( (long*)m_pDispBuf,
							m_XMinPos,
							m_XMaxPos );
	pDC->MoveTo( 0, k1-m_pDispBuf[m_XMinPos]);//move to first position
	for( x=m_XMinPos; x<m_XMaxPos; x++ )	//~11mSecs on 266PII
	{
		y = k1-m_pDispBuf[x];
		pDC->LineTo( m_TranslateTbl[x], y );		//draw to the next pt.
	}
}


//////////////////////////////////////////////////////////////////////
//		Perform FFT and plot waterfall
//////////////////////////////////////////////////////////////////////
void CPlotData::PlotFFTWater( CDC* pDC)
{
INT i,x,y,k1,k2,k3,k4;
COLORREF cr;
CDC MemDCA;		// memory DCs for the above bitmaps
CDC MemDCB;
	k4 = m_rPlotsize.bottom;
//create memory DC pointers for the waterfall bitmap buffers
	MemDCA.CreateCompatibleDC( pDC );
	MemDCB.CreateCompatibleDC( pDC );
CBitmap* pbmOldA = MemDCA.SelectObject( &m_bmWaterA );
CBitmap* pbmOldB = MemDCB.SelectObject( &m_bmWaterB );
	if(m_NeedToClrBufs)
	{
		MemDCA.BitBlt( 0, 0, m_rPlotsize.right,k4, NULL, 0,0,BLACKNESS);
		MemDCB.BitBlt( 0, 0, m_rPlotsize.right,k4, NULL, 0,0,BLACKNESS);
		m_NeedToClrBufs = FALSE;
	}
	k3 = k4/3; //y starting point

	pDC->MoveTo( 0, k3-(k3*m_pDispBuf[m_XMinPos])/255);//move to first position
	for( x=m_XMinPos; x<m_XMaxPos; x++ )
	{
		y = k3-(k3*m_pDispBuf[x])/255;
		pDC->LineTo( m_TranslateTbl[x], y );		//draw to the next pt.
	}
	m_InputOverload = fnGetFFTData( (long*)m_pDispBuf,	m_XMinPos, m_XMaxPos );
	k2 = (m_XMaxPos-m_XMinPos+1);

	if(m_BufToggle) //use A buffer
	{
		// Copy lines from buf B to A shifted down 1 lines
		MemDCA.BitBlt( 0, 1, m_rPlotsize.right,k4-k3-1, &MemDCB, 0,0,SRCCOPY);
		// Draw top two new lines int to buf A
		for( x=0; x<m_rPlotsize.right; x++)
		{
			k1 = (x*k2)/m_rPlotsize.right + m_XMinPos;
			i = m_pDispBuf[k1];
			cr = m_pDoc->m_ColorTbl[i];		//RGB( i, i, i);
			MemDCA.SetPixel( x , 0, cr );
		}
		// copy buf A into main plot bitmap
		pDC->BitBlt( 0, k3,	m_rPlotsize.right,k4-k3, &MemDCA, 0,0,SRCCOPY);
	}
	else			// use B buffer
	{
		// Copy lines from buf A to B shifted down 1 lines
		MemDCB.BitBlt( 0, 1, m_rPlotsize.right,k4-k3-1, &MemDCA, 0,0,SRCCOPY);
		// Draw top two new lines int to buf B
		for( x=1; x<m_rPlotsize.right; x++)
		{
			k1 = (x*k2)/m_rPlotsize.right + m_XMinPos;
			i = m_pDispBuf[k1];
			cr = m_pDoc->m_ColorTbl[i];		//RGB( i, i, i);
			MemDCB.SetPixel( x , 0, cr );
		}
		// copy buf B into main plot bitmap
		pDC->BitBlt( 0, k3,	m_rPlotsize.right,k4-k3, &MemDCB, 0,0,SRCCOPY);
	}
	MemDCA.SelectObject( pbmOldA );
	MemDCB.SelectObject( pbmOldB );
	m_BufToggle = !m_BufToggle;
}

//////////////////////////////////////////////////////////////////////
// Plot Baseband Data as vector plot on top of spectrum graph
//////////////////////////////////////////////////////////////////////
void CPlotData::PlotOverlayVectData( CDC* pDC )
{
INT x,xc,yc;
long VectArray[16];
CBrush* pOldBrush;
CBrush Brush;
	Brush.CreateSolidBrush( RGB(0,0,0) );
	yc = m_rPlotsize.bottom/8;	// y center
	xc = yc;
	pOldBrush = pDC->SelectObject(&Brush);
	pDC->Ellipse( 0, 0, xc+xc, m_rPlotsize.bottom/4);	//draw circle
	pDC->SelectObject( pOldBrush );
	fnGetVectorData(VectArray,0);
	pDC->SelectObject(m_pPenC);
	for(x=0; x<16; x+=2)
	{
		pDC->MoveTo( xc,  yc );
		pDC->LineTo( xc+(yc*VectArray[x]/1000), 
							yc-((yc)*VectArray[x+1]/1000) );
	}
}
//////////////////////////////////////////////////////////////////////
// Plot Baseband Data as vector plot on top of waterfall graph
//////////////////////////////////////////////////////////////////////
void CPlotData::PlotOverlayVectDataWaterfall( CDC* pDC )
{
INT x,xc,yc;
long VectArray[16];
CBrush* pOldBrush;
CBrush Brush;
	Brush.CreateSolidBrush( RGB(0,0,0) );
	yc = m_rPlotsize.bottom - m_rPlotsize.bottom/8;	// y center
	xc = m_rPlotsize.bottom/8;
	pOldBrush = pDC->SelectObject(&Brush);
	pDC->Ellipse( 0, (3*m_rPlotsize.bottom)/4, 2*xc, m_rPlotsize.bottom);//draw circle
	pDC->SelectObject( pOldBrush );
	fnGetVectorData(VectArray,0);
	pDC->SelectObject(m_pPenC);
	for(x=0; x<16; x+=2)
	{
		pDC->MoveTo( xc,  yc );
		pDC->LineTo( xc+(xc*VectArray[x]/1000), 
							yc-(xc*VectArray[x+1]/1000) );
	}
}
//////////////////////////////////////////////////////////////////////
// Plot Sync position distribution
//////////////////////////////////////////////////////////////////////
void CPlotData::PlotSyncData( CDC* pDC )
{
INT x,y,i,k1,k2;
INT ys;
	fnGetSyncData( m_SyncData,0);
	y = m_rPlotsize.bottom;
	k1 = y;
	k2 = m_rPlotsize.right/16;
	pDC->SelectObject(m_pPenG);
	for( x=k2/2,i=0; i<16; i++,x+=k2)
	{
		ys = m_SyncData[i]*k1/1000;
		pDC->MoveTo( x, y );
		pDC->LineTo( x, y-ys );
	}
}

//////////////////////////////////////////////////////////////////////
// Plots the input data directly in the time domain
//////////////////////////////////////////////////////////////////////
void CPlotData::PlotInputData( CDC* pDC)
{
INT x,y,k1;
BOOL overload = FALSE;
	k1 = m_rPlotsize.bottom/2;		//center
	pDC->MoveTo( 0, k1);			//move to first position
	overload = fnGetRawData( (long*)m_pDispBuf, 0, m_rPlotsize.right );
	if(overload)
		pDC->SelectObject(m_pPenR);
	for( x=0; x<m_rPlotsize.right; x++ )
	{
		y = k1 + (k1*m_pDispBuf[x])/32767;
		pDC->LineTo( x, y );	//draw to the next pt.
	}
}


//////////////////////////////////////////////////////////////////////
// Sets the plot zoom factor and calculates all the new parameters.
//////////////////////////////////////////////////////////////////////
void CPlotData::SetZoom( INT zoom, BOOL Cent, INT fmin, INT fmax)
{
INT xdelta;
CString Msg;
INT maxbuf;
INT minbuf;
	minbuf = (2048*fmin)/8000;
	maxbuf = (2048*fmax)/8000;
	m_Zoom = zoom;
	xdelta = (maxbuf-minbuf-1)/zoom;
	if( !Cent )
	{
		if( (m_XCurPos <= m_XMinPos) || (m_XCurPos >= m_XMaxPos) )
			Cent = TRUE;
	}
	if( Cent )
	{
		// Try to center the cursor
		m_XMinPos = m_XCurPos - xdelta/2;
		m_XMaxPos = m_XCurPos + xdelta/2;
		if( m_XMinPos<minbuf )
		{
			m_XMinPos = minbuf;
			m_XMaxPos = xdelta + minbuf;
		}
		else
		{
			if( m_XMaxPos > maxbuf )
			{
				m_XMinPos = maxbuf-xdelta;
				if( m_XMinPos<minbuf )
					m_XMinPos = minbuf;
				m_XMaxPos = maxbuf;
			}
		}
	}
	if(m_XMaxPos == m_XMinPos)
		return;
	m_CursPos = 100*m_rPlotsize.right*( m_XCurPos - m_XMinPos)
					/(m_XMaxPos - m_XMinPos); 
	if( (m_CursPos%100) > 50 )		//round to nearest integer
		m_CursPos = m_CursPos/100 + 1;
	else
		m_CursPos = m_CursPos/100;

// The translate table is used to map DispBuf[] index's into
//		Screen plot coordinates.
	for(INT i=m_XMinPos; i<m_XMaxPos; i++)
	{
		m_TranslateTbl[i] = ( (i-m_XMinPos)*m_rPlotsize.right )
									/ (m_XMaxPos - m_XMinPos);
	}
}

//////////////////////////////////////////////////////////////////////
// Called when a user clicks on the plot window to place the plot cursor.
// Sets the plot cursor to the specified screen position.  The DispBuf[]
//   index is then calculated.  The cursor on the screen is then updated.
//////////////////////////////////////////////////////////////////////
void CPlotData::SetPlotCursorPos( INT pos )
{
	m_CursPos = pos - m_rPlot.left;
	m_XCurPos = (m_CursPos*(m_XMaxPos - m_XMinPos)
					/m_rPlotsize.right) + m_XMinPos;
}

void CPlotData::SetRewindPos(INT y)
{
INT rewblocks = 0;
INT waterstart;
	if( m_pDoc->m_pSettings->m_TabSel == FFT_DISPVIEW)
	{
		rewblocks = 99;
	}
	else
	{
		if( m_pDoc->m_pSettings->m_TabSel == WATERFALL_DISPVIEW)
		{
			waterstart = m_rPlot.Height()/3 + m_rPlot.top;
			if(	y > waterstart )
				rewblocks = y - waterstart;
			else
				rewblocks = 0;
		}
	}
	if(rewblocks!=0)
		fnRewindInput(rewblocks);
}

//////////////////////////////////////////////////////////////////////
// Called to clear the display buffers
//////////////////////////////////////////////////////////////////////
void CPlotData::ClearBuffers()
{
	for(INT i=0; i<DISPBUF_SIZE; i++)
		m_pDispBuf[i] = 0;
	m_NeedToClrBufs = TRUE;		// so waterfal buffers can get cleared
}


