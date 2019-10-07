#pragma once
#ifndef INSPECTOR_H
#define INSPECTOR_H
#include "Module.h"
#include "Application.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>

class WindowInspector : public Module
{
public:
	WindowInspector(Application* app, bool start_enabled = true);
	~WindowInspector();

	bool Start();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool Draw();
	bool Cleanup();
	bool isEnabled();

private:
	bool enabled;
	bool open_transform = true;
};
#endif
