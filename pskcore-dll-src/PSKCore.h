//////////////////////////////////////////////////////////////////////
// PSKCore.h : main header file for the PSKCORE DLL
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// PSK31Core Library for transmission and reception of PSK31 signals
//    using a PC soundcard  or .wav files.
//	               Copyright 2000, Moe Wheatley, AE4JY
//
//This library is free software; you can redistribute it and/or
//modify it under the terms of the GNU Lesser General Public
//License as published by the Free Software Foundation; either
//version 2.1 of the License, or (at your option) any later version.
//
//This library is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//Lesser General Public License for more details.
//
//You should have received a copy of the GNU Lesser General Public
//License along with this library; if not, write to the Free Software
//Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// http://www.gnu.org/copyleft/lesser.html
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PSKCORE_H__F8820945_EC65_11D3_B4E7_903B50C10000__INCLUDED_)
#define AFX_PSKCORE_H__F8820945_EC65_11D3_B4E7_903B50C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#define COREDLL_EXPORTS 1


#include "resource.h"		// main symbols
#include "pskcoreimplicit.h"

extern "C" long COREDLL_API __stdcall fnGetDebugData(double* val1, double* val2);

/////////////////////////////////////////////////////////////////////////////
#define SAMP_RATE 8000	//soundcard samplerate
#define MAX_CHANNELS 50
#define TX_BUF_SIZE 32000

/////////////////////////////////////////////////////////////////////////////
// CPSKCoreApp
// See PSKCore.cpp for the implementation of this class
//

#ifdef DLL_VERSION	//(dec) 11-Mar-2001
class CPSKCoreApp : public CWinApp
{
public:
	CPSKCoreApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPSKCoreApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CPSKCoreApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PSKCORE_H__F8820945_EC65_11D3_B4E7_903B50C10000__INCLUDED_)
