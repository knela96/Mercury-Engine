#ifndef __TIMER_H__
#define __TIMER_H__

#include "Globals.h"
#include "SDL.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	void Stop();
	void Resume();

	void Reset();

	Uint32 Read();

	double ReadMs() const;
	double ReadTicks() const;
	float ReadSec() const;

private:

	bool	running;
	Uint32	started_at;
	Uint32	stopped_at;
	Uint32  resume_at;
	static double frequency;
};

#endif //__TIMER_H__