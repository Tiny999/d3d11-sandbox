#include "App.h"
#include "Window.h"

App::App()
	:
	wnd(800, 600, "Direct3d11 Sandbox")
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

		DoFrame();
	}
}

void App::DoFrame()
{
	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	wnd.Gfx().ClearBuffer(c, c, 1.f);
	wnd.Gfx().DrawTestTriangle(
		timer.Peek(), 
		wnd.mouse.GetPosX() / 400.0f - 1.0f,
		-wnd.mouse.GetPosY() / 300.0f + 1.0f
	);
	wnd.Gfx().EndFrame();
}
