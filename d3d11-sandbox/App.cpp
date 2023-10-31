#include "App.h"
#include "Window.h"
#include <memory>
#include <algorithm>
#include "Surface.h"
#include "GDIPlusManager.h"
#include "MathUtils.h"
#include "Sheet.h"
#include "Box.h"
#include "Pyramid.h"
#include "Cylinder.h"
#include "SkinnedBox.h"
#include "TestModel.h"
#include "imgui/imgui_impl_dx11.h"

#include <string>

GDIPlusManager gdipm;

App::App()  
	:
	wnd(800, 600, "Direct3d11 Sandbox"),
	light(wnd.Gfx())
{
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
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


	const auto transform = DirectX::XMMatrixRotationRollPitchYaw(pos.roll, pos.pitch, pos.yaw) *
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	nano.Draw(wnd.Gfx(), transform);
	// draw light
	light.Draw(wnd.Gfx());

	// imgui windows
	camera.SpawnControlWindow();
	light.SpawnControlWindow();
	wnd.Gfx().EndFrame();
}


void App::ShowModelWindow()
{
	if (ImGui::Begin("Model"))
	{
		using namespace std::string_literals;

		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &pos.roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &pos.pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &pos.yaw, -180.0f, 180.0f);

		ImGui::Text("Position");
		ImGui::SliderFloat("X", &pos.x, -20.0f, 20.0f);
		ImGui::SliderFloat("Y", &pos.y, -20.0f, 20.0f);
		ImGui::SliderFloat("Z", &pos.z, -20.0f, 20.0f);
	}
	ImGui::End();
}
