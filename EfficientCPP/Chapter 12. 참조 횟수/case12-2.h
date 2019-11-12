#pragma once
#include "common12.h"

template <class T>
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
		init();
	}
	~RCIPtr()
	{
		if (counter)
			counter->removeReference();
	}
	RCIPtr& operator=(const RCIPtr& rhs)
	{
		if (counter != rhs.counter)
		{
			if (counter)
				counter->removeReference();

			counter = rhs.counter;
			init();
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
	};

	RCIPtr<T>::CountHolder *counter;
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
	RCIPtr<BigInt> value;
};