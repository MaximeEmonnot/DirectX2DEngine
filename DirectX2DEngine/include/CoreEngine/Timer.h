#pragma once
#include <memory>
#include <chrono>
#include <functional>

// Updates the DELTATIME value.
#define TICKCLOCK Timer::GetInstance().Update();

#define DELTATIME Timer::GetInstance().DeltaTime()

#define TIMER Timer::GetInstance()

class Timer {
public:
	Timer();
	Timer(const Timer&) = delete;
	Timer& operator= (const Timer&) = delete;

	static Timer&	GetInstance();

	void			MeasureExecution(const std::function<void()>& function);

	void			Update();

	float			DeltaTime() const;
private:
	static std::unique_ptr<Timer>			pInstance;

	std::chrono::steady_clock::time_point	last;
	float									deltaTime;
};