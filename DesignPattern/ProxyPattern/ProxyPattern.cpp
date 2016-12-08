#include <iostream>
#include <tchar.h>

class Subject
{
public:
	virtual void request() = 0;
};

class RealSubject : public Subject
{
public:
	void request()
	{
		std::cout << "Response by RealSubject" << std::endl;
	}
};

class Proxy : public Subject
{
public:
	Proxy() : real_subject_(nullptr) {}
	~Proxy()
	{
		if (real_subject_)
			delete real_subject_;
	}

	void request()
	{
		if (real_subject_ == nullptr)
			real_subject_ = new RealSubject;

		real_subject_->request();
	}

private:
	RealSubject* real_subject_;
};

int _tmain(int argc, _TCHAR* argv[])
{
	Subject* proxy = new Proxy();
	proxy->request();
	delete proxy;

	system("pause");
	return 0;
}