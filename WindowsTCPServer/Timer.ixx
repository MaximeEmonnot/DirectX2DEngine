export module Timer;

import <iostream>;
import <functional>;
import <chrono>;
import <memory>;

export class Timer
{
public:

	Timer() = default;

	static Timer& GetInstance()
	{
		if (!pInstance) pInstance = std::make_unique<Timer>();
		return *pInstance;
	}

	void MeasureExecution(const std::function<void()> & function)
	{
		auto start = std::chrono::high_resolution_clock::now();
		function();
		auto stop = std::chrono::high_resolution_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

		std::cout << "Time taken by function in microseconds : " << duration.count() << "\n";
	}

private:
	static std::unique_ptr<Timer> pInstance;
};
std::unique_ptr<Timer> Timer::pInstance = nullptr;

export Timer& TIMER = Timer::GetInstance();