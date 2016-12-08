#include <tchar.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <mutex>

// 6. 스마트 포인터 shared_ptr을 활용한 싱글턴 패턴
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
			// 객체 소멸
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

// 5. 멀티 스레드 기반에서 싱글턴 패턴 구현
// volate 변수 - 컴파일러가 메모리 접근을 최소화 시키는 최적화를 하지 못하게 막는 예약어
// 추천하진 않음 블로그에 안넣음
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
//			atexit(destroy); // 종료 함수 등록
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
//			// 객체 소멸
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

// 4. 템플릿을 이용한 싱글턴 패턴 구현
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
//			atexit(destroy); // 종료 함수 등록
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
//			// 객체 소멸
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

// 3. atexit() 함수를 이용한 싱글턴
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
//			// 객체 소멸
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
//	// main 함수 종료 직전 명시적으로 해제
//	pSingleton->destroy();
//}