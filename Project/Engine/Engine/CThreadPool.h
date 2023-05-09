#pragma once

// thread
// thread.join()	해당하는 쓰레드들이 실행을 종료하면 리턴하는 함수 
// thread.detach()  해당 쓰레드를 실행 시킨 후, 잊어버리는 것  알아서 백그라운드에서 돌아감
// 기본적으로 프로세스가 종료될 때, 해당 프로세스 안에 있는 모든 쓰레드들은 종료 여부와 상관없이 자동으로 종료
// 쓰레드를 detach 하게 된다면 main 함수에서는 더이상 쓰레드들이 종료될 때 까지 기다리지 않습니다.
// thread::id this_id = std::this_thread::get_id(); 현재 돌아가고있는 쓰레드를 알 수 있음
//  서로 다른 쓰레드들이 동일한 자원을 사용할 때 발생하는 문제를 경쟁 상태(race condtion) 이라 부릅니다

// mutex : 쓰레드는 lock이 호출한 시점부터 unlock이 호출할 때 까지 mutex를 소유
// m.lock() 과 m.unlock() 사이에 한 쓰레드만이 유일하게 실행할 수 있는 코드 부분을 임계 영역(critical section) 이라고 부름
// lock한 쓰레드을 unlock하지않아 쓰레드들이 연산을 멈춘 상태를 데드락이라 함
// 데드락을 해결하기 위해 unique_ptr을 사용(unique_ptr은 메모리를 사용후 반드시 해제해야하므로, unique_ptr의 소멸자에서 처리
// lock_guard 객체는 뮤텍스를 인자로 받아서 생성하게 되는데, 이 때 생성자에서 뮤텍스를 lock 하게 됩니다. 그리고 lock_guard 가 소멸될 때 알아서 lock 했던 뮤텍스를 unlock 하게 됩니다.
// 내가 어떤 쓰레드 T 를 사용해서, 비동기적으로 값을 받아내겠다 라는 의미는, 미래에 (future) 쓰레드 T 가 원하는 데이터를 돌려 주겠다 라는 약속 (promise) 라고 볼 수 있습니다.
// promise는 객체를 정의할 때, 연산을 수행 후에 돌려줄 객체의 타입을 템플릿 인자로 받습니다
// 연산이 끝난 다음에 promise 객체는 자신이 가지고 있는 future 객체에 값을 넣어주게 됩니다. 이 때 promise 객체에 대응되는 future 객체는 위와 같이 get_future 함수를 통해서 얻을 수 있습니다.
// 하지만 data 가 아직은 실제 연산 결과를 포함하고 있는 것은 아닙니다. data 가 실제 결과를 포함하기 위해서는
// promise 객체가 자신의 future 객체에 데이터를 제공한 후에 대응되는 future 객체의 get 함수를 통해 얻어낼 수 있습니다

// promise 는 생산자-소비자 패턴에서 마치 생산자 (producer) 의 역할을 수행하고, future 는 소비자 (consumer) 의 역할을 수행


class CThreadPool
	: public CSingleton<CThreadPool>
{
public:
	CThreadPool();
	virtual ~CThreadPool();

	//// job 을 추가한다.
	//void EnqueueJob(std::function<void()> job);

/// @@
public:
	void Start()
	{
		m_stop = false;
		m_condition.notify_all();
	}

	void Join();
private:
	void WorkerThread();
/// @@
private:
	// 총 Worker 쓰레드의 개수.
	size_t								m_Numthreads;
	// Worker 쓰레드를 보관하는 벡터.
	std::vector<std::thread>			m_vecThreads;
	// 할일들을 보관하는 job 큐.
	std::queue<std::function<void()>>	m_vecTasks;
	// 위의 job 큐를 위한 cv 와 m.
	std::condition_variable				m_condition;
	std::mutex							m_mutex;

// @@
	std::condition_variable m_allFinished;
	std::atomic<int>        m_busy = 0;
	std::atomic<bool>       m_stop = false;
// @@

    //template<class F, class... Args>
    //auto enqueue(F&& f, Args&&... args) 
    //    -> std::future<typename std::result_of<F(Args...)>::type>;
    
	// 모든 쓰레드 종료
	bool stop_all;

// @@@
public:
	template <typename F, typename... Args>
	std::future<std::invoke_result_t<F, Args ...>> EnqueueJob(F&& f, Args&&...args);
// @@@
};

template <typename F, typename... Args>
std::future<std::invoke_result_t<F, Args ...>> CThreadPool::EnqueueJob(F&& f, Args&&...args)
{
	using return_t = std::invoke_result_t<F, Args ...>;
	auto pWork = std::make_shared<std::packaged_task<return_t()>>(std::bind(std::forward<F>(f),
		std::forward<Args>(args)...));
	std::future<return_t> resultFuture = pWork->get_future();

	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_vecTasks.push([pWork]() { (*pWork)(); });
	}


	return resultFuture;
}
