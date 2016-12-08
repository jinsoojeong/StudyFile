#include <tchar.h>
#include <iostream>

class AbstractClass
{
private:
	virtual void subMethod() = 0;

public:
	void templateMethod()
	{
		subMethod();
	}
};


class ConcreateClass : public AbstractClass
{
public:
	void subMethod() override
	{
		std::cout << "subMethod" << std::endl;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	AbstractClass* test = new ConcreateClass;
	test->templateMethod();

	system("pause");
	return 0;
}

