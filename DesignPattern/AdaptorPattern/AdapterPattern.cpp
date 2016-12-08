#include <iostream>
#include <tchar.h>

class Adaptee
{
public:
	void adaptedOperation()
	{
		std::cout << "adaptedOperation µ¿ÀÛ" << std::endl;
	}
};

class Adapter
{
public:
	virtual void operation() = 0;
};

class ConcreateAdapterA : public Adapter
{
public:
	void operation() override
	{
		adaptee.adaptedOperation();
	}

private:
	Adaptee adaptee;
};

class ConcreateAdapterB : public Adapter, public Adaptee
{
public:
	void operation() override
	{
		adaptedOperation();
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	ConcreateAdapterA ObjectAdapter;
	ObjectAdapter.operation();

	ConcreateAdapterB ClassAdapter;
	ClassAdapter.operation();

	system("pause");
	return 0;
}