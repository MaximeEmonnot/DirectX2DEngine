#include "Timer.h"

// STATIC VARIABLES INITIALIZATION
std::unique_ptr<Timer> Timer::pInstance = nullptr;

Timer::Timer()
	:
	last(std::chrono::steady_clock::now()),
	deltaTime(0.f)
{}

Timer& Timer::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<Timer>();
	return *pInstance;
}

void Timer::Update()
{
	const std::chrono::steady_clock::time_point old = last;
	last = std::chrono::steady_clock::now();
	deltaTime = std::chrono::duration<float>(last - old).count();
}

float Timer::DeltaTime() const
{
	return deltaTime;
}
