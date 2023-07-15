#include "App.h"
#include "Window.h"

App::App()
	:
	wnd(800, 300, "Direct3d11 Sandbox")
{
}

int App::Go()
{
	while (true)
	{
		// process all messages
		if (const auto ecode = Window::ProcessMessages())
		{
			// if ecode has value, return ecode (most likely will be exitcode 0)
			return *ecode;
		}
	}
}

void App::DoFrame()
{
}
