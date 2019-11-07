#include "Application.h"

int main() 
{
	Application::Instance().Initialize();
	Application::Instance().Run();
	Application::Instance().Terminate();
	return 0;
}