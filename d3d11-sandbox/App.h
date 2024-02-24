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
	int x = 0, y = 0;
	Camera camera;
	ImGuiManager imgui;
	Window wnd;
	ChiliTimer timer;
	PointLight light;
	float speed_factor = 1.0f;
	Model nano{ wnd.Gfx(), "models\\nanosuit\\nanosuit.obj" };
	//Model wall{ wnd.Gfx(), "models\\brick_wall\\brick_wall.obj" };
};

