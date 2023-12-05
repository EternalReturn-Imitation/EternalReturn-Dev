#pragma once

typedef void (*EXIT)(void);

template<typename T>
class CSingleton
{
private:
	static T* m_Inst;

public:
	static T* GetInst();
	static void Destroy();

protected:
	CSingleton() { }
	virtual ~CSingleton() {}	
};

template<typename T>
T* CSingleton<T>::m_Inst = nullptr;

template<typename T>
inline T* CSingleton<T>::GetInst()
{
	if (nullptr == m_Inst)
	{
		m_Inst = new T;
		atexit( (EXIT) &CSingleton<T>::Destroy);
	}

	return m_Inst;
}

template<typename T>
inline void CSingleton<T>::Destroy()
{
	if (nullptr != m_Inst)
	{
		delete m_Inst;
		m_Inst = nullptr;
	}
}
