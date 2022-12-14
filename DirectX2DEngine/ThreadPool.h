#pragma once
#include <functional>
#include <condition_variable>
#include <memory>
#include <thread>
#include <mutex>
#include <queue>

#define THREAD ThreadPool::GetInstance()
class ThreadPool
{
public:
	ThreadPool();
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
	~ThreadPool();

	static ThreadPool& GetInstance();

	void Enqueue(std::function<void()> task);

private:
	static std::unique_ptr<ThreadPool> pInstance;

	std::vector<std::thread> threads;
	std::queue<std::function<void()>> tasks;
	std::condition_variable eventVar;
	std::mutex eventMutex;
	bool bStopping = false;
	const size_t numThreads = 8u;
};

