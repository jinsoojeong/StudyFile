#include <iostream>
#include <tchar.h>

class PartA
{

};

class PartB
{

};

class PartC
{

};

class Product
{
public:
	Product() : partA_(nullptr), partB_(nullptr), partC_(nullptr) {}
	~Product()
	{
		if (partA_ != nullptr)
			delete partA_;

		if (partB_ != nullptr)
			delete partB_;

		if (partC_ != nullptr)
			delete partC_;
	}

	void SetProcutPart(PartA* partA, PartB* partB, PartC* partC)
	{
		partA_ = partA;
		partB_ = partB;
		partC_ = partC;
	}

private:
	PartA* partA_;
	PartB* partB_;
	PartC* partC_;
};

class Builder
{
public:
	virtual Product* getResult(PartA* partA, PartB* partB, PartC* partC) = 0;
	virtual PartA* buildPartA() = 0;
	virtual PartB* buildPartB() = 0;
	virtual PartC* buildPartC() = 0;
};

class ConcreteBuilder : public Builder
{
public:
	PartA* buildPartA() override
	{
		return new PartA;
	}

	PartB* buildPartB() override
	{
		return new PartB;
	}

	PartC* buildPartC() override
	{
		return new PartC;
	}

	Product* getResult(PartA* partA, PartB* partB, PartC* partC) override
	{
		product_ = new Product;
		product_->SetProcutPart(partA, partB, partC);
		return product_;
	}

private:
	Product* product_;
};

class Director
{
public:
	Director(Builder* builder) : builder_(builder)
	{
	}

	~Director()
	{
		if (builder_ != nullptr)
			delete builder_;
	}

	Product* construct()
	{
		return builder_->getResult(builder_->buildPartA(), builder_->buildPartB(), builder_->buildPartC());
	}

private:
	Builder* builder_;
};

int _tmain(int argc, _TCHAR* argv[])
{
	Director* director = new Director(new ConcreteBuilder);
	Product* product = director->construct();

	delete director;
	delete product;

	system("pause");
	return 0;
}

// 예제. 기본형
//
//class Product
//{
//
//};
//
//class Builder
//{
//public:
//	virtual Product* getResult() = 0;
//	virtual void buildPart() = 0;
//};
//
//class ConcreteBuilder : public Builder
//{
//public:
//	void buildPart() override
//	{
//		product_ = new Product;
//	}
//
//	Product* getResult() override
//	{
//		return product_;
//	}
//
//private:
//	Product* product_;
//};
//
//class Director
//{
//public:
//	Director(Builder* builder) : builder_(builder)
//	{
//	}
//	
//	~Director()
//	{
//		if (builder_ != nullptr)
//			delete builder_;
//	}
//
//	Product* construct()
//	{
//		builder_->buildPart();
//		return builder_->getResult();
//	}
//
//private:
//	Builder* builder_;
//};
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	Director* director = new Director(new ConcreteBuilder);
//	Product* product = director->construct();
//
//	delete director;
//	delete product;
//
//	system("pause");
//	return 0;
//}