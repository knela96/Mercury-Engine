#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

#include "SDL.h"

//#include "imgui.h"
//#include "imgui_impl_sdl.h"
//#include "imgui_impl_opengl3.h"
#include "ModuleGUI.h"

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
	// Start the Dear ImGui frame
	/*ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();


	bool r = NULL;
	ImGui::ShowDemoWindow(&r);*/

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


