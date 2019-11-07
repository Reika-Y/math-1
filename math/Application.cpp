#include "Application.h"
#include <DxLib.h>
#include "Rendering.h"


void Application::Run(void)
{
	Rendering rendering;
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		rendering.RenderScanLine();
	}
}

void Application::Terminate(void)
{
	DxLib_End();
}

const Size Application::screenSize(void)
{
	return _screenSize;
}

Application::Application() :_screenSize{ 640,480 }
{	
}


Application::~Application()
{
	
}

int Application::Initialize(void)
{
	SetGraphMode(_screenSize.w, _screenSize.h, 16);	// 65536色モードに設定
	ChangeWindowMode(true);
	SetWindowText("Math");
	if (DxLib_Init() == -1)
	{
		return false;
	}
	return 0;
}
