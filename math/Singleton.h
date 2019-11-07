#pragma once

// �V���O���g���e���v���[�g�N���X
// �Q�ƂŕԂ�
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
	// �R�s�[�֎~
	Singleton(const Singleton<T>&) {};
	// ����֎~
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
