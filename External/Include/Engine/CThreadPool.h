#pragma once

// thread
// thread.join()	�ش��ϴ� ��������� ������ �����ϸ� �����ϴ� �Լ� 
// thread.detach()  �ش� �����带 ���� ��Ų ��, �ؾ������ ��  �˾Ƽ� ��׶��忡�� ���ư�
// �⺻������ ���μ����� ����� ��, �ش� ���μ��� �ȿ� �ִ� ��� ��������� ���� ���ο� ������� �ڵ����� ����
// �����带 detach �ϰ� �ȴٸ� main �Լ������� ���̻� ��������� ����� �� ���� ��ٸ��� �ʽ��ϴ�.
// thread::id this_id = std::this_thread::get_id(); ���� ���ư����ִ� �����带 �� �� ����
//  ���� �ٸ� ��������� ������ �ڿ��� ����� �� �߻��ϴ� ������ ���� ����(race condtion) �̶� �θ��ϴ�

// mutex : ������� lock�� ȣ���� �������� unlock�� ȣ���� �� ���� mutex�� ����
// m.lock() �� m.unlock() ���̿� �� �����常�� �����ϰ� ������ �� �ִ� �ڵ� �κ��� �Ӱ� ����(critical section) �̶�� �θ�
// lock�� �������� unlock�����ʾ� ��������� ������ ���� ���¸� ������̶� ��
// ������� �ذ��ϱ� ���� unique_ptr�� ���(unique_ptr�� �޸𸮸� ����� �ݵ�� �����ؾ��ϹǷ�, unique_ptr�� �Ҹ��ڿ��� ó��
// lock_guard ��ü�� ���ؽ��� ���ڷ� �޾Ƽ� �����ϰ� �Ǵµ�, �� �� �����ڿ��� ���ؽ��� lock �ϰ� �˴ϴ�. �׸��� lock_guard �� �Ҹ�� �� �˾Ƽ� lock �ߴ� ���ؽ��� unlock �ϰ� �˴ϴ�.
// ���� � ������ T �� ����ؼ�, �񵿱������� ���� �޾Ƴ��ڴ� ��� �ǹ̴�, �̷��� (future) ������ T �� ���ϴ� �����͸� ���� �ְڴ� ��� ��� (promise) ��� �� �� �ֽ��ϴ�.
// promise�� ��ü�� ������ ��, ������ ���� �Ŀ� ������ ��ü�� Ÿ���� ���ø� ���ڷ� �޽��ϴ�
// ������ ���� ������ promise ��ü�� �ڽ��� ������ �ִ� future ��ü�� ���� �־��ְ� �˴ϴ�. �� �� promise ��ü�� �����Ǵ� future ��ü�� ���� ���� get_future �Լ��� ���ؼ� ���� �� �ֽ��ϴ�.
// ������ data �� ������ ���� ���� ����� �����ϰ� �ִ� ���� �ƴմϴ�. data �� ���� ����� �����ϱ� ���ؼ���
// promise ��ü�� �ڽ��� future ��ü�� �����͸� ������ �Ŀ� �����Ǵ� future ��ü�� get �Լ��� ���� �� �� �ֽ��ϴ�

// promise �� ������-�Һ��� ���Ͽ��� ��ġ ������ (producer) �� ������ �����ϰ�, future �� �Һ��� (consumer) �� ������ ����


class CThreadPool
	: public CSingleton<CThreadPool>
{
public:
	CThreadPool();
	virtual ~CThreadPool();

	//// job �� �߰��Ѵ�.
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
	// �� Worker �������� ����.
	size_t								m_Numthreads;
	// Worker �����带 �����ϴ� ����.
	std::vector<std::thread>			m_vecThreads;
	// ���ϵ��� �����ϴ� job ť.
	std::queue<std::function<void()>>	m_vecTasks;
	// ���� job ť�� ���� cv �� m.
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
    
	// ��� ������ ����
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
