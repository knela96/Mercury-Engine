#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "ModuleGUI.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

ModuleGUI::ModuleGUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

// Destructor
ModuleGUI::~ModuleGUI()
{
}

// Called before render is available
bool ModuleGUI::Init()
{
	bool r = NULL;
	ImGui::ShowDemoWindow(r);
	return true;
}

// PreUpdate: clear buffer
update_status ModuleGUI::PreUpdate(float dt)
{
	

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleGUI::PostUpdate(float dt)
{
	
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleGUI::CleanUp()
{
	

	return true;
}


