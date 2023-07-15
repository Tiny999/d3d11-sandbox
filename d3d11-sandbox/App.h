#pragma once
#include "Window.h"

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
};

