export module ThreadPool;

import <condition_variable>;
import <thread>;
import <mutex>;
import <functional>;
import <queue>;
import <memory>;
import <iostream>;

export class ThreadPool
{
public:
	ThreadPool()
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
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
	~ThreadPool()
	{
		{
			std::unique_lock<std::mutex> lock(eventMutex);
			bStopping = true;
		}
		// We notify all waiting threads so we can make them join safely
		eventVar.notify_all();

		for (std::thread& thread : threads) thread.join();
	}

	static ThreadPool& GetInstance()
	{
		if (!pInstance)
			pInstance = std::make_unique<ThreadPool>();
		return *pInstance;
	}

	// Enqueues a task that will be executed by one of the threads in the pool.
	void				Enqueue(std::function<void()> task)
	{
		{
			std::unique_lock<std::mutex> lock(eventMutex);
			tasks.emplace(std::move(task));
		}
		eventVar.notify_one();
	}

private:
	static std::unique_ptr<ThreadPool>	pInstance;

	const size_t						numThreads = 8u;
	std::vector<std::thread>			threads;
	std::queue<std::function<void()>>	tasks;
	std::condition_variable				eventVar;
	std::mutex							eventMutex;
	bool								bStopping = false;
};
// STATIC VARIABLES INITIALIZATION
std::unique_ptr<ThreadPool> ThreadPool::pInstance = nullptr;

export ThreadPool& THREAD = ThreadPool::GetInstance();