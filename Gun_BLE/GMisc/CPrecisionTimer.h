// Awesome! - http://www.codeguru.com/cpp/w-p/system/timers/article.php/c5759/Creating-a-HighPrecision-HighResolution-and-Highly-Reliable-Timer-Utilising-Minimal-CPU-Resources.htm


#pragma once

#include <windows.h>
#include <atlstr.h>


class CPrecisionTimer
{
  LARGE_INTEGER lFreq, lStart;

public:
  CPrecisionTimer()
  {
    QueryPerformanceFrequency(&lFreq);
  }

  inline void Start()
  {
    QueryPerformanceCounter(&lStart);
  }
  
  inline double Stop()
  {
    // Return duration in seconds...
    LARGE_INTEGER lEnd;
    QueryPerformanceCounter(&lEnd);
    return (double(lEnd.QuadPart - lStart.QuadPart) / lFreq.QuadPart);
  }
  inline double Lapse()
  {
    // Return duration in seconds...
    LARGE_INTEGER lEnd;
    QueryPerformanceCounter(&lEnd);
	double d = double(lEnd.QuadPart - lStart.QuadPart) / lFreq.QuadPart;
	lStart = lEnd;
    return d;
  }
  
  inline int LapseMillisecs()
  {
    LARGE_INTEGER EndingTime;
	QueryPerformanceCounter(&EndingTime);
	LARGE_INTEGER ElapsedMilliseconds;
	ElapsedMilliseconds.QuadPart = EndingTime.QuadPart - lStart.QuadPart;
	ElapsedMilliseconds.QuadPart *= 1000; // To guard against loss-of-precision, we convert to millisec
	ElapsedMilliseconds.QuadPart /= lFreq.QuadPart; // *before* dividing by ticks-per-second.
	return (int)ElapsedMilliseconds.QuadPart;
  }

  inline long long LapseMicrosecs()
  {
    LARGE_INTEGER EndingTime;
	QueryPerformanceCounter(&EndingTime);
	LARGE_INTEGER ElapsedMicroseconds;
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - lStart.QuadPart;

	//
	// We now have the elapsed number of ticks, along with the
	// number of ticks-per-second. We use these values
	// to convert to the number of elapsed microseconds.
	// To guard against loss-of-precision, we convert
	// to microseconds *before* dividing by ticks-per-second.
	//

	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= lFreq.QuadPart;
	return (long long)ElapsedMicroseconds.QuadPart;
  }
};


extern CPrecisionTimer PT;

