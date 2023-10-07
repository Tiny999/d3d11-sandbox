#pragma once
#include "Window.h"
#include "ChiliTimer.h"
#include "Drawable.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include <set>

class App
{
public:
	App();
	// master frame || Message loop
	int Go();
private:
	void DoFrame();
	void SpawnSimulationWindow() noexcept;
	void SpawnBoxWindowManagerWindow() noexcept;
	void SpawnBoxWindows() noexcept;
private:
	Camera camera;
	ImGuiManager imgui;
	Window wnd;
	ChiliTimer timer;
	PointLight light;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	std::vector<class Box*> boxes;
	static constexpr size_t nDrawables = 180;
	float speed_factor = 1.f;
	std::optional<int> comboBoxIndex = 0;
	std::set<int> boxControlIds;
};

