
#include "CPrecisionTimer.h"
#include "stdio.h"

CPrecisionTimer PT;

/* Another example..  It demonstrated a reliable 1ms timer: http://forums.codeguru.com/showthread.php?522839-timeSetEvent-worked-in-XP-not-in-Windows-7



void CALLBACK TimerProc(  UINT uTimerID,
						  UINT uMsg,
						  DWORD_PTR dwUser,
						  DWORD_PTR dw1,
						  DWORD_PTR dw2 )
{
	static int counter(0);
	double d = PT.Lapse();
	printf("%.3d: %lf\n", counter++, d);
}


	PT.Start();
	MMRESULT mmr = timeBeginPeriod(1);
	mmr = timeSetEvent(1, 1, &TimerProc, 42, TIME_PERIODIC);

	Sleep(100);

 */