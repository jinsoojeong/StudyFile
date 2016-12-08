#include <iostream>
#include <tchar.h>

class Product
{
public:
	virtual void print() = 0;
};

class ConcreteProduct : public Product
{
public:
	void print() override
	{
		std::cout << "ConcreateProduct" << std::endl;
	}
};

class Creator
{
public:
	Product* AnOperation()
	{
		return FactoryMethod();
	}

protected:
	virtual Product* FactoryMethod() = 0;
};

class ConcreteCreator : public Creator
{
private:
	Product* FactoryMethod()
	{
		return new ConcreteProduct;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	ConcreteCreator creator;

	Product* product = creator.AnOperation();
	product->print();

	delete product;

	system("pause");
	return 0;
}