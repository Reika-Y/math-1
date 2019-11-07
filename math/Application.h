#pragma once
#include "Singleton.h"
#include "Geometry.h"

class Application:public Singleton<Application>
{
	friend Singleton<Application>;
public:
	int Initialize(void);
	void Run(void);
	void Terminate(void);
	const Size screenSize(void);
private:
	Application();
	~Application();

	const Size _screenSize;
};

