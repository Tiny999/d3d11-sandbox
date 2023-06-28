#include <Windows.h>

#include "Window.h"

int CALLBACK WinMain(
	HINSTANCE	hInstance, 
	HINSTANCE	hPrevInstance, 
	LPSTR		lpCmdLine,
	int			nCmdShow
) 
{
	Window wnd(800, 300, "Direct3d11 Sandbox");

	// Message Pump
	MSG msg;
	BOOL gResult;

	while ((gResult  =  GetMessage(&msg, nullptr, 0, 0))  > 0)
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	if (gResult == -1)
	{
		return -1;
	}

	return msg.wParam;
}