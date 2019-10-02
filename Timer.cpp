// ----------------------------------------------------
// Timer.cpp
// Body for CPU Tick Timer class
// ----------------------------------------------------

#include "Timer.h"
double Timer::frequency = 0;

// ---------------------------------------------
Timer::Timer()
{
	if (frequency == 0)
		frequency = SDL_GetPerformanceFrequency();

	Start();
}

// ---------------------------------------------
void Timer::Start()
{
	running = true;
	started_at = SDL_GetTicks();
}

// ---------------------------------------------
void Timer::Stop()
{
	running = false;
	stopped_at = SDL_GetTicks();
}

// ---------------------------------------------
void Timer::Resume()
{
	running = true;
	started_at = SDL_GetTicks() - (stopped_at - started_at);
}
//-----------------------------------------------
void Timer::Reset()
{
	running = false;
	stopped_at = SDL_GetTicks();
	resume_at = SDL_GetTicks();
}

// ---------------------------------------------
Uint32 Timer::Read()
{
	if(running == true)
	{
		return SDL_GetTicks() - started_at;
	}
	else
	{
		return stopped_at - started_at;
	}
}

double Timer::ReadMs() const
{
	return 1000.0 * (double(SDL_GetTicks() - started_at) / double(frequency));
}

// ---------------------------------------------
double Timer::ReadTicks() const
{
	return SDL_GetTicks() - started_at;
}
float Timer::ReadSec() const
{
	return float(SDL_GetTicks() - started_at) / 1000.0f;
}


