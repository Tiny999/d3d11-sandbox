#pragma once
#include "Window.h"
#include "ChiliTimer.h"
#include "Drawable.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include "Mesh.h"
#include <set>

class App
{
public:
	App();
	// master frame || Message loop
	int Go();
private:
	void DoFrame();
	void ShowImguiDemoWindow();
private:
	Camera camera;
	ImGuiManager imgui;
	Window wnd;
	ChiliTimer timer;
	PointLight light;
	float speed_factor = 1.0f;
	Model nano{ wnd.Gfx(), "models\\nano.gltf" };
};

