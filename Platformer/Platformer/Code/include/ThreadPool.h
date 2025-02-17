#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include "ResourceManager.h"

class ThreadPool
{
public:
	ThreadPool() : ThreadPool(10) {}
	ThreadPool(uint32_t _nbThreads);
	~ThreadPool();

	void Enqueue(std::function<void()> const& func);
	void WaitUntilFinished();

private:
	void CheckQueue();

	std::vector<std::thread> workers;
	std::deque<std::function<void()>> taskQueue;
	std::mutex queueMutex;
	std::condition_variable waitCondition;
	std::condition_variable finishCondition;
	std::atomic<bool> stop{false};
	std::atomic<int> tasksRemaining{0};
	uint32_t nbThreads = 0;
};