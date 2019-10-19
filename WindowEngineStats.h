#ifndef ENGINE_STATS_H
#define ENGINE_STATS_H
#include "Module.h"
#include "Application.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>

class WindowEngineStats : public Module
{
public:
	WindowEngineStats(Application* app, bool start_enabled = true);
	~WindowEngineStats();

	bool Start();
	bool Draw();
	bool Cleanup();
	bool isEnabled();

private:
	bool enabled;



	float FPS_array[20];
	float beforeLastFramefps = 0;
	float values_offset = 0;
};
#endif
