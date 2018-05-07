#include "windows.h"  
#include <thread>  
#include <mutex>  
#include <deque>  
#include <vector>  
#include <condition_variable>  
#include <tchar.h> 


class CThreadDemo
{
private:
	std::deque<int> m_data;
	std::mutex m_mtx; // ȫ�ֻ�����.  
	std::condition_variable m_cv; // ȫ����������.  
	int       m_nGen;

private:
	void ProductThread() {
		while (true) {
			std::unique_lock <std::mutex> lck(m_mtx);
			m_nGen = ++m_nGen % 1000;
			printf("product %d\n", m_nGen);
			m_data.push_back(m_nGen);
			lck.unlock();
			m_cv.notify_all();

			/* �ȴ�1S */
			std::chrono::milliseconds dura(1000);
			std::this_thread::sleep_for(dura);
		}
	}

	void ConsumeThread() {
		while (true) {
			std::unique_lock <std::mutex> lck(m_mtx);
			while (m_data.empty()) {
				m_cv.wait(lck);
			}
			int nData = m_data.front();
			m_data.pop_front();
			printf("consume %d\n", nData);
			lck.unlock();

			/* �ȴ�2S */
			std::chrono::milliseconds dura(2000);
			std::this_thread::sleep_for(dura);
		}
	}
public:
	CThreadDemo() {
		m_data.clear();
		m_nGen = 0;
	}

	void Start() {
		std::vector<std::thread> threads;
		threads.clear();
		for (int i = 0; i < 5; i++) {/* �������߳� */
			threads.push_back(std::thread(&CThreadDemo::ProductThread, this));
		}
		for (int i = 5; i < 10; i++) {/* �������߳� */
			threads.push_back(std::thread(&CThreadDemo::ConsumeThread, this));
		}
		for (auto& t : threads) {/* �ȴ������̵߳��˳� */
			t.join();
		}
	}
};


int _tmain(int argc, _TCHAR* argv[])
{
	CThreadDemo test;
	test.Start();
	return 0;
}