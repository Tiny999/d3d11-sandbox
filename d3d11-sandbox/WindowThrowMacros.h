#pragma once

// Error exception helper macro
#define WND_EXCEPT(hr) Window::WinException(__LINE__, __FILE__, hr)
#define WND_LAST_EXCEPT() Window::WinException(__LINE__, __FILE__, GetLastError())