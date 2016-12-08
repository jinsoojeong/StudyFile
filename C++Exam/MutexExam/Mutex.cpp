#include <iostream>
#include <thread>
#include <mutex>

std::mutex mutex;
int value = 0;

void lockguard()
{
	std::lock_guard<std::mutex> guard(mutex); // lock_guard() ���
	value += 1;
}

void uniquelock()
{
	std::unique_lock<std::mutex> unique(mutex); // unique_lock() ���
	value += 1;

}

int main()
{
	mutex.lock();	// lock() ���
	value += 1;		//�۾�
	mutex.unlock();	// unlock() ���

	if (mutex.try_lock() == true) // try_lock() ���
	{
		value += 1;
		mutex.unlock();
	}
	else
		std::cout << "already used!" << std::endl;

	lockguard(); // lock_guard ���

	uniquelock(); // unique_lock ���

	system("pause");
	return 0;
}