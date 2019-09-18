#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "ModuleGUI.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_sdl.h"
#include "imgui\imgui_impl_opengl3.h"

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
	return true;
}

// PreUpdate: clear buffer
update_status ModuleGUI::PreUpdate(float dt)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
	}

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	ImGui::ShowDemoWindow((bool *) true);

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


