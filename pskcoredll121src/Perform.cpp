///////////////////////////////////////////////////////
// Perform.cpp : implementation file
//
//  NOTE:: these functions depend on the 64 bit performance timer
// found in all Pentium processors. This timer may not be present
//  in some of the non-Intel versions.
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


#include "stdafx.h"
#include "Perform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
static LONGLONG StartTime;
static LONGLONG StopTime;
static LONGLONG DeltaTime;
static LONGLONG CountFreq;
static LONGLONG DeltaTimeMax;
static LONGLONG DeltaTimeMin;
static LONGLONG DeltaTimeAve;
static LONGLONG DeltaSamples;


// call to initialize the prformance timer
void InitPerformance()
{
	QueryPerformanceFrequency( (LARGE_INTEGER*)&CountFreq );	//get clock freq
	DeltaTimeMax = 0;
	DeltaTimeAve = 0;
	DeltaSamples = 0;
	DeltaTimeMin = 0x7FFFFFFFFFFFFFFF;
}

// Starts the performance timer
void StartPerformance()
{
	QueryPerformanceCounter( (LARGE_INTEGER*)&StartTime );
}

// Stop performance timer and calculate timing values
void StopPerformance()
{
	QueryPerformanceCounter( (LARGE_INTEGER*)&StopTime );
	DeltaTime = StopTime-StartTime;
	DeltaTimeAve += DeltaTime;
	DeltaSamples++;
	if( DeltaTime>DeltaTimeMax )
		DeltaTimeMax = DeltaTime;
	if( DeltaTime<DeltaTimeMin )
		DeltaTimeMin = DeltaTime;

}

// Call this to measure time between succesive calls to SamplePerformance()
void SamplePerformance()
{
	if(	DeltaSamples == 0 )
	{
		QueryPerformanceCounter( (LARGE_INTEGER*)&StartTime );
	}
	else
	{
		QueryPerformanceCounter( (LARGE_INTEGER*)&StopTime );
		DeltaTime = StopTime-StartTime;
		DeltaTimeAve += DeltaTime;
		if( DeltaTime>DeltaTimeMax )
			DeltaTimeMax = DeltaTime;
		if( DeltaTime<DeltaTimeMin )
			DeltaTimeMin = DeltaTime;
		QueryPerformanceCounter( (LARGE_INTEGER*)&StartTime );
	}
	DeltaSamples++;
}

// output various timing statistics to Message Box
// DeltaTimeMax == maximum time between start()-stop() or sample()-Sample()
// DeltaTimeMin == minimum time between start()-stop() or sample()-Sample()
// DeltaTimeAve == average time between start()-stop() or sample()-Sample()
// DeltaSamples == number of time samples captured
void ReadPerformance()
{
char buf[200];
	if(DeltaSamples != 0 )
	{
		DeltaTime = (DeltaTime*1000000)/CountFreq;
		DeltaTimeMin = (DeltaTimeMin*1000000)/CountFreq;
		DeltaTimeMax = (DeltaTimeMax*1000000)/CountFreq;
		DeltaTimeAve = DeltaTimeAve/DeltaSamples;
		DeltaTimeAve = (DeltaTimeAve*1000000)/CountFreq;
		sprintf( buf, " Max=%I64u uSec Min=%I64u uSec\nAve=%I64u uSec #Samps=%I64u",
					DeltaTimeMax,DeltaTimeMin,DeltaTimeAve,DeltaSamples);
		AfxMessageBox( buf );
	}
}