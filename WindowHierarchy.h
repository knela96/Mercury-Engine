#ifndef HIERARCHY_H
#define HIERARCHY_H
#include "Module.h"
#include "Application.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>

class WindowHierarchy : public Module
{
public:
	WindowHierarchy(Application* app, bool start_enabled = true);
	~WindowHierarchy();

	bool Start();
	bool Draw();
	bool Cleanup();
	bool isEnabled();

private:
	bool enabled;
};
#endif
