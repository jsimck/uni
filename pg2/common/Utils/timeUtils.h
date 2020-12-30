#ifndef __TIMEUTILS_H_
#define __TIMEUTILS_H_

#ifndef WIN32
#define __int64 long long
  #include <cstdlib>
  #include <sys/time.h>
  #include <unistd.h>
#define secondsToMicro 1000000

#else // windows
  #include <windows.h>
#endif

class Timer
{
public:
	static LARGE_INTEGER currentTime, frequency;

public:
	inline static void intialize()
	{
#ifdef WIN32
		QueryPerformanceFrequency(&frequency);
#endif
	}

    inline static __int64 getCurrentTime()
	{
#ifdef WIN32
		QueryPerformanceCounter(&currentTime);
		return currentTime.QuadPart;
#else
		timeval internalTime;
		gettimeofday(&internalTime, NULL);
		return (__int64) internalTime.tv_sec * secondsToMicro + internalTime.tv_usec;
#endif
	}

    static __int64 getPrecision()
	{
#ifdef WIN32
		return frequency.QuadPart;
#else
		return secondsToMicro;
#endif
	}

    static float getElapsedTimeSeconds(__int64 lastTime)
	{
#ifdef WIN32
		return float(getCurrentTime() - lastTime)/frequency.QuadPart;
#else
		return float(getCurrentTime() - lastTime) /secondsToMicro;
#endif
	}
};

class FPSCounter
{
protected:
	__int64 frameStart;       // Mark the beginning of a frame
	float   frameInterval;    //The delay between two frames
	float	elapsedTime;
	float	internalFPS;      //An internal FPS counter
	float	tickCounter;      //This will count the clock ticks
	float	fps;              //The number of frames per second

public:
	FPSCounter();

	inline const float getFrameInterval() const {return frameInterval; }
	inline const float getElapsedTime()   const {return elapsedTime; }
	inline const float getFPS()           const {return fps; }

	virtual void  markFrameStart() { frameStart = Timer::getCurrentTime(); }
	virtual void  markFrameEnd();


};

inline FPSCounter::FPSCounter() : frameInterval(0.3f), internalFPS(0.0f), tickCounter(0.0f), elapsedTime(0.0f), frameStart(0), fps(0.0f)
{
  Timer::intialize();
}

inline void FPSCounter::markFrameEnd()
{
	if(frameStart)
	{
		frameInterval = Timer::getElapsedTimeSeconds(frameStart);
		tickCounter  += frameInterval;
		elapsedTime  += frameInterval;
		internalFPS++;

		if(tickCounter >= 1.0f)
		{
			fps          = internalFPS/tickCounter;
			internalFPS  = 0.0f;
			tickCounter  = 0.0f;
		}
	}
}


#endif
