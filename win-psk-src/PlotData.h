// PlotData.h: interface for the CPlotData class.
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//

#if !defined(AFX_PLOTDATA_H__91277FC1_4F28_11D3_A232_00A0C996E7F5__INCLUDED_)
#define AFX_PLOTDATA_H__91277FC1_4F28_11D3_A232_00A0C996E7F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinPSKDoc.h"

#define DISPBUF_SIZE (2048)	// 2048/2 FFT points or 2048 raw data points

class CPlotData  
{
public:
	void SetRewindPos( INT y);
	CPlotData();
	virtual ~CPlotData();
	void SetPlotCursorIndex( INT index ){m_XCurPos = index;};
	void SetPlotSize( CRect cr ){m_rPlotsize = cr;};
	void SetPlotCursorPos( INT pos );
	void SetZoom(INT zoom, BOOL Cent, INT fmin, INT fmax);
	void ClearBuffers(void);
	void OnDrawPlot( HWND hWnd, CFormView* pView,CWinPSKDoc* pDoc);
	BOOL CurInPlot(CPoint pt){return m_rPlot.PtInRect(pt);}
	INT GetZoom(void){return m_Zoom;}
	INT GetXmin(void){return m_XMinPos;}
	INT GetXmax(void){return m_XMaxPos;}
	INT GetPlotCursorIndex( void ){return m_XCurPos;}
//	public things for access by non class worker thread
	void DrawPlot();


private:
// functions
	void PlotFFTData( CDC* pDC );
	void PlotFFTWater( CDC* pDC);
	void PlotOverlayVectData( CDC* pDC );
	void PlotOverlayVectDataWaterfall( CDC* pDC );
	void PlotInputData( CDC* pDC );
	void PlotSyncData( CDC* pDC );
// variables
	BOOL m_NeedToClrBufs;
	BOOL m_BufToggle;
	BOOL m_LastOvrLoad;
	BOOL m_InputOverload;
	INT m_FFTAveVal;
	INT	m_CursPos;			// Curs position in screen units
	INT	m_CursIndx;			// Curs position in DispBuf[] index units
	INT	m_XMinPos;
	INT	m_XMaxPos;
	INT	m_XCurPos;
	INT m_Zoom;
	UINT	m_DispHead;
	UINT	m_DispTail;
	LONG m_SyncData[16];
	CPen* m_pPenG;			//different colored pens used for drawing plots
	CPen* m_pPenY;
	CPen* m_pPenW;
	CPen* m_pPenR;
	CPen* m_pPenC;
	CPen* m_pPenB;
	INT* m_TranslateTbl;
	INT* m_pDispBuf;
	RECT m_rPlotsize;		// size of the plot screen bitmap
	CRect m_rPlot;			// client plot screen rectangle coordinates
	CBitmap m_bmPlot;		// bitmap image of plot window
	CBitmap m_bmWaterA;		// bitmap image for waterfall ping pong buf A
	CBitmap m_bmWaterB;		// bitmap image for waterfall ping pong buf B
	CWinPSKDoc *m_pDoc;
	CDC* m_pDC;
	HWND m_hViewWnd;
};

#endif // !defined(AFX_PLOTDATA_H__91277FC1_4F28_11D3_A232_00A0C996E7F5__INCLUDED_)
