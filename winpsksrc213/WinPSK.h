// WinPSK.h : main header file for the WINPSK application
//
//////////////////////////////////////////////////////////////////////
//
#if !defined(AFX_WINPSK_H__EFF3B524_3631_11D3_B48C_E03851C10000__INCLUDED_)
#define AFX_WINPSK_H__EFF3B524_3631_11D3_B48C_E03851C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include <math.h>
#include "PskCoreImplicit.h"
#define BACK_SPACE_CODE 0x08	// define some control codes for

#define IOM_NORMAL 0		//Use soundcard only
#define IOM_WAVEIN (1<<0)	//1==use input wave file, 0==use soundcard
#define IOM_WAVELOG (1<<1)	//1==copy soundcard in to outwave file, 0==don't
#define IOM_WAVETX (1<<2)	//1==Save Tx to outwave file, 0==don't
#define IOM_NOTXSOUND (1<<3)//1==No Tx to soundcard, 0==Tx Output to soundcard
#define IOM_ECHOINPUT (1<<4)//1==Echo input to SoundcarOut 0== No Echo


/////////////////////////////////////////////////////////////////////////////
// CWinPSKApp:
// See WinPSK.cpp for the implementation of this class
//

class CWinPSKApp : public CWinApp
{
public:
	CWinPSKApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinPSKApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CWinPSKApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINPSK_H__EFF3B524_3631_11D3_B48C_E03851C10000__INCLUDED_)
