#pragma once
#include "common12.h"

template<class T>
class RCPtr
{
public:
	RCPtr(T* realPtr = nullptr) : pointee(realPtr) { init(); }
	RCPtr(const RCPtr& rhs) : pointee(rhs.pointee) { init(); }
	~RCPtr()
	{
		if (pointee)
			pointee->removeReference();
	}

	RCPtr& operator=(const RCPtr& rhs)
	{
		if (pointee != rhs.pointee)
		{
			if (pointee)
				pointee->removeReference();
			pointee = rhs.pointee;
			init();
		}

		return *this;
	}

	T* operator->() const { return pointee; }
	T& operator*() const { return *pointee; }

private:
	T * pointee;
	void init()
	{
		if (pointee == nullptr)
			return;

		if (pointee->isshareable())
			pointee = new T(*pointee);

		pointee->addReference();
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
	RCPtr<BigInt> value;
};
