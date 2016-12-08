#include <iostream>
#include <list>
#include <tchar.h>

class Observer
{
public:
	virtual void update(std::string data) = 0;
};

class ConcreateObserver : public Observer
{
public:
	void update(std::string data) override 
	{
		std::cout << data.c_str() << " : update" << std::endl;
	}
};

class Subject
{
public:
	void attach(Observer* observer)
	{
		observer_list_.push_back(observer);
	}

	void detach(Observer* observer)
	{
		observer_list_.remove(observer);
	}

	virtual void change(std::string data) = 0;

protected:
	void notify(std::string data)
	{
		for each(std::list<Observer*>::value_type itor in observer_list_)
		{
			(*itor).update(data);
		}
	}

private:
	std::list<Observer*> observer_list_;
};

class ConcreateSubject : public Subject
{
public:
	void change(std::string data) override
	{
		notify(data);
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	Subject* pSubject = new ConcreateSubject;

	Observer* pObserver1 = new ConcreateObserver;
	Observer* pObserver2 = new ConcreateObserver;
	Observer* pObserver3 = new ConcreateObserver;

	pSubject->attach(pObserver1);
	pSubject->attach(pObserver2);
	pSubject->attach(pObserver3);

	pSubject->change("change notify");

	delete pSubject;
	delete pObserver1;
	delete pObserver2;
	delete pObserver3;

	system("pause");
	return 0;
}