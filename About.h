#ifndef ABOUT_H
#define ABOUT_H
#include "Application.h"
#include "Module.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>

class About : public Module
{
public:
	About(Application* app, bool start_enabled = true);
	~About();

	bool Start();

	update_status Update();
	update_status PreUpdate();
	update_status PostUpdate();
	bool Draw() const;
	bool Cleanup();
	bool isEnabled();
	
private:
	bool enabled = false;
	bool unFold = false;

};
#endif
