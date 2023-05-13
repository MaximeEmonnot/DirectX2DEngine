#pragma once
#include <condition_variable>
#include <thread>
#include <mutex>
#include <functional>
#include <queue>
#include <memory>

#define THREAD ThreadPool::GetInstance()
class ThreadPool
{
public:
	ThreadPool();
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
	~ThreadPool();

	static ThreadPool& GetInstance();

	// Enqueues a task that will be executed by one of the threads in the pool.
	void				Enqueue(std::function<void()> task);

private:
	static std::unique_ptr<ThreadPool>	pInstance;

	const size_t						numThreads = 8u;
	std::vector<std::thread>			threads;
	std::queue<std::function<void()>>	tasks;
	std::condition_variable				eventVar;
	std::mutex							eventMutex;
	bool								bStopping = false;
};

