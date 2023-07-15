#include <Windows.h>

#include "Window.h"
#include "Exception.h"
#include "App.h"
#include <sstream>
#include <iostream>

int CALLBACK WinMain(
	HINSTANCE	hInstance, 
	HINSTANCE	hPrevInstance, 
	LPSTR		lpCmdLine,
	int			nCmdShow
) 
{
	try {
		return App{}.Go();
	}
	catch (const Exception& e)
	{
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "No Available Information", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
}