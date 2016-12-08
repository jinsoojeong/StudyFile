#include <iostream>
#include <tchar.h>

class State
{
public:
	virtual void handle() = 0;
};

class ConcreateState1 : public State
{
public:
	void handle() override
	{
		std::cout << "ConcreateState1" << std::endl;
	}
};

class ConcreateState2 : public State
{
public:
	void handle() override
	{
		std::cout << "ConcreateState2" << std::endl;
	}
};

class Context
{
public:
	Context(State* state) : state_(state) {}
	~Context() 
	{
		if (state_ != nullptr)
			delete state_;
	}

	void SetState(State* state)
	{
		if (state_ != nullptr)
			delete state_;

		state_ = state;
	}

	void request()
	{
		state_->handle();
	}

private:
	State* state_;
};

int _tmain(int atgc, _TCHAR* argv[])
{
	Context* context = new Context(new ConcreateState1);
	context->request();

	context->SetState(new ConcreateState2);
	context->request();

	delete context;
	system("pause");
	return 0;
}