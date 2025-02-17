#include "ThreadPool.h"
#include "ResourceManager.h"
#include <tracy/Tracy.hpp>

ThreadPool::ThreadPool(uint32_t _nbThreads) : nbThreads(_nbThreads)
{
	for (uint32_t i = 0; i < nbThreads; i++)
		workers.emplace_back(&ThreadPool::CheckQueue, this);
}

ThreadPool::~ThreadPool()
{
    stop.store(true, std::memory_order_release);
    waitCondition.notify_all();

    for (auto& worker : workers)
        if (worker.joinable())
            worker.join();

    workers.clear();
}

void ThreadPool::Enqueue(std::function<void()> const& func)
{
    ZoneScoped;
    tasksRemaining.fetch_add(1, std::memory_order_acq_rel);
    {
        std::unique_lock lock(queueMutex);
        taskQueue.emplace_back(func);
    }
    waitCondition.notify_one();
}

void ThreadPool::WaitUntilFinished()
{
    ZoneScoped;
    std::unique_lock lock(queueMutex);
    finishCondition.wait(lock, [this]() { return tasksRemaining.load(std::memory_order_acquire) == 0 && taskQueue.empty(); });
}

void ThreadPool::CheckQueue()
{
    ZoneScoped;
    while (true)
    {
        std::function<void()> task;

        {
            std::unique_lock lock(queueMutex);
            waitCondition.wait(lock, [this]() { return stop.load() || !taskQueue.empty(); });

            if (stop.load() && taskQueue.empty())
                return;

            task = std::move(taskQueue.front());
            taskQueue.pop_front();
        }

        task();
        tasksRemaining.fetch_sub(1, std::memory_order_acq_rel);
        finishCondition.notify_one();
    }
}
