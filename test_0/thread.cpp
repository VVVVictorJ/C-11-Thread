#include<iostream>
#include<thread>
#include <chrono>
#include<tchar.h>
#include <mutex>     
using namespace std;

std::timed_mutex mtx;

void myfirstThread(int num)
{
	while (mtx.try_lock_for(std::chrono::microseconds(200))) {
		for (int i = 0; i < 10; ++i) {

			//cout << "Hello world="<<num++<<"次" << endl;
			//cout << endl;
			printf("Hello world %i 次。\n", num);
			this_thread::sleep_for(chrono::milliseconds(10));
			num++;
		}
	}
	this_thread::sleep_for(chrono::milliseconds(10));
	mtx.unlock();
}
void mysecondThread(int &num)
{
	while (!mtx.try_lock_for(std::chrono::microseconds(200))) {
		this_thread::sleep_for(std::chrono::microseconds(10));
		for (int i = 0; i < 10; ++i) {
			//cout << "Fuck you" << num++ << "次" << endl;
			//cout << endl;
			printf("Fuck you %i 次。\n", num);
			this_thread::sleep_for(chrono::milliseconds(10));
			num++;
		}
	}
	this_thread::sleep_for(std::chrono::microseconds(10));
	mtx.unlock();
}

int _tmain(int argc, _TCHAR* argv[])
{
	int n = 1;
	thread myThread0;

	thread myThread1(myfirstThread, n);
	thread yourThread(mysecondThread, std::ref(n));
	myThread1.join();
	yourThread.join();

	n++;
	//printf("\nmove!\n");
	thread myThread3(std::move(yourThread));
	return system("pause");
}