#include "CoreEngine/ThreadPool.h"

#include "DebugEngine/EngineException.h"

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
					std::unique_lock<std::mutex> lock(eventMutex);
					eventVar.wait(lock, [&] { return bStopping || !tasks.empty(); });
					if (bStopping) break;

					task = std::move(tasks.front());
					tasks.pop();
				}
				try {
					task();
				}
				catch (EngineException& e)
				{
					SHOW_MESSAGE("DirectX 2D Engine - Engine Exception", e.what());
				}
				catch (std::exception& e)
				{
					SHOW_MESSAGE("DirectX 2D Engine - Standard Library Exception", e.what());
				}
				catch (...)
				{
					SHOW_MESSAGE("DirectX 2D Engine - Unknown Exception", "An unknown exception has been caught.");
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

void ThreadPool::KillPool()
{
}
