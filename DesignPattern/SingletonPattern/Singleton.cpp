#include <tchar.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <mutex>

// 6. ����Ʈ ������ shared_ptr�� Ȱ���� �̱��� ����
//
template<typename T>
class Singleton
{
public:
	Singleton() {}
	virtual ~Singleton() {}

	Singleton(const Singleton& s) {}

	static T* GetInstance()
	{
		std::call_once(once_flag_, [] {
			pInstance_.reset(new T);
		});

		return pInstance_.get();
	}

	static void destroy()
	{
		if (pInstance_ != nullptr)
		{
			delete pInstance_;
			// ��ü �Ҹ�
		}
	}

private:
	static std::shared_ptr<T> pInstance_;
	static std::once_flag once_flag_;
};

template<typename T> std::shared_ptr<T> Singleton<T>::pInstance_ = nullptr;
template<typename T> std::once_flag Singleton<T>::once_flag_;

class TemplateTest : public Singleton<TemplateTest>
{
public:
	void template_singleton_test()
	{
		std::cout << "template singleton test" << std::endl;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	TemplateTest::GetInstance()->template_singleton_test();

	system("pause");
	return 0;
}

// 5. ��Ƽ ������ ��ݿ��� �̱��� ���� ����
// volate ���� - �����Ϸ��� �޸� ������ �ּ�ȭ ��Ű�� ����ȭ�� ���� ���ϰ� ���� �����
// ��õ���� ���� ��α׿� �ȳ���
//template<typename T>
//class Singleton : public CMultiThreadSync<T>
//{
//public:
//	Singleton() {}
//	virtual ~Singleton() {}
//
//	Singleton(const Singleton& s) {}
//
//	static T* GetInstance()
//	{
//		if (pInstance_ == nullptr)
//		{ 
//			CThreadSync cs;
//
//			pInstance_ = new T;
//			atexit(destroy); // ���� �Լ� ���
//		}
//
//		return pInstance_;
//	}
//
//	static void destroy()
//	{
//		if (pInstance_ != nullptr)
//		{
//			delete pInstance_;
//			// ��ü �Ҹ�
//		}
//	}
//
//private:
//	static T* volatile pInstance_;
//};
//
//template <typename T> T* Singleton <T>::pInstance_;
//
//class TemplateTest : public Singleton<TemplateTest>
//{
//public:
//	void template_singleton_test()
//	{
//		std::cout << "template singleton test" << std::endl;
//	}
//};
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	TemplateTest::GetInstance()->template_singleton_test();
//
//	return 0;
//}

// 4. ���ø��� �̿��� �̱��� ���� ����
//
//template<typename T>
//class Singleton
//{
//public:
//	Singleton() {}
//	virtual ~Singleton() {}
//
//	Singleton(const Singleton& s) {}
//
//	static T* GetInstance()
//	{
//		if (pInstance_ == nullptr)
//		{ 
//			pInstance_ = new T;
//			atexit(destroy); // ���� �Լ� ���
//		}
//
//		return pInstance_;
//	}
//
//	static void destroy()
//	{
//		if (pInstance_ != nullptr)
//		{
//			delete pInstance_;
//			// ��ü �Ҹ�
//		}
//	}
//
//private:
//	static T* pInstance_;
//};
//
//template <typename T> T* Singleton <T>::pInstance_;
//
//class TemplateTest : public Singleton<TemplateTest>
//{
//public:
//	void template_singleton_test()
//	{
//		std::cout << "template singleton test" << std::endl;
//	}
//};
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	TemplateTest::GetInstance()->template_singleton_test();
//
//	return 0;
//}

// 3. atexit() �Լ��� �̿��� �̱���
//
//class Singleton
//{
//public:
//	Singleton() {}
//	~Singleton() {}
//
//	Singleton(const Singleton& s) {}
//
//	static Singleton* GetInstance()
//	{
//		if (pInstance_ == nullptr)
//		{ 
//			pInstance_ = new Singleton;
//			atexit(destroy);
//		}
//
//		return pInstance_;
//	}
//
//	static void destroy()
//	{
//		if (pInstance_ != nullptr)
//		{
//			delete pInstance_;
//			// ��ü �Ҹ�
//		}
//	}
//
//private:
//	static Singleton* pInstance_;
//};
//
//Singleton* Singleton::pInstance_ = nullptr;
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	Singleton* pSingleton = Singleton::GetInstance();
//
//	return 0;
//}

// 2. Static Singleton
//
//class Singleton
//{
//public:
//	Singleton() {}
//	~Singleton() {}
//	Singleton(const Singleton& s) {}
//
//	static Singleton* GetInstance()
//	{
//		return &pInstance_;
//	}
//
//private:
//	static Singleton pInstance_;
//};
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	Singleton* pSingleton = Singleton::GetInstance();
//
//	return 0;
//}


// 1. Dynamic Singleton
//
//class Singleton
//{
//public:
//	Singleton() {}
//	~Singleton() {}
//
//	Singleton(const Singleton& s) {}
//
//	static Singleton* GetInstance()
//	{
//		if (pInstance_ == nullptr)
//			pInstance_ = new Singleton;
//
//		return pInstance_;
//	}
//
//	void destroy()
//	{
//		if (pInstance_ != nullptr)
//		{
//			delete pInstance_;
//			pInstance_ = nullptr;
//		}
//	}
//
//private:
//	static Singleton* pInstance_;
//};
//
//Singleton* Singleton::pInstance_ = nullptr;
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	Singleton* pSingleton = Singleton::GetInstance();
//
//	Singleton* pSingleton2 = Singleton::GetInstance();
//
//	// main �Լ� ���� ���� ��������� ����
//	pSingleton->destroy();
//}