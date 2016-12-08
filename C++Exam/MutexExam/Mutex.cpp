#include <iostream>
#include <thread>
#include <mutex>

std::mutex mutex;
int value = 0;

void lockguard()
{
	std::lock_guard<std::mutex> guard(mutex); // lock_guard() 사용
	value += 1;
}

void uniquelock()
{
	std::unique_lock<std::mutex> unique(mutex); // unique_lock() 사용
	value += 1;

}

int main()
{
	mutex.lock();	// lock() 사용
	value += 1;		//작업
	mutex.unlock();	// unlock() 사용

	if (mutex.try_lock() == true) // try_lock() 사용
	{
		value += 1;
		mutex.unlock();
	}
	else
		std::cout << "already used!" << std::endl;

	lockguard(); // lock_guard 사용

	uniquelock(); // unique_lock 사용

	system("pause");
	return 0;
}