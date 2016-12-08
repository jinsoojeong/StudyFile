#include <iostream>

class Strategy
{
public:
	virtual void AlgorithmInterface() = 0;
};

//Strategy Algorithm A
class ConcreateStrategyA : public Strategy
{
public:
	void AlgorithmInterface() override { std::cout << "Processed by Strategy A" << std::endl; }
};

//Strategy Algorithm B
class ConcreateStrategyB : public Strategy
{
public:
	void AlgorithmInterface() override { std::cout << "Processed by Strategy B" << std::endl; }
};

//Strategy Algorithm C
class ConcreateStrategyC : public Strategy
{
public:
	void AlgorithmInterface() override { std::cout << "Processed by Strategy C" << std::endl; }
};

//Context
class Context
{
public:
	Context() : pStrategy_() {}
	~Context() 
	{ 
		if (pStrategy_) 
			delete pStrategy_; 
	}

	void ChangeStrategy(Strategy* pStrategy)
	{
		if (pStrategy_ != nullptr) 
			delete pStrategy_;

		pStrategy_ = pStrategy;
	}

	void ContextInterface() 
	{ 
		if (pStrategy_)
			pStrategy_->AlgorithmInterface();
	}

private:
	Strategy* pStrategy_;
};

int main()
{
	Context* pContext = new Context();
	pContext->ChangeStrategy(new ConcreateStrategyA());
	pContext->ContextInterface();

	pContext->ChangeStrategy(new ConcreateStrategyB());
	pContext->ContextInterface();

	pContext->ChangeStrategy(new ConcreateStrategyC());
	pContext->ContextInterface();
	delete pContext;

	return 0;
}