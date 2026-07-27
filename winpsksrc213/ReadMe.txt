==========================================================================
++++++++++++++++++   R E A D M E . T X T   +++++++++++++++++++++++++++++++
++++++++++++++    for WinPSK program source code    ++++++++++++++++++++++
==========================================================================
Revision history:
ver. 0.1	initial beta release 12-16-99
ver. 0.2	12-17-99 Fixed CWID problem
ver. 0.3	12-19-99 Fixed Font save problem, changed settings.dat method
				made macro titles longer, limit text to 256
ver. 0.4	12-20-99 Fixed accent text, modified macros, added 'start'keyword

ver. 1.0	12-30-99 Initial non-beta release
		Bug fixes:
			Fixed Missing characters at end of TX
			Made AFC smoother
			Fixed screen maximize restoration problem
			Fixed UTC date problem
			Fixed problem if Tune button was double clicked
			Changed CW ID spacing
			Fixed display zoom problems
			Fixed(by removing) the memory resource display problem.
			Fixed problem of version number in registry key
		Functional Changes:
			Changed FFT size to 2048 making updates faster(.37 Sec)
			Zoom limit reduced to 10
			Removed useless signal views(3D,I/Q,eye,large vector)
			Rewrote waterfall display for 10x speedup, made color, and bigger
			Changed test mode to demo mode and added fast option
			Added option to turn off tx text echoing in rx window
			Removed QPSK lsb mode box and moved lsb select into setup
			Increased macro text size to 2K
			Placed Macro title text on button and made button larger
			Changed UTC time/date format
			Added means to measure and adjust soundcard clock frequency
			Added an input overload message to spectral displays
			Added CPU too slow message in signal displays
ver. 1.1	1-23-00 maintenance release
		Bug fixes:
			Fixed FFT amplitude scaling problem
			Fixed Fast DemoMode CPU %use bug
			Fixed "About Box" text problem
		Functional Changes:
			Added Narrow AFC mode after Transmit to reduce freq drift
			Added Setup links to Window's Mixer audio level control
			Changed CW ID speed/spacing
			Added Local Time option

ver. 2.0	5-29-00 New DLL based version release
		Bug fixes:
			Fixed ongoing problems with AFC
		Functional Changes:
			Added new QSO information edit boxes
			Added more macros and macro keywords
			Added new signal tuning functionality.
			Expanded frequency range
			Cleaned up user interface
			Added more user setup options
ver. 2.01	5-30-00 Maintenance release
		Bug fixes:
			Fixed asymetric AFC capture range in dll

ver. 2.02	6-05-00 Maintenance release
		Bug fixes:
			Fixed Focus problem with Tx/Rx button

ver. 2.03	6-24-00 Maintenance release
		Functional Changes:
			Limited frequency range to 200-3500Hz.
			Added Small Spectral view to waterfall view.
		Bug fixes:
			Fixed Copy/Paste to TX window problem.

ver. 2.04	6-25-00 Maintenance release
		Bug fixes:
			Fixed (again?)Copy/Paste to TX window crash problem.
			Fixed hiding mouse cursor problem

ver. 2.05	7-02-00 Maintenance release
		Functional Changes:
			Added "Always on Top" selection to View menu.
		Bug fixes:
			Fixed Crash if soundcard was being used by another program.

ver. 2.06	7-15-00 Maintenance release
		Functional Changes:
			Added multiple text files in macros and keywords in the text files.
		Bug fixes:
			Fixed Input and sync view problem with general setup.

ver. 2.07	7-23-00 Maintenance release
		Bug fixes:
			Fixed Boot up crashing problem. DLL sent a DATARDY msg to update
			the screen before the void CWinPSKView::OnDraw(CDC* pDC) was called
			for the first time to init the screen.

ver. 2.08	8-03-00 Maintenance release
		Functional Changes:
			Added "rewind" feature that rightclicking in the spectrum or
                        waterfall backs up the audio data and re-receives it.  Added delay between
                        PTT and audio.
		Bug fixes:
			Wasn't restoring several settings on entry. Fixed tab order bug with
			BPSK checkbox.
ver. 2.09	12-24-00 Maintenance release
		Functional Changes:
			Added full duplex satellite mode with fast tracking AFC.                     PTT and audio.
ver. 2.10	8-4-01 Maintenance release
		Functional Changes:
			Added wave file reading/writing support.
			Added TX Offset function
			Added Waterfall palette file support
ver. 2.11	9-8-01 Maintenance release
		Bug fixes:
			Fixed some com port issues, support for PSKCoreDLL ver 1.13
ver. 2.13	5-22-03 Maintenance release
		Bug fixes:
			Added double speed PSK63 support for PSKCoreDLL ver 1.16
Moe Wheatley AE4JY <ae4jy@qsl.net>
==========================================================================

System Requirements:

133MHz Pentium ( works on 486 but MUST have floating point processor)
Windows 95/98/NT ( is a 32 bit Windows application )
A soundcard that is supported by Windows.

-----------------------------------------------------------------------
For those interested in the source code for this application, it was written
Using MicroSoft Visual C++ 6.0.  MFC was used for the user interface stuff
and a single worker thread is used to generate and process all the data
streams.

Note: when executing in DEBUG mode, because the optimizer is off, execution
speed will slow significantly.
========================================================================
 VC++ 6.0      Project Source File Descriptions for : WinPSK
========================================================================
ErrorCodes.h    // some table include files
PskDetTbl.h
PSKTables.h
PlotData.h

fft.cpp		//the FFT class
fft.h
           
IOCntrl.cpp	//DSP thread class
IOCntrl.h

MacroDlg.cpp	// macro setup dlg
MacroDlg.h

MainFrm.cpp	//main frame of program
MainFrm.h

PlotData.cpp	// Signal data plotting class
PlotData.h

RcvEditCtrl.cpp	//RX text window ctrl
RcvEditCtrl.h

RcvView.cpp	//RX view window
RcvView.h

SetDlg.cpp	//settings dlg
SetDlg.h

Settings.cpp	//program settings stored here
Settings.h

Sound.cpp	// soundcard interface class
Sound.h

SquelchCtrl.cpp	// the squelch control
SquelchCtrl.h

WinPSK.cpp	// main app
WinPSK.h

WinPSKDoc.cpp	// document class for inter-object communications
WinPSKDoc.h

WinPSKView.cpp	// Form view class for most of the controls
WinPSKView.h

XmitEditCtrl.cpp // TX edit control
XmitEditCtrl.h

XmitView.cpp	//TX view window
XmitView.h


/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named WSCGen.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Developer Studio reads and updates this file.

/////////////////////////////////////////////////////////////////////////////
