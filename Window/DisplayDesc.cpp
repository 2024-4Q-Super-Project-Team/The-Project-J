#include "pch.h"
#include "DisplayDesc.h"

Display::WindowDesc::WindowDesc()
{
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	/*WndClass.lpfnWndProc = WindowManager::WinProc;*/
	WndClass.cbClsExtra = NULL;
	WndClass.cbWndExtra = NULL;
	/*WndClass.hInstance = *Engine::Application::Instance;
	WndClass.hIcon = LoadIcon(*Engine::Application::Instance, IDI_APPLICATION);*/
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = L"Project";
}
