#include "Application.h"
#include "WindowInspector.h"
#include "GameObject.h"
#include "MeshObject.h"
#include "Component.h"

WindowInspector::WindowInspector(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	enabled = true;
}


WindowInspector::~WindowInspector()
{
}


bool WindowInspector::Start()
{/*
	face_lenght = 1.0f;
	vertex_lenght = 1.0f;
	face_color = { 0.0f,1.0f,1.0f,1.0f };
	vertex_color = { 1.0f,0.0f,0.0f,1.0f };*/
	return true;
}

bool WindowInspector::Draw()
{
	if (App->gui->openInspector) {
		ImGui::SetNextWindowSizeConstraints(ImVec2(400, -1), ImVec2(1000, -1));
		ImGui::Begin("Inspector",&App->gui->openInspector);
		if (active_gameObject != nullptr) {

			ImGui::InputText("Name", (char*)&active_gameObject->name, 20); 

			if (ImGui::IsItemActive()) {
				App->input->writting = true;
			}
			else {
				App->input->writting = false;
			}

			ImGui::SameLine(); ImGui::Checkbox("Show", &active_gameObject->active);

			for (int i = 0; i < active_gameObject->components.size(); ++i) {
				active_gameObject->components[i]->Update();
			}

		}
		


		ImGui::End();
	}
	return false;
}

bool WindowInspector::Cleanup()
{
	return true;
}

bool WindowInspector::isEnabled()
{
	return enabled;
}
