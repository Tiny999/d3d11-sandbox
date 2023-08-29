#pragma once
#include "Window.h"
#include "ChiliTimer.h"
#include "Drawable.h"

class App
{
public:
	App();
	// master frame || Message loop
	int Go();
private:
	void DoFrame();
private:
	Window wnd;
	ChiliTimer timer;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	static constexpr size_t nDrawables = 180;
};

