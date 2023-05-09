#include "pch.h"
#include "CThreadPool.h"

CThreadPool::CThreadPool()
	: m_Numthreads(std::thread::hardware_concurrency())
{
	m_vecThreads.reserve(m_Numthreads);
	for (size_t i = 0; i < m_Numthreads; ++i)
	{
		m_vecThreads.emplace_back([this]()
			{
				this->WorkerThread();
				m_condition.notify_one();
			});
	}
}

void CThreadPool::Join()
{
	std::unique_lock<std::mutex> lock{ m_mutex };
	m_allFinished.wait(lock, [this]() { return this->m_vecTasks.empty() && m_busy == 0; });
}

void CThreadPool::WorkerThread()
{
	while (true)
	{
		std::unique_lock<std::mutex> guard{ m_mutex };
		m_condition.wait(guard, [this]() { return false == this->m_vecTasks.empty() || m_stop; });

		if (m_vecTasks.empty())
		{
			return;
		}

		std::function<void()> work = std::move(m_vecTasks.front());
		m_vecTasks.pop();
		guard.unlock();

		++m_busy;
		work();
		--m_busy;
		m_allFinished.notify_one();
	}
}

CThreadPool::~CThreadPool()
{
	m_stop = true;
	m_condition.notify_all();

	for (auto& jobThread : m_vecThreads)
	{
		jobThread.join();
	}
}



//#include "pch.h"
//#include "CThreadPool.h"
//
//
//CThreadPool::CThreadPool()
//{
//
//}
//
//CThreadPool::CThreadPool(int Threadsize)
//{
//    for (size_t i = 0; i < Threadsize; ++i)
//        m_vecThreads.emplace_back(
//            [this]
//            {
//                for (;;)
//                {
//                    std::function<void()> task;
//
//                    {
//                        std::unique_lock<std::mutex> lock(this->m_mutex);
//                        this->m_condition.wait(lock,
//                            [this] { return this->stop_all || !this->m_vecTasks.empty(); });
//                        if (this->stop_all && this->m_vecTasks.empty())
//                            return;
//                        task = std::move(this->m_vecTasks.front());
//                        this->m_vecTasks.pop();
//                    }
//
//                    task();
//                }
//            }
//            );
//}
//
//CThreadPool::~CThreadPool()
//{
//    std::unique_lock<std::mutex> lock(m_mutex);
//    stop_all = true;
//    
//    m_condition.notify_all();
//    for (std::thread& worker : m_vecThreads)
//        worker.join();
//
//
//
//}
//
//
//template<class F, class... Args>
//auto CThreadPool::enqueue(F&& f, Args&&... args)
//-> std::future<typename std::result_of<F(Args...)>::type>
//{
//    using return_type = typename std::result_of<F(Args...)>::type;
//
//    auto task = std::make_shared< std::packaged_task<return_type()> >(
//        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
//        );
//
//    std::future<return_type> res = task->get_future();
//    {
//        std::unique_lock<std::mutex> lock(m_mutex);
//
//        // don't allow enqueueing after stopping the pool
//        if (stop_all)
//            throw std::runtime_error("enqueue on stopped ThreadPool");
//
//        m_vecTasks.emplace([task]() { (*task)(); });
//    }
//    m_condition.notify_one();
//    return res;
//}