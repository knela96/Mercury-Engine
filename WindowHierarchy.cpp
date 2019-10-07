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

update_status WindowHierarchy::PreUpdate(float dt) {
	return UPDATE_CONTINUE;
}

update_status WindowHierarchy::PostUpdate(float dt) {
	return UPDATE_CONTINUE;
}

bool WindowHierarchy::Draw()
{
	ImGui::Begin("Hierarchy");
	if (ImGui::TreeNode("Tree"))
	{
		ImGui::Text("Node");
		ImGui::TreePop();
	}
	ImGui::End();
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
