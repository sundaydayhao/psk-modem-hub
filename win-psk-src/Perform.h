///////////////////////////////////////////////////////
// Perform.h : Global interface for the performance functions
//////////////////////////////////////////////////////////////////////
//

#if !defined(_INCLUDE_PERFORMXXX_H_)
#define _INCLUDE_PERFORMXXX_H_

/////////////////////////////////////////////////////////////////////////////
extern void InitPerformance();
extern void StartDSPPerformance();
extern void StopDSPPerformance();
extern void StartDISPPerformance();
extern void StopDISPPerformance();
extern INT ReadPerformance(INT Period);
#endif //#if !defined(_INCLUDE_PERFORMXXX_H_)