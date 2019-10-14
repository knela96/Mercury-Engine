#include "Application.h"
#include "WindowHierarchy.h"

WindowHierarchy::WindowHierarchy(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	enabled = true;
}


WindowHierarchy::~WindowHierarchy()
{
}


bool WindowHierarchy::Start()
{
	return true;
}


bool WindowHierarchy::Draw()
{
	if (App->gui->openHirearchy) {
		ImGui::Begin("Hierarchy",&App->gui->openHirearchy);
		if (ImGui::TreeNode("Tree"))
		{
			ImGui::Text("Node");
			ImGui::TreePop();
		}
		ImGui::End();
	}
	return false;
}

bool WindowHierarchy::Cleanup()
{
	return true;
}

bool WindowHierarchy::isEnabled()
{
	return enabled;
}
