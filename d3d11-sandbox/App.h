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
	void ShowModelWindow();
private:
	Camera camera;
	ImGuiManager imgui;
	Window wnd;
	ChiliTimer timer;
	PointLight light;
	float speed_factor = 1.0f;
	Model nano{ wnd.Gfx(), "models\\nanosuit.obj" };
	struct
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	} pos;
};

