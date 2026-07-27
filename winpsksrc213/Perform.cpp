///////////////////////////////////////////////////////
// Perform.cpp : implementation file
//
//  NOTE:: these functions depend on the 64 bit performance timer
// found in all Pentium processors. This timer may not be present
//  in some of the non-Intel versions.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <math.h>
#include "Perform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
static LONGLONG CountFreq;
static LONGLONG DSPStartTime;
static LONGLONG DSPStopTime;
static LONGLONG DSPDeltaTimeAve;
static LONGLONG DSPDeltaSum;

static LONGLONG DISPStartTime;
static LONGLONG DISPStopTime;
static LONGLONG DISPDeltaTimeAve;
static LONGLONG DISPDeltaSum;

// call to initialize the performance timer
void InitPerformance()
{
	QueryPerformanceFrequency( (LARGE_INTEGER*)&CountFreq );	//get clock freq
	DSPDeltaTimeAve = 0;
	DSPDeltaSum = 0;
	DISPDeltaTimeAve = 0;
	DISPDeltaSum = 0;
	QueryPerformanceCounter( (LARGE_INTEGER*)&DSPStartTime );
	QueryPerformanceCounter( (LARGE_INTEGER*)&DISPStartTime );
}

// Starts the DSP performance timer
void StartDSPPerformance()
{
	QueryPerformanceCounter( (LARGE_INTEGER*)&DSPStartTime );
}

// Stop  DSP performance timer and calculate timing average
void StopDSPPerformance()
{
	QueryPerformanceCounter( (LARGE_INTEGER*)&DSPStopTime );
	DSPDeltaSum = DSPDeltaSum + DSPStopTime - DSPStartTime - DSPDeltaTimeAve;
	DSPDeltaTimeAve = DSPDeltaSum>>2;
}

// Starts the DISPLAY performance timer
void StartDISPPerformance()
{
	QueryPerformanceCounter( (LARGE_INTEGER*)&DISPStartTime );
}

// Stop  DISPLAY performance timer and calculate timing average
void StopDISPPerformance()
{
	QueryPerformanceCounter( (LARGE_INTEGER*)&DISPStopTime );
	DISPDeltaSum = DISPDeltaSum + DISPStopTime - DISPStartTime - DISPDeltaTimeAve;
	DISPDeltaTimeAve = DISPDeltaSum >> 2;
}

// calculate and return CPU percentage of DSP and Display threads
// Doesn't take into consideration main process thread or system times
//  so is more of a relative measure of performance of the two worker threads.
INT ReadPerformance(INT Period)
{
LONGLONG temp;
	temp = (DISPDeltaTimeAve*1000000)/CountFreq;		// display time
	temp = temp + (DSPDeltaTimeAve*1000000)/CountFreq;				// dsp time
//temp now is total average time spent processing a buffers worth of sound data
//   both the DSP functions of RX and TX as well as the screen plotting times
	temp = (100L*temp)/(LONGLONG)Period;	//calc percentage
	return (INT)temp;
}

