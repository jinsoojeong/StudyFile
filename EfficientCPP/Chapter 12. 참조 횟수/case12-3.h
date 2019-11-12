#pragma once

#include "common12.h"

class MutexLock
{
public:
	MutexLock() { mutex = CreateMutex(NULL, FALSE, NULL); }
	virtual ~MutexLock() { CloseHandle(mutex); }
	void lock() { WaitForSingleObject(mutex, INFINITE); }
	void unlock() { ReleaseMutex(mutex); }

private:
	HANDLE mutex;
};

template <class T, class LOCK>
class RCIPtr
{
public:
	RCIPtr(T* realPtr = nullptr) : counter(new CountHolder())
	{
		counter->pointee = realPtr;
		init();
	}
	RCIPtr(const RCIPtr& rhs) : counter(rhs.counter)
	{
		if (rhs.counter) rhs.counter->lock.lock();
		init();
		if (rhs.counter) rhs.counter->lock.unlock();
	}
	~RCIPtr()
	{
		if (counter)
		{
			counter->lock.lock();
			counter->removeReference();
			counter->lock.unlock();
		}

	}
	RCIPtr& operator=(const RCIPtr& rhs)
	{
		if (counter != rhs.counter)
		{
			if (counter)
			{
				counter->lock.lock();
				counter->removeReference();
				counter->lock.unlock();
			}

			counter = rhs.counter;

			if (rhs.counter) rhs.counter->lock.lock();
			init();
			if (rhs.counter) rhs.counter->lock.unlock();
		}

		return *this;
	}

	T* operator->() const { return counter->pointee; }
	T& operator*() const { return *(counter->pointee); }

private:
	struct CountHolder : public RCObject
	{
		~CountHolder() { delete pointee; }
		T* pointee;
		LOCK lock; // 잠금 클래스
	};

	RCIPtr<T, LOCK>::CountHolder *counter;
	void init()
	{
		if (counter == nullptr)
			return;

		if (counter->isshareable() == false)
		{
			counter = new CountHolder();
			counter->pointee = new T(*counter->pointee);
		}

		counter->addReference();
	}
};

class RCBigInt
{
	friend RCBigInt operator+ (const RCBigInt& left, const RCBigInt& right)
	{
		return RCBigInt(*(left.value) + *(right.value));
	}

public:
	RCBigInt(const char* p) : value(new BigInt(p)) {}
	RCBigInt(unsigned u = 0) : value(new BigInt(u)) {}
	RCBigInt(const BigInt& bi) : value(new BigInt(bi)) {}

private:
	RCIPtr<BigInt, MutexLock> value;
};