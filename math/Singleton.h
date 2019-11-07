#pragma once

// シングルトンテンプレートクラス
// 参照で返す
template<typename T>
class Singleton
{
public:
	static T& Instance()
	{
		static T _instance;
		return _instance;
	}
protected:
	Singleton();
	virtual ~Singleton();
private:
	// コピー禁止
	Singleton(const Singleton<T>&) {};
	// 代入禁止
	void operator = (const Singleton<T>&) {};
};

template<typename T>
Singleton<T>::Singleton()
{
}

template<typename T>
Singleton<T>::~Singleton()
{
}
