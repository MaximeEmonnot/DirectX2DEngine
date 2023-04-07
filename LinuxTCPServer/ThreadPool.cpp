#include "ThreadPool.h"

#include <iostream>

// STATIC VARIABLES INITIALIZATION
std::unique_ptr<ThreadPool> ThreadPool::pInstance = nullptr;

//******//

ThreadPool::ThreadPool()
{
	for (size_t i = 0; i < numThreads; ++i) {
		threads.emplace_back([&] {
			while (true) {
				std::function<void()> task;
				{
					// We wait for a notify...
					std::unique_lock<std::mutex> lock(eventMutex);
					eventVar.wait(lock, [&] { return bStopping || !tasks.empty(); });
					if (bStopping) break;

					//...so that we can execute a new task
					task = std::move(tasks.front());
					tasks.pop();
				}
				try {
					task();
				}
				catch (std::exception& e)
				{
					std::cerr << e.what() << std::endl;
				}
			}
			});
	}
}

ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex> lock(eventMutex);
		bStopping = true;
	}
	// We notify all waiting threads so we can make them join safely
	eventVar.notify_all();

	for (std::thread& thread : threads) thread.join();
}

ThreadPool& ThreadPool::GetInstance()
{
	if (!pInstance)
		pInstance = std::make_unique<ThreadPool>();
	return *pInstance;
}

void ThreadPool::Enqueue(std::function<void()> task)
{
	{
		std::unique_lock<std::mutex> lock(eventMutex);
		tasks.emplace(std::move(task));
	}
	eventVar.notify_one();
}