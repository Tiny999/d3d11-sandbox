#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(69);
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


int CALLBACK WinMain(
	HINSTANCE	hInstance, 
	HINSTANCE	hPrevInstance, 
	LPSTR		lpCmdLine,
	int			nCmdShow
) 
{
	const auto pClassName = "hw3d";

	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;
	wc.hIcon = nullptr;


	// Register Window Class
	RegisterClassEx(&wc);

	// Create window
	HWND hWnd = CreateWindowEx(
		0, pClassName,
		"Direct3D Sandbox",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		200, 200, 640, 480,
		nullptr, nullptr, hInstance, nullptr
	);

	// Show window
	ShowWindow(hWnd, SW_SHOW);

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