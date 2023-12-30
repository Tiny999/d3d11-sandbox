#include "App.h"
#include "Window.h"
#include <memory>
#include <algorithm>
#include "GDIPlusManager.h"
#include "MathUtils.h"
#include "imgui/imgui_impl_dx11.h"

#include <string>

GDIPlusManager gdipm;

App::App()  
	:
	wnd(1280, 720, "Direct3d11 Sandbox"),
	light(wnd.Gfx())
{
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
	wnd.DisableCursor();
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
	const auto dt = timer.Mark() * speed_factor;
	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	wnd.Gfx().SetCamera(camera.GetMatrix());
	light.Bind(wnd.Gfx(), camera.GetMatrix());


	nano.Draw(wnd.Gfx());
	// draw light
	light.Draw(wnd.Gfx());

	// imgui windows
	camera.SpawnControlWindow();
	light.SpawnControlWindow();
	nano.ShowWindow("nanosuit");
	ShowRawInputWindow();



	wnd.Gfx().EndFrame();
}


void App::ShowImguiDemoWindow()
{
	static bool show_demo_window = true;
	if (show_demo_window)
	{
		ImGui::ShowDemoWindow(&show_demo_window);
	};
}

void App::ShowRawInputWindow()
{
	while (const auto d = wnd.mouse.ReadRawDelta())
	{
		x += d->x;
		y += d->y;

		if (ImGui::Begin("Raw Input"))
		{
			ImGui::Text("Tally: (%d, %d)", x, y);
		}

		ImGui::End();
	}
}
