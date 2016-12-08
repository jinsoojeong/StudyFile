#include <tchar.h>
#include <iostream>

class Prototype
{
public:
	virtual Prototype* Clone() = 0;
	virtual void Print() = 0;
	virtual void Setnum(int n) = 0;
};

class ConcretePrototype1 : public Prototype
{
public:
	ConcretePrototype1() {}
	ConcretePrototype1(const ConcretePrototype1& p) 
	{
		num_ = p.num_;
	}

	Prototype* Clone() override final
	{
		return new ConcretePrototype1(*this);
	}

	void Print() override
	{
		std::cout << "concretePtototype1 : " << num_ << std::endl;
	}

	void Setnum(int n) override
	{
		num_ = n;
	}

	int num_ = 0;
};

class ConcretePrototype2 : public Prototype
{
public:
	ConcretePrototype2() {}
	ConcretePrototype2(const ConcretePrototype2& p) 
	{
		num_ = p.num_;
	}

	Prototype* Clone() override final
	{
		return new ConcretePrototype2(*this);
	}

	void Print() override
	{
		std::cout << "concretePtototype2 : " << num_ << std::endl;
	}

	void Setnum(int n) override
	{
		num_ = n;
	}

	int num_ = 0;
};

int _tmain(int argc, _TCHAR* argv[])
{
	Prototype* original = new ConcretePrototype1();
	original->Setnum(10);
	Prototype* clone = original->Clone();

	std::cout << "original : ";
	original->Print();

	std::cout << "clone : ";
	clone->Print();

	delete original;
	delete clone;

	system("pause");
	return 0;
}