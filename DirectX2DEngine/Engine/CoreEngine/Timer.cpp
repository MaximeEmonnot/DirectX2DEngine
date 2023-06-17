#include "CoreEngine/Timer.h"

#include <iostream>

#include "DebugEngine/LoggerManager.h"

// STATIC VARIABLES INITIALIZATION
std::unique_ptr<Timer> Timer::pInstance = nullptr;

//******//

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

void Timer::MeasureExecution(const std::function<void()>& function)
{
	auto start = std::chrono::high_resolution_clock::now();
	function();
	auto stop = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	LOG(std::string("Time taken by function in microseconds : ") + std::to_string(duration.count()) + std::string("\n"), LOG_DEBUG);
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
