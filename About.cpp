#include "About.h"
#include "GameObject.h"
#include "Component.h"
#include "ModuleGUI.h"

About::About(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	enabled = true;
}


About::~About()
{
}

bool About::Start()
{
	return true;
}

update_status About::Update()
{
	return UPDATE_CONTINUE;
}

update_status About::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status About::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool About::Draw()
{
	if (App->gui->about) {
		ImGui::SetNextWindowSizeConstraints(ImVec2(400, -1), ImVec2(1000, -1));
		ImGui::Begin("About", &App->gui->openAbout);
		ImGui::End();		
	}
	return false;
}

bool About::Cleanup()
{
	return true;
}

bool About::isEnabled()
{
	return enabled;
}
