#include "Window.h"
#include "resource.h"
#include "imgui/imgui_impl_win32.h"
#include <sstream>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


const char* Window::WindowClass::GetName() noexcept
{
    return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
    return wndClass.hInst;
}

// initialize singleton instance
Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
    :
    hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
	wc.hCursor = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));;

	// Register Window Class
	RegisterClassEx(&wc);
}


Window::WindowClass::~WindowClass()
{ 
    UnregisterClass(wndClassName, GetInstance());
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg;

	// while message queue has messages, dispatch them (but do not block)
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// Check for quit message and return code to signify that app is quitting
		if (msg.message == WM_QUIT)
		{
			return msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}


	// return empty optional when not quitting app
	return {};
}

Window::Window(int width, int height, const char* name)
	:
	width(width),
	height(height)
{
	// calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top; 
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
	{
		throw WND_LAST_EXCEPT();
	}

	// Create window
	hWnd = CreateWindow(
		WindowClass::GetName(), name, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr, WindowClass::GetInstance(), this
	);

	if (hWnd == nullptr)
	{
		throw WND_LAST_EXCEPT();
	}

	// Show Window 
	ShowWindow(hWnd, SW_SHOWDEFAULT);

	// Init ImGui_Win32 Impl
	ImGui_ImplWin32_Init(hWnd);

	// Create graphics object
	pGfx = std::make_unique<Graphics>(hWnd);
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

void Window::SetTitle(const std::string title)
{
	if (SetWindowTextA(hWnd, title.c_str()) == 0)
	{
		throw WND_LAST_EXCEPT();
	}
}

Graphics& Window::Gfx()
{
	return *pGfx;
}

LRESULT Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // use create param passed in from CreateWindow() to store windowClass pointer
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

		// set winAPI managed user data to store ptr to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

		// forward message to window class handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// if message before WM_NCCREATE, handle with default window proc
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
   // retrieve ptr to window class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	// forward message to window class handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		return true;
	}

	const auto& imio = ImGui::GetIO();

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	// Clear keystate if window loses focus
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;

	/******************* KEYBOARD EVENTS ********************/
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (imio.WantCaptureKeyboard)
		{
			// If imgui capturing this message, don't pass on to window
			break;
		}

		if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled())
		{
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
		if (imio.WantCaptureKeyboard)
		{
			// If imgui capturing this message, don't pass on to window
			break;
		}

		kbd.onKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
	case WM_SYSKEYUP:
		if (imio.WantCaptureKeyboard)
		{
			// If imgui capturing this message, don't pass on to window
			break;
		}

		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;

	/******************* MOUSE EVENTS ********************/
	case WM_MOUSEMOVE:
	{
		if (imio.WantCaptureMouse)
		{
			// If imgui capturing this message, don't pass on to window
			break;
		}

		POINTS pt = MAKEPOINTS(lParam);
		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
		{
			// Mouse is in window, fire mouse move event
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow())
			{
				// Mouse is just entering the window for the first time, so fire mouse enter event and capture mouse
				SetCapture(hWnd);
				mouse.onMouseEnter();
			}
		}
		else
		{
			// If Mouse is outside window but key is still being pressed => Keep calling mouse move
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				mouse.OnMouseMove(pt.x, pt.y);
			}
			else
			{
				// Key no longer pressed => uncapture mouse
				ReleaseCapture();
				mouse.onMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		if (imio.WantCaptureMouse)
		{
			// If imgui capturing this message, don't pass on to window
			break;
		}

		POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		if (imio.WantCaptureMouse)
		{
			// If imgui capturing this message, don't pass on to window
			break;
		}

		POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		if (imio.WantCaptureMouse)
		{
			// If imgui capturing this message, don't pass on to window
			break;
		}

		POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		if (imio.WantCaptureMouse)
		{
			// If imgui capturing this message, don't pass on to window
			break;
		}

		POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		if (imio.WantCaptureMouse)
		{
			// If imgui capturing this message, don't pass on to window
			break;
		}

		POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.onWheelDelta(pt.x, pt.y, delta);
	}
	}

	return DefWindowProcA(hWnd, msg, wParam, lParam);
}

Window::WinException::WinException(int line, const char* file, HRESULT hr) noexcept
	:
	hr(hr), 
	Exception(line, file)
{}

const char* Window::WinException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetEroorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::WinException::GetType() const noexcept
{
	return "Custom: Window Exception";
}

std::string Window::WinException::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);
	if (nMsgLen == 0)
	{
		return "Unidentified Error Code";
	}

	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT Window::WinException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::WinException::GetEroorString() const noexcept
{
	return TranslateErrorCode(hr);
}
