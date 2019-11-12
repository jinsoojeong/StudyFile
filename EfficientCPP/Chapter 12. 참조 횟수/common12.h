#pragma once

#include <iostream>
#include <list>
#include <set>
#include <array>
#include <vector>
#include <string>
#include <mutex>
#include <algorithm>
#include <Windows.h>

class BigInt : public RCObject
{
	friend BigInt operator+ (const BigInt& lhs, const BigInt& rhs)
	{
		return BigInt(lhs, rhs);
	}

public:
	BigInt(const char* s)
	{
		if (s[0] == '\0')
			s = "0";

		size = ndigits = strlen(s);
		digits = new char[size];
		for (unsigned i = 0; i < ndigits; ++i)
			digits[i] = s[ndigits - 1 - i] - '0';
	}
	BigInt(unsigned u = 0)
	{
		unsigned v = u;

		for (ndigits = 1; (v /= 10) > 0; ++ndigits) {}
		size = ndigits;
		digits = new char[size];

		for (unsigned i = 0; i < ndigits; ++i)
		{
			digits[i] = u % 10;
			u /= 10;
		}
	}
	BigInt(const BigInt& copyFrom) // 복사 생성자
	{
		size = ndigits = copyFrom.ndigits;
		digits = new char[size];

		for (unsigned i = 0; i < ndigits; ++i)
			digits[i] = copyFrom.digits[i];
	}
	BigInt& operator= (const BigInt& rhs)
	{
		if (this == &rhs)
			return *this;

		ndigits = rhs.ndigits;

		if (ndigits > size)
		{
			delete[] digits;
			size = ndigits;
			digits = new char[size];

			for (unsigned i = 0; i < ndigits; ++i)
				digits[i] = rhs.digits[i];

			return *this;
		}
	}
	BigInt& operator+= (const BigInt& rhs)
	{
		unsigned max = 1 + (rhs.ndigits > ndigits ? rhs.ndigits : ndigits);

		if (size < max)
		{
			size = max;
			char* d = new char[size];

			for (unsigned i = 0; i < ndigits; ++i)
				d[i] = digits[i];

			delete[] digits;
			digits = d;
		}

		while (ndigits < max)
			digits[ndigits++] = 0;

		for (unsigned i = 0; i < ndigits; ++i)
		{
			digits[i] += rhs.fetch(i);
			if (digits[i] >= 10)
			{
				digits[i] -= 10;
				digits[i + 1] += 1;
			}
		}

		if (digits[ndigits - 1] == 0)
			--ndigits;

		return *this;
	}
	~BigInt()
	{
		delete[] digits;
	}

private:
	char* digits;
	unsigned ndigits;
	unsigned size; // 할당된 문자열의 크기
	BigInt(const BigInt& left, const BigInt& right) // 연산 생성자
	{
		size = 1 + (left.ndigits > right.ndigits ? left.ndigits : right.ndigits);
		digits = new char[size];
		ndigits = left.ndigits;

		for (unsigned i = 0; i < ndigits; ++i)
			digits[i] = left.digits[i];

		*this += right;
	}
	inline char fetch(unsigned i) const
	{
		return i < ndigits ? digits[i] : 0;
	}

};

class RCObject
{
public:
	void addReference() { ++refCount; }
	void removeRefCount() { if (--refCount == 0) delete this; }

	void markunshareable() { shareable = false; }
	bool isshareable() const { return shareable; }
	bool isshared() const { return refCount > 1; }

protected:
	RCObject() : refCount(0), shareable(true) {}
	RCObject(const RCObject& rhs) : refCount(0), shareable(true) {}
	RCObject& operator= (const RCObject& rhs) { return *this; }
	virtual ~RCObject() {}

private:
	int refCount;
	bool shareable;
};