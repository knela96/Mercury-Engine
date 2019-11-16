#ifndef WINDOW_FILESYSTEM
#define WINDOW_FILESYSTEM
#include "Module.h"
#include "Application.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>

class WindowFileSystem : public Module
{
public:
	WindowFileSystem(Application* app, bool start_enabled = true);
	~WindowFileSystem();

	bool Start();
	bool Draw();
	bool Cleanup();
	bool isEnabled();

private:
	bool enabled;
};

#endif