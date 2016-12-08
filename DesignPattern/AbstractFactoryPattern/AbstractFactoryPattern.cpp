#include <iostream>
#include <tchar.h>

// 예제2. 템플릿을 활용한 추상 팩토리 패턴
//
class AbstractProductA
{
public:
	virtual void Print() = 0;
};

class AbstractProductB
{
public:
	virtual void Print() = 0;
};

class ConcreteProductA : public AbstractProductA
{
public:
	void Print() override
	{
		std::cout << "ConcreteProductA" << std::endl;
	}
};

class ConcreteProductB : public AbstractProductB
{
public:
	void Print() override
	{
		std::cout << "ConcreteProductB" << std::endl;
	}
};

template<typename T>
class AbtractFactory
{
public:
	T* Create()
	{
		return CreateProduct();
	}

protected:
	virtual T* CreateProduct() = 0;
};

template<typename T1, typename T2>
class ConcreteFactory : public AbtractFactory<T1>
{
private:
	T1* CreateProduct() override
	{
		return new T2;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	ConcreteFactory<AbstractProductA, ConcreteProductA> producA_factory;
	ConcreteFactory<AbstractProductB, ConcreteProductB> producB_factory;

	AbstractProductA* apA = producA_factory.Create();
	AbstractProductB* apB = producB_factory.Create();

	apA->Print();
	apB->Print();

	delete apA;
	delete apB;

	system("pause");
	return 0;
}

// 예제 1. 일반 적인 추상 팩토리 사용
//
//class AbstractProductA
//{
//public:
//	virtual void Print() = 0;
//};
//
//class AbstractProductB
//{
//public:
//	virtual void Print() = 0;
//};
//
//class ConcreteProductA : public AbstractProductA
//{
//public:
//	void Print() override
//	{
//		std::cout << "ConcreteProductA" << std::endl;
//	}
//};
//
//class ConcreteProductB : public AbstractProductB
//{
//public:
//	void Print() override
//	{
//		std::cout << "ConcreteProductB" << std::endl;
//	}
//};
//
//class AbtractFactory
//{
//public:
//	virtual AbstractProductA* createProductA() = 0;
//	virtual AbstractProductB* createProductB() = 0;
//};
//
//class ConcreteFactory : public AbtractFactory
//{
//public:
//	AbstractProductA* createProductA() override
//	{
//		return new ConcreteProductA;
//	}
//
//	AbstractProductB* createProductB() override
//	{
//		return new ConcreteProductB;
//	}
//};
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	ConcreteFactory factory;
//
//	AbstractProductA* apA = factory.createProductA();
//	apA->Print();
//
//	AbstractProductB* apB = factory.createProductB();
//	apB->Print();
//
//	system("pause");
//	return 0;
//}